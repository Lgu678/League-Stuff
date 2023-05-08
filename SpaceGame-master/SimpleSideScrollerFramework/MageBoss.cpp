#include "MageBoss.h"
#include "../Odyssey/Odyssey.h"
#include "Fireball.h"
#include "MageBullet.h"
#include "Lightning.h"
#include <cmath>
#include <math.h>


MageBoss::MageBoss()
{
	movementPattern = new SimpleMovement(this->body, SimpleMovement::SQUARE);
	bossHealth = 500;
	selectedGun = gunSelection::FIREBALL;
	fireCooldown = 30;
	bulletCooldown = 50;
	type = L"MAGE_BOSS";
	bulletFire = -1;
	pivot = false;
	fired = false;
	dashCooldown = 15;
	srand(time(NULL));
	orbsActive = false;
	lightningCooldown = 3;
	lightningAngle = 0.0f;
}



MageBoss::~MageBoss()
{
	for (int i = 0; i < 4; i++)
	{
		if (orbs[i] != NULL)
		{

			game->getGSM()->getPhyiscs()->getGameWorld()->DestroyBody(orbs[i]->getBody());
			delete orbs[i];
			orbs[i] = NULL;
		}
	}
}

void MageBoss::changeAnimationState()
{
}

void MageBoss::initOrbs()
{
	float x = this->getBody()->GetPosition().x;
	float y = this->getBody()->GetPosition().y;
	AnimatedSpriteType *st = game->getGSM()->getSpriteManager()->getSpriteType(13);

	for (int i = 0; i < 4; i++)
	{
		orbs[i] = new AnimatedSprite();
		orbs[i]->setSpriteType(st);
		orbs[i]->setAlpha(255);
		orbs[i]->setCurrentState(L"IDLE");
	}

	game->getGSM()->getPhyiscs()->initMageOrb(orbs[0], x, y - .0781, this);
	game->getGSM()->getPhyiscs()->initMageOrb(orbs[1], x, y + .0781, this);
	game->getGSM()->getPhyiscs()->initMageOrb(orbs[2], x - .0781, y, this);
	game->getGSM()->getPhyiscs()->initMageOrb(orbs[3], x + .0781, y, this);

	
}

void MageBoss::activateOrbs()
{
	orbsActive = true;

	b2Vec2 force;
	force.x = -2000;
	force.y = 0;
	game->getGSM()->getPhyiscs()->activateMageOrb(orbs[0], force);

	force.x = 2000;
	force.y = 0;
	game->getGSM()->getPhyiscs()->activateMageOrb(orbs[1], force);

	force.x = 0;
	force.y = -2000;
	game->getGSM()->getPhyiscs()->activateMageOrb(orbs[2], force);

	force.x = 0;
	force.y = 2000;
	game->getGSM()->getPhyiscs()->activateMageOrb(orbs[3], force);

}

void MageBoss::manageOrbs()
{
	float magePosX = this->getBody()->GetPosition().x;

	for (int i = 0; i < 4; i++)
	{
		float orbPosX = orbs[i]->getBody()->GetPosition().x;

		if (magePosX == orbPosX)
		{
			b2Vec2 force;
			force.x = -2000;
			force.y = 0;
			orbs[i]->getBody()->ApplyLinearImpulse(force, orbs[i]->getBody()->GetWorldCenter(), true);
		}

	}

}

void MageBoss::reallignOrbs()
{
	float posX = this->getBody()->GetPosition().x;
	float posY = this->getBody()->GetPosition().y;

	orbs[0]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
	orbs[1]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
	orbs[2]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
	orbs[3]->getBody()->SetLinearVelocity(b2Vec2(0, 0));

	orbs[0]->getBody()->SetTransform(b2Vec2(posX, posY - 15), 0.0f);
	orbs[1]->getBody()->SetTransform(b2Vec2(posX, posY + 15), 0.0f);
	orbs[2]->getBody()->SetTransform(b2Vec2(posX + 15, posY), 0.0f);
	orbs[3]->getBody()->SetTransform(b2Vec2(posX - 15, posY), 0.0f);

	b2Vec2 force;
	force.x = -2000;
	force.y = 0;
	orbs[0]->getBody()->ApplyLinearImpulse(force, orbs[0]->getBody()->GetWorldCenter(), true);
	force.x = 2000;
	force.y = 0;
	orbs[1]->getBody()->ApplyLinearImpulse(force, orbs[1]->getBody()->GetWorldCenter(), true);
	force.x = 0;
	force.y = -2000;
	orbs[2]->getBody()->ApplyLinearImpulse(force, orbs[2]->getBody()->GetWorldCenter(), true);
	force.x = 0;
	force.y = 2000;
	orbs[3]->getBody()->ApplyLinearImpulse(force, orbs[3]->getBody()->GetWorldCenter(), true);
}

void MageBoss::deactivateOrbs()
{
	orbsActive = false;

	for (int i = 0; i < 4; i++)
	{
		game->getGSM()->getPhyiscs()->deactivateMageOrb(orbs[i]);
	}
}

void MageBoss::destroyOrbs()
{
	orbsActive = false;
	//todo?
}

void MageBoss::think(Game *game)
{
	int pX = this->getBody()->GetPosition().x;
	int pY = this->getBody()->GetPosition().y;


	if (orbsActive)
		this->manageOrbs();

	if (dashCooldown <= 0)
	{
		this->dash(game);
	}

	if (this->getCurrentState() == L"IDLE" || this->getCurrentState() == L"IDLE_RIGHT")
	{
		if (this->getMarkedForDeath() == true)
		{
			this->setCurrentState(L"PHASE_TWO_IDLE");
			this->setMarkedForDeath(false);
			this->setHealth(500);
			this->initOrbs();
		}
		else
		{
			if (this->isPlayerInRadius() || this->wasJustShot())
			{

				if (this->getFireCooldown() <= 0)
				{
					this->setSelectedGun(FIREBALL);
					Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"FIREBALL");
					//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
					bullet->setDamageType('P');
					bullet->setCurrentState(L"PRIMARY_FIRE");
					game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
					game->getGSM()->getSpriteManager()->addActiveBullet(bullet);
					this->setFireCooldown(5);
				}
				else
				{
					this->decFireCooldown();
				}

				b2Vec2 vel = this->body->GetLinearVelocity();
				if (vel.x < 0)
					this->setCurrentState(L"IDLE");
				else
					this->setCurrentState(L"IDLE_RIGHT");
			}
			else
			{

				//movementPattern->processNextMovement();
				changeAnimationState();
			}
		}
	}
	else if (this->getCurrentState() == L"PHASE_TWO_IDLE")
	{
		if (this->getMarkedForDeath() == true)
		{
			this->setCurrentState(L"PHASE_THREE_IDLE");
			this->setMarkedForDeath(false);
			this->setHealth(500);
			this->activateOrbs();
			this->reallignOrbs();
		}
		else
		{
			if (this->isPlayerInRadius() || this->wasJustShot())
			{

				if (this->getBulletCooldown() <= 0)
				{
					this->setSelectedGun(MAGE_BULLET);
					Bullet *b = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"MAGE_BULLET");
					MageBullet *mageBullet = dynamic_cast<MageBullet*>(b);
					mageBullet->reset();
					mageBullet->setDamageType('P');
					mageBullet->setCurrentState(L"PRIMARY_FIRE");

					Bullet *b1 = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"MAGE_BULLET");
					MageBullet *mageBullet1 = dynamic_cast<MageBullet*>(b1);
					mageBullet1->reset();
					mageBullet1->setDamageType('P');
					mageBullet1->setCurrentState(L"PRIMARY_FIRE");

					Bullet *b2 = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"MAGE_BULLET");
					MageBullet *mageBullet2 = dynamic_cast<MageBullet*>(b2);
					mageBullet2->reset();
					mageBullet2->setDamageType('P');
					mageBullet2->setCurrentState(L"PRIMARY_FIRE");

					//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
					//Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"MAGE_BULLET");

					game->getGSM()->getPhyiscs()->activateEnemyBullet(mageBullet,0,-50);
					game->getGSM()->getPhyiscs()->activateEnemyBullet(mageBullet1,-30,30);
					game->getGSM()->getPhyiscs()->activateEnemyBullet(mageBullet2,30,-30);
					game->getGSM()->getSpriteManager()->addActiveBullet(mageBullet);
					game->getGSM()->getSpriteManager()->addActiveBullet(mageBullet1);
					game->getGSM()->getSpriteManager()->addActiveBullet(mageBullet2);
					this->setBulletCooldown(40);
					bulletFire = 10;
					fired = true;
				}
				else
				{
					this->decBulletCooldown();
				}
				/*if (fired == true)
				{
					if (bulletFire == 0)
					{
						b2Vec2 bullPos;
						list<Bullet*>::iterator active = game->getGSM()->getSpriteManager()->getActiveBulletsIterator();
						while (active != game->getGSM()->getSpriteManager()->getEndOfActiveBulletsIterator())
						{
							Bullet *b = (*active);
							if (b->getType() == L"MAGE_BULLET")
							{
								bullPos = b->getBody()->GetPosition();
								b->handleCollision(game);
							}
							active++;
						}

					}
					else
						bulletFire--;
				}*/

				if (this->getFireCooldown() <= 0)
				{
					this->setSelectedGun(FIREBALL);
					Bullet *fireBall = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"FIREBALL");
					//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
					fireBall->setDamageType('P');
					fireBall->setCurrentState(L"PRIMARY_FIRE");
					game->getGSM()->getPhyiscs()->activateEnemyBullet(fireBall, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
					game->getGSM()->getSpriteManager()->addActiveBullet(fireBall);
					this->setFireCooldown(5);
				}
				else
				{
					this->decFireCooldown();
				}

				b2Vec2 vel = this->body->GetLinearVelocity();
				if (vel.x < 0)
					this->setCurrentState(L"PHASE_TWO_IDLE");
				else
					this->setCurrentState(L"PHASE_TWO_IDLE");
			}
			else
			{

				//movementPattern->processNextMovement();
				changeAnimationState();
			}

		}
	}
	else if (this->getCurrentState() == L"PHASE_THREE_IDLE")
	{

		if (this->getMarkedForDeath() == true)
		{
			int size = this->getSpriteType()->getSequenceSize(L"DEATH");
			if (this->getFrameIndex() == size - 2)
			{
				this->setCurrentState(L"DEAD");
			}
			return;
		}

		if (this->isPlayerInRadius() || this->wasJustShot())
		{

			if (lightningCooldown <= 0)
			{
				//45
				Bullet *lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 45.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);
				//90
				lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 90.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);
				//135
				lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 135.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);
				//225
				lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 225.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);
				//270
				lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 270.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);
				//315
				lightning = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LIGHTNING");
				lightning->setDamageType('P');
				lightning->setCurrentState(L"PRIMARY_FIRE");
				game->getGSM()->getPhyiscs()->activateMageLightning(lightning, this, 315.0 + lightningAngle);
				game->getGSM()->getSpriteManager()->addActiveBullet(lightning);

				lightningCooldown = 1;
			}
			else
				lightningCooldown--;

			if (soundCooldown <= 0)
			{
				game->getAudio()->playSound(W_ELECTRIC_AMBIENT_SOUND_PATH, false);
				soundCooldown = 20;
			}
			else
				soundCooldown--;
		}

		lightningAngle += 2;
	}

}

void MageBoss::dash(Game *game)
{
	b2Body *botBody = this->getBody();
	b2Vec2 finalPos;


	bool validX = false;
	bool validY = false;

	while (!validX || !validY)
	{


		int r = (rand() % (2 - 0 + 1));
		int r2 = (rand() % (2 - 0 + 1));
		int dX = 1;
		int dY = 1;

		if (r < 1)
			dX = -1;
		else
			dX = 1;

		if (r2 < 1)
			dY = -1;
		else
			dY = 1;

		int maxV = rangeY;
		int maxH = rangeX;
		int distV = ((rand() % (maxV - 1))) + 1;
		int distH = ((rand() % (maxH - 1))) + 1;

		distV *= dY;
		distH *= dX;

		float xLowerBound = this->initPos.x - rangeX;
		float xUpperBound = this->initPos.x + rangeX;

		float yLowerBound = this->initPos.y - rangeY;
		float yUpperBound = this->initPos.y + rangeY;

		float curPosX = botBody->GetPosition().x;
		float curPosY = botBody->GetPosition().y;

		float testPosX = curPosX + distH;
		float testPosY = curPosY + distV;

		if (testPosX < xUpperBound && testPosX > xLowerBound)
			validX = true;

		if (testPosY < yUpperBound && testPosY > yLowerBound)
			validY = true;


		finalPos.x = testPosX;
		finalPos.y = testPosY;
	}






	botBody->SetTransform(finalPos, botBody->GetAngle());

	if (orbsActive)
	{
		this->reallignOrbs();
		game->getAudio()->playSound(W_MAGE_ORB_DASH_SOUND_PATH, false);
	}
	else
	{
		game->getAudio()->playSound(W_MAGE_DASH_SOUND_PATH, false);
	}


	//botBody->ApplyLinearImpulse(b2Vec2(30 * dX, 30 * dY), botBody->GetWorldCenter(), true);
	//botBody->SetTransform(b2Vec2(botBody->GetPosition().x + (1 * dX), (botBody->GetPosition().y + (1 * dY))), botBody->GetAngle());
	//setDashTime = false
	dashCooldown = 30;

}

Bot* MageBoss::clone(Game *game)
{
	MageBoss *newBot = new MageBoss();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initMageBoss(newBot, 40, 40, 50);
	newBot->movementPattern->setBody(newBot->getBody());
	newBot->registerGame(this->game);
	newBot->setRangeX(this->rangeX);
	newBot->setRangeY(this->rangeY);
	return newBot;
}