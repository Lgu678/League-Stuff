#include "SpiderBot.h"
#include "../Odyssey/Odyssey.h"


SpiderBot::SpiderBot()
{

	movementPattern = new SimpleMovement(this->body, SimpleMovement::IDLE);
	//walkTicks = 20;
	//lastState = "north";
	//ticksMoved = 0;
	type = L"SPIDER_BOT";
	this->setCurrentState(L"IDLE");
}


SpiderBot::~SpiderBot()
{
}


void SpiderBot::reset(Game *game)
{
	finishedAttacking = true;
    chase = false;
	MeleeBot::reset(game);
}

void SpiderBot::think(Game *game)
{




	b2Body *player = game->getGSM()->getPhyiscs()->getPlayerBody();
	b2Body *botBody = this->getBody();

	float dY = botBody->GetPosition().y - player->GetPosition().y;
	float dX = botBody->GetPosition().x - player->GetPosition().x;

	dY = dY * dY;
	dX = dX * dX;

	float distance = sqrt(dX + dY);



	if (this->getMarkedForDeath() == true)
	{
		int size = this->getSpriteType()->getSequenceSize(L"DEATH");
		if (this->getFrameIndex() == size - 2)
		{
			this->setCurrentState(L"DEAD");
		}
		return;
	}


	if (playerInAttackRadius == true)
	{

		// WHEN THE PLAYER IS IN THE ATTACK RADIUS WE NOW CAN CONTROL WHEN TO DAMAGE HIM
		// IT CAN BE ON A SET INTERVAL OR ON A SPECIFIC FRAME OF THE ATTACK ANIMATION
		if (this->getCurrentState() != L"ATTACKING")
		{
			finishedAttacking = false;
			this->setCurrentState(L"ATTACKING");
		}

		//botBody->SetTransform(botBody->GetPosition(), angle);

		if (this->getFrameIndex() == 10)
			readyToDamagePlayer = true;

		b2Vec2 movementVector;
		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;

		float angle = atan2f(movementVector.y, movementVector.x);


		if (readyToDamagePlayer)
		{
			Player *player = game->getGSM()->getSpriteManager()->getPlayer();
			//player->setPlayerHealth(player->getPlayerHealth() - 10);
			game->getGSM()->getSpriteManager()->getPlayer()->decPlayerHealth(50);
			//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
			readyToDamagePlayer = false;
			game->getAudio()->playSound(W_SPIDER_ATTACK_SOUND_PATH, false);
			thinkTime = 10;

		}

		b2Vec2 reset;
		reset.x = 0.0f;
		reset.y = 0.0f;

		botBody->SetLinearVelocity(reset);



	}
	if (this->getCurrentState() == L"ATTACKING")
	if (this->getFrameIndex() == 16)
		finishedAttacking = true;
	

	if (((this->isPlayerInRadius() || this->wasJustShot()) && playerInAttackRadius == false && finishedAttacking == true) || (chase == true && finishedAttacking == true))
	{

		b2Vec2 vel = this->body->GetLinearVelocity();
		if (this->getCurrentState() != L"RUNNING")
			this->setCurrentState(L"RUNNING");


		chase = true;
		b2Vec2 movementVector;

		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;

		float angle = atan2f(movementVector.y, movementVector.x);

		movementVector.Normalize();

		movementVector *= 3.5;

		botBody->SetTransform(botBody->GetPosition(), angle);

		botBody->SetLinearVelocity(movementVector);




	}
	else
	{
	}

}


Bot* SpiderBot::clone(Game *game)
{
	SpiderBot *newBot = new SpiderBot();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initMeleeBot(newBot, 64, 64, 60);
	newBot->movementPattern->setBody(newBot->getBody());
	return newBot;
}
