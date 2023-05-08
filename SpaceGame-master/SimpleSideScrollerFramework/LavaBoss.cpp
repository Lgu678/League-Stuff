#include "LavaBoss.h"
#include "physicsManager.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Player.h"


LavaBoss::LavaBoss()
{
	movementPattern = new SimpleMovement(this->body, SimpleMovement::SQUARE);
	//walkTicks = 20;
	//lastState = "north";
	//ticksMoved = 0;
	health = 2000;
	type = L"LAVA_BOSS";
	lavaBurstCooldown = 0;
	sinkCooldown = 40;
	ballCooldown = 10;
	invincible = false;
}



LavaBoss::~LavaBoss()
{
}

void LavaBoss::changeAnimationState()
{

}

void LavaBoss::think(Game *game)
{
	b2Vec2 playerPos = game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetPosition();
	//this->getBody()->GetWorldCenter();
	b2Vec2 bossPos = this->getBody()->GetPosition();
	float pixelScale = game->getGSM()->getPhyiscs()->getPixelScaling();
	if (health >= 3725 && health <= 3775)
	{
		if (sinkCooldown <= 400)
		{
			this->setCurrentState(L"SINK");
			this->setInvincible(true);
			if (lavaBurstCooldown <= 0)
			{
					this->setSelectedGun(LAVA_BURST);
					Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LAVA_BURST");
					//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
					bullet->setDamageType('P');
					bullet->setCurrentState(L"PRIMARY_FIRE");
					//float pX = (game->getGUI()->getViewport()->getViewportX()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
					//float pY = (game->getGUI()->getViewport()->getViewportY()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
					//float ppX = pX + ((rand() % 1000) * pixelScale);
					//float ppY = pY + ((rand() % 768 + 100) * pixelScale);
					game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
					//game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, ppX, ppY);
					game->getGSM()->getSpriteManager()->addActiveBullet(bullet);

					lavaBurstCooldown = 20;
			}
		}
		else
		{
			this->setHealth(3700);
			sinkCooldown = 0;
		}
		lavaBurstCooldown--;
		sinkCooldown++;
		return;
	} 
	else if (health >= 2475 && health <= 2525)
	{
		if (sinkCooldown <= 400)
		{
			this->setCurrentState(L"SINK");
			this->setInvincible(true);
			if (lavaBurstCooldown <= 0)
			{
				this->setSelectedGun(LAVA_BURST);
				Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LAVA_BURST");
				//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
				bullet->setDamageType('P');
				bullet->setCurrentState(L"PRIMARY_FIRE");
				//float pX = (game->getGUI()->getViewport()->getViewportX()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
				//float pY = (game->getGUI()->getViewport()->getViewportY()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
				//float ppX = pX + ((rand() % 1000) * pixelScale);
				//float ppY = pY + ((rand() % 768 + 100) * pixelScale);
				game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
				//game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, ppX, ppY);
				game->getGSM()->getSpriteManager()->addActiveBullet(bullet);

				lavaBurstCooldown = 20;
			}
		}
		else
		{
			this->setHealth(2450);
			sinkCooldown = 0;
		}

		lavaBurstCooldown--;
		sinkCooldown++;
		return;
	}
	else if (health >= 1225 && health <= 1275)
	{
		if (sinkCooldown <= 400)
		{
			this->setCurrentState(L"SINK");
			this->setInvincible(true);
			if (lavaBurstCooldown <= 0)
			{
				this->setSelectedGun(LAVA_BURST);
				Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LAVA_BURST");
				//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
				bullet->setDamageType('P');
				bullet->setCurrentState(L"PRIMARY_FIRE");
				//float pX = (game->getGUI()->getViewport()->getViewportX()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
				//float pY = (game->getGUI()->getViewport()->getViewportY()) * pixelScale; //* game->getGSM()->getPhyiscs()->getPixelScaling();
				//float ppX = pX + ((rand() % 1000) * pixelScale);
				//float ppY = pY + ((rand() % 768 + 100) * pixelScale);
				game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
				//game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, ppX, ppY);
				game->getGSM()->getSpriteManager()->addActiveBullet(bullet);

				lavaBurstCooldown = 20;
			}
		}
		else
		{
			this->setHealth(1200);
			sinkCooldown = 0;
		}
		lavaBurstCooldown--;
		sinkCooldown++;
		return;
	}



	if (this->getCurrentState() == L"SINK" || this->getCurrentState() == L"RISE")
	{
		this->setCurrentState(L"RISE");
		b2Vec2 bullPos;
		list<Bullet*>::iterator active = game->getGSM()->getSpriteManager()->getActiveBulletsIterator();
		while (active != game->getGSM()->getSpriteManager()->getEndOfActiveBulletsIterator())
		{
			Bullet *b = (*active);
			if (b->getType() == L"LAVA_BURST")
			{
				bullPos = b->getBody()->GetPosition();
				b->setCollided(true);
			}
			active++;
		}
		if (this->getCurrentState() == L"RISE" && this->getFrameIndex() == 14)
		{
			this->setCurrentState(L"IDLE_FOWARD");
			this->setInvincible(false);
		}
		return;
	}

	if ((this->isPlayerInRadius() || this->wasJustShot()) && this->getInvincible() != true)
	{

		if (ballCooldown <= 0)
		{
			this->setSelectedGun(LAVA_BALL);
			this->setCurrentState(L"SWIPE_FOWARD");
			Bullet *bullet = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LAVA_BALL");
			//game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav", false);
			bullet->setDamageType('P');
			bullet->setCurrentState(L"PRIMARY_FIRE");
			game->getGSM()->getPhyiscs()->activateEnemyBullet(bullet, this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
			game->getGSM()->getSpriteManager()->addActiveBullet(bullet);
			ballCooldown = 10;
		}
		else
		{
			ballCooldown--;
		}
		return;
	}


	if (this->getCurrentState() != L"SUBMERGE")
	{
		if ((playerPos.x >= 17.538 || playerPos.x <= 14.387) && (playerPos.y < 28.49 || playerPos.y > 26.6256294))
		{
			if (playerPos.x < bossPos.x)
				this->setCurrentState(L"IDLE_LEFT");
			else
				this->setCurrentState(L"IDLE_RIGHT");

		}
		else if (playerPos.y > bossPos.y + 1)
			this->setCurrentState(L"IDLE_FOWARD");
		else if (playerPos.y < bossPos.y)
			this->setCurrentState(L"IDLE_BACK");
		this->setInvincible(false);
	}
	else
		this->setInvincible(true);
}

Bot* LavaBoss::clone(Game *game)
{
	LavaBoss *newBot = new LavaBoss();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initLavaBoss(newBot, 192, 234, 50);
	newBot->movementPattern->setBody(newBot->getBody());
	return newBot;
}
