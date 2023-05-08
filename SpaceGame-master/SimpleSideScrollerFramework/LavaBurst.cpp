#include "LavaBurst.h"
#include "LavaBurstRayCast.h"
#include "src\sssf\gsm\state\GameStateManager.h"

LavaBurst::LavaBurst()
{
	speed = 10;
	initCollide = false;
	type = L"LAVA_BURST";
	primaryDamage = 50;
	//secondaryDamage = 30;
}


LavaBurst::~LavaBurst()
{
}

void LavaBurst::update()
{
	AnimatedSprite::updateSprite();

	if (this->getCurrentState() == L"PRIMARY_FIRE")
	{
		int size = this->getSpriteType()->getSequenceSize(L"PRIMARY_FIRE");
		int index = this->getFrameIndex();
		if (this->getFrameIndex() == size - 2)
		{
			this->setCurrentState(L"FULLSIZE");
			//this->handleCollision(game);
		}
	}
}

void LavaBurst::handleCollision(Game *game)
{
	if (this->getCurrentState() != L"FULLSIZE")
	{
		float pixelScale = game->getGSM()->getPhyiscs()->getPixelScaling();
		float angle = 0.0f;
		//initCollide = true;

		LavaBurstRayCast cast;
		cast.setIgnoreBody(this->getBody());
		cast.setIgnoreBody(game->getGSM()->getPhyiscs()->getPlayerBody());

		float x = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x;
		float y = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y;

		float pixelScaling = game->getGSM()->getPhyiscs()->getPixelScaling();
		float32 L = 100 * pixelScaling;
		b2Vec2 point1(this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);


		b2Vec2 point2;
		point2.x = point1.x + (49 * pixelScale);
		point2.y = point1.y + (54 * pixelScale);
		game->getGSM()->getPhyiscs()->getGameWorld()->RayCast(&cast, point1, point2);

		vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
		while (bodyIT != cast.getBodiesFoundEnd())
		{
			b2Body *body = *(bodyIT);
			Player *p = static_cast<Player*>(body->GetUserData());
			p->decPlayerHealth(5);
			//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
			bodyIT++;
		}
	}
	else
	{
		float pixelScale = game->getGSM()->getPhyiscs()->getPixelScaling();
		float angle = 0.0f;
		//initCollide = true;

		LavaBurstRayCast cast;
		cast.setIgnoreBody(this->getBody());
		cast.setIgnoreBody(game->getGSM()->getPhyiscs()->getPlayerBody());

		float x = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x;
		float y = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y;

		float pixelScaling = game->getGSM()->getPhyiscs()->getPixelScaling();
		float32 L = 100 * pixelScaling;
		b2Vec2 point1(this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);


			b2Vec2 point2;
			point2.x = point1.x + (47 * pixelScale);
			point2.y = point1.y + (136 * pixelScale);
			game->getGSM()->getPhyiscs()->getGameWorld()->RayCast(&cast, point1, point2);

		vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
		while (bodyIT != cast.getBodiesFoundEnd())
		{
			b2Body *body = *(bodyIT);
			Player *p = static_cast<Player*>(body->GetUserData());
			p->decPlayerHealth(5);
			//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
			bodyIT++;
		}
	}

	//vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
	//while (bodyIT != cast.getBodiesFoundEnd())
	//{
	//	b2Body *body = *(bodyIT);
	//	Bot *bot = static_cast<Bot*>(body->GetUserData());
	//	bot->setJustShot(true);
	//	//bot->setHealth(bot->getHealth() - this->splashDmg);
	//	bodyIT++;
	//}
	//game->getAudio()->playSound(L"data\\sounds\\rocket_explosion.wav", false);

	//initCollide = true;
}


Bullet*	LavaBurst::clone(Game *game)
{
	LavaBurst *clone = new LavaBurst();
	game->getGSM()->getPhyiscs()->initLavaBurstBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
