#pragma once
#include "MeleeBot.h"
#include "src\sssf\game\Game.h"
#include "SimpleMovement.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "src\sssf\gsm\sprite\SpriteManager.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Player.h"
class MageBoss : public MeleeBot
{
private:
	//int walkTicks;
	//string lastState;
	//int ticksMoved;
	SimpleMovement* movementPattern;
	unsigned int bossHealth;
	int selectedGun;
	int fireCooldown;
	int bulletCooldown;
	int bulletFire;
	int dashCooldown;
	boolean pivot;
	boolean fired;
	bool dashB;
	int lightningCooldown;
	int soundCooldown;
	float rangeX;
	float rangeY;
	b2Vec2 initPos;

	AnimatedSprite *orbs[4];

	Game *game;

	bool orbsActive;

	float lightningAngle;


	const float pixelScaling = 0.009765625;

public:
	MageBoss();
	~MageBoss();

	enum gunSelection
	{
		FIREBALL,
		MAGE_BULLET,
		ACID

	};

	void manageOrbs();

	bool areOrbsActive()
	{
		return orbsActive;
	}

	void destroyOrbs();

	void reallignOrbs();

	AnimatedSprite *getOrbs(int i)
	{
		return orbs[i];
	}

	void setOrbsActive(bool b)
	{
		orbsActive = b;
	}

	void setSelectedGun(unsigned int x)
	{
		selectedGun = x;
	}

	void registerGame(Game *g)
	{
		game = g;
	}

	void setInitPos(float x, float y) { initPos.x = x; initPos.y = y; }
	void setRangeX(int x){ rangeX = x*pixelScaling; }
	void setRangeY(int y){ rangeY = y*pixelScaling; }

	int getSelectedGun()
	{
		return selectedGun;
	}

	int	getBossHealth()	{ return bossHealth; }

	boolean getPivot() { return pivot; }
	void setPivot(boolean p) { pivot = p; }

	void decDash()
	{
		dashCooldown--;
	}

	void initOrbs();
	void activateOrbs();
	void deactivateOrbs();

	void setBossHealth(unsigned int x)
	{
		bossHealth = x;
	}

	unsigned int getFireCooldown()
	{
		return fireCooldown;
	}

	void setFireCooldown(int x)
	{
		fireCooldown = x;
	}

	void decFireCooldown()
	{
		fireCooldown--;
	}

	unsigned int getBulletCooldown()
	{
		return bulletCooldown;
	}

	void setBulletCooldown(int x)
	{
		bulletCooldown = x;
	}

	void decBulletCooldown()
	{
		bulletCooldown--;
	}

	float getLightningAngle()
	{
		return lightningAngle;
	}

	void changeAnimationState();
	void dash(Game *game);
	void changeMovementType(int type) { movementPattern->changeMovementType(type); }
	//void setWalkTicks(int wT) { walkTicks = wT; }
	Bot* clone(Game *game);
	void think(Game *game);
	void reset(Game *game){}

};