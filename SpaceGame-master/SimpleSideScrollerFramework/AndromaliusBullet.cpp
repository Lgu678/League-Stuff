#include "AndromaliusBullet.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "DarkEnergyRayCast.h"

AndromaliusBullet::AndromaliusBullet()
{
	speed = 0;
	initCollide = false;
	type = L"Andromalius_BULLET";
	primaryDamage = 50;
	//this->setSecondaryDaAndromalius(50);
	//cooldown = 50;
	//detonateTime = 40;
}


AndromaliusBullet::~AndromaliusBullet()
{
}

void AndromaliusBullet::handleCollision(Game *game)
{
	initCollide = true;
	//this->detonate(game);

}


void AndromaliusBullet::detonate(Game *game)
{


	float angle = 0.0f;
	DarkEnergyRayCast cast;
	cast.setIgnoreBody(this->getBody());

	float pixelScaling = game->getGSM()->getPhyiscs()->getPixelScaling();

	float x = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x;
	float y = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y;

	float32 L = 65 * pixelScaling;
	b2Vec2 point1(this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
	for (int i = 0; i < 180; i++)
	{
		b2Vec2 d(L * cosf(angle), L * sinf(angle));
		b2Vec2 point2 = point1 + d;
		game->getGSM()->getPhyiscs()->getGameWorld()->RayCast(&cast, point1, point2);
		angle += 2.0f * b2_pi / 180.0f;
	}

	vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
	while (bodyIT != cast.getBodiesFoundEnd())
	{
		b2Body *body = *(bodyIT);
		Player *p = static_cast<Player*>(body->GetUserData());
		p->decPlayerHealth(50);
		//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
		bodyIT++;
	}

	// play a sound

	b2Vec2 bullPos = this->getBody()->GetPosition();
	Effect *darkEnergy = game->getGSM()->getSpriteManager()->getEffectRecycler()->retrieveEffect(game, L"DARKENERGY");
	darkEnergy->setDefaultX(((bullPos.x) / pixelScaling) - 50);
	darkEnergy->setDefaultY(((bullPos.y) / pixelScaling) - 50);
	game->getGSM()->getSpriteManager()->addEffect(darkEnergy);
	initCollide = true;
}

void AndromaliusBullet::reset()
{
	initCollide = false;
	detonateTime = 40;
}

void AndromaliusBullet::decDetonateTime()
{
	detonateTime--;

}


Bullet*	AndromaliusBullet::clone(Game *game)
{
	AndromaliusBullet *clone = new AndromaliusBullet();
	game->getGSM()->getPhyiscs()->initEnemyBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	clone->detonateTime = 40;
	return clone;
}
