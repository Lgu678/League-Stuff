#include "RatBot.h"


RatBot::RatBot()
{

	movementPattern = new SimpleMovement(this->body, SimpleMovement::IDLE);
	//walkTicks = 20;
	//lastState = "north";
	//ticksMoved = 0;
	type = L"RAT_BOT";
	this->setCurrentState(L"IDLE");
}


RatBot::~RatBot()
{
}

void RatBot::reset(Game *game)
{
	MeleeBot::reset(game);
}

void RatBot::think(Game *game)
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
		b2Vec2 movementVector;
		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;

		if (this->getCurrentState() != L"ATTACK" || this->getCurrentState() != L"ATTACK_LEFT")
		{
			b2Vec2 vel = this->body->GetLinearVelocity();
			finishedAttacking = false;
			if (movementVector.x > 0)
				this->setCurrentState(L"ATTACK");
			else
				this->setCurrentState(L"ATTACK_LEFT");
		}

		//botBody->SetTransform(botBody->GetPosition(), angle);

		if (this->getFrameIndex() == 8)
			readyToDamagePlayer = true;

		/*b2Vec2 movementVector;
		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;*/

		float angle = atan2f(movementVector.y, movementVector.x);


		if (readyToDamagePlayer)
		{
			Player *player = game->getGSM()->getSpriteManager()->getPlayer();
			//player->setPlayerHealth(player->getPlayerHealth() - 10);
			game->getGSM()->getSpriteManager()->getPlayer()->decPlayerHealth(50);
			game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
			readyToDamagePlayer = false;
			thinkTime = 10;

		}

		b2Vec2 reset;
		reset.x = 0.0f;
		reset.y = 0.0f;

		botBody->SetLinearVelocity(reset);



	}
	if (this->getCurrentState() == L"ATTACK" || this->getCurrentState() == L"ATTACK_LEFT")
	{
		if (this->getFrameIndex() == 8)
		{
			finishedAttacking = true;
			//chase = true;
		}
	}

	if (((this->isPlayerInRadius() || this->wasJustShot()) && playerInAttackRadius == false && finishedAttacking == true) || (chase == true && finishedAttacking == true))
	{
		b2Vec2 vel = this->body->GetLinearVelocity();
		//b2Vec2 vel = this->body->GetLinearVelocity();
		if (this->getCurrentState() != L"WALKING" || this->getCurrentState() != L"WALKING_LEFT")
		{
			if (vel.x > 0)
				this->setCurrentState(L"WALKING");
			else
				this->setCurrentState(L"WALKING_LEFT");
		}


		chase = true;
		b2Vec2 movementVector;

		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;

		float angle = atan2f(movementVector.y, movementVector.x);

		movementVector.Normalize();

		movementVector *= 3.5;

		botBody->SetTransform(botBody->GetPosition(), 0);

		botBody->SetLinearVelocity(movementVector);

	}
	else
	{
	}

}


Bot* RatBot::clone(Game *game)
{
	RatBot *newBot = new RatBot();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initMeleeBot(newBot, 124, 35, 50);
	newBot->movementPattern->setBody(newBot->getBody());
	return newBot;
}
