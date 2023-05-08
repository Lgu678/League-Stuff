#include "SkullBot.h"
#include "physicsManager.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Player.h"


SkullBot::SkullBot()
{
	movementPattern = new SimpleMovement(this->body, SimpleMovement::SQUARE);
	//walkTicks = 20;
	//lastState = "north";
	//ticksMoved = 0;
	type = L"SKULL_BOT";	
	dmgCountDown = 33;
}



SkullBot::~SkullBot()
{
}

void SkullBot::reset(Game *game)
{
	MeleeBot::reset(game);
}

void SkullBot::changeAnimationState()
{
	if (this->getMarkedForDeath() == true)
	{
		return;
	}


	b2Vec2 vel = this->body->GetLinearVelocity();
	if (vel.x < 0)
		this->setCurrentState(L"IDLE");
	else
		this->setCurrentState(L"IDLE_RIGHT");
	/*if (lastState.find("north") != -1)
	this->setCurrentState(states[1]);
	else if (lastState.find("east") != -1)
	this->setCurrentState(states[3]);
	else if (lastState.find("south") != -1)
	this->setCurrentState(states[2]);
	else//west
	this->setCurrentState(states[4]);*/


}

void SkullBot::think(Game *game)
{
	if (!(this->isPlayerInRadius() || this->wasJustShot()))
	{

		b2Vec2 vel = this->body->GetLinearVelocity();
		if (vel.x < 0)
			this->setCurrentState(L"IDLE");
		else
			this->setCurrentState(L"IDLE_RIGHT");

		if (this->getMarkedForDeath() == true)
		{
			int size = this->getSpriteType()->getSequenceSize(L"DEATH");
			if (this->getFrameIndex() == size - 2)
			{
				this->setCurrentState(L"DEAD");
			}
			return;
		}
	}


	if (this->isPlayerInRadius() || this->wasJustShot())
	{
		if (this->getCurrentState() != L"ATTACK")
			this->setCurrentState(L"ATTACK");

		b2Body *player = game->getGSM()->getPhyiscs()->getPlayerBody();
		b2Body *botBody = this->getBody();
		b2Vec2 movementVector;

		//THIS MAKES THE BOT RUN AWAY FROM THE PLAYER, COULD BE USEFULL
		/*	movementVector.x = botBody->GetPosition().x - player->GetPosition().x;
		movementVector.y = botBody->GetPosition().y - player->GetPosition().y;*/

		movementVector.x = player->GetPosition().x - botBody->GetPosition().x;
		movementVector.y = player->GetPosition().y - botBody->GetPosition().y;

		movementVector.Normalize();

		movementVector *= 2;

		botBody->SetLinearVelocity(movementVector);

		if (playerInAttackRadius == true)
		{

			// WHEN THE PLAYER IS IN THE ATTACK RADIUS WE NOW CAN CONTROL WHEN TO DAMAGE HIM
			// IT CAN BE ON A SET INTERVAL OR ON A SPECIFIC FRAME OF THE ATTACK ANIMATION

			if (dmgCountDown == 6)
			{
				Effect *skullAttack = game->getGSM()->getSpriteManager()->getEffectRecycler()->retrieveEffect(game, L"SKULLATTACKEFFECT");

				skullAttack->setDefaultX(((this->getBody()->GetPosition().x) / game->getGSM()->getPhyiscs()->getPixelScaling()));
				skullAttack->setDefaultY(((this->getBody()->GetPosition().y) / game->getGSM()->getPhyiscs()->getPixelScaling()));
				skullAttack->setCurrentState(L"ATTACK");

				float dx = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x - this->getBody()->GetPosition().x;
				float dy = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y - this->getBody()->GetPosition().y;
				float angle = atan2f(dy, dx);

				skullAttack->setAngle(angle);

				game->getGSM()->getSpriteManager()->addEffect(skullAttack);
			}

			dmgCountDown--;
			if (dmgCountDown == 0)
			{
				Player *player = game->getGSM()->getSpriteManager()->getPlayer();				
				game->getGSM()->getSpriteManager()->getPlayer()->decPlayerHealth(10);				
				dmgCountDown = 33;


			}

		}


	}
	else//Regular movement pattern
	{

		movementPattern->processNextMovement();
		changeAnimationState();
	}
}

Bot* SkullBot::clone(Game *game)
{
	SkullBot *newBot = new SkullBot();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initMeleeBot(newBot,32,32,25);
	newBot->movementPattern->setBody(newBot->getBody());
	return newBot;
}
