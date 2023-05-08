#pragma once
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "src\sssf\game\Game.h"
class Player :	public AnimatedSprite
{
private:

	int playerHealth;
	int selectedGun;
	unsigned int nextShowCountdown;
	bool collidingWithBot;
	unsigned int playerLives;
	bool outOfHealth;
	bool outOfLives;
	int playerStartingHealth;
	int immuneCounter;
	bool isImmune;
	Game *game;

	AnimatedSprite *status;
	unsigned int statusLoop;

	AnimatedSprite *flameThrower;
	bool flameThrowerOn;

	int initX;
	int initY;

	float mouseAngle;
	

public:
	Player();
	~Player();

	enum gunSelection
	{
		DEFAULT,
		SPREAD,
		ROCKET,
		FLAMETHROWER,
		FORCE

	};

	void setInitPos(int x, int y)
	{
		initX = x;
		initY = y;
	}

	int getInitX()
	{
		return initX;
	}

	int getInitY()
	{
		return initY;
	}

	void setMouseAngle(float x)
	{
		mouseAngle = x;
	}

	float getMouseAngle()
	{
		return mouseAngle;
	}

	void initStatusSprite();
	void initFlameThrower();

	void updateStatus();
	void updateFlameThrower();

	void startFlameThrower();
	void stopFlameThrower();

	bool isFlameThrowerOn()
	{
		return flameThrowerOn;
	}

	AnimatedSprite* getStatus()
	{
		return status;
	}

	AnimatedSprite* getFlameThrower()
	{
		return flameThrower;
	}

	void registerPlayer(Game *g)
	{
		game = g;
	}

	bool isOutOfLives()
	{
		return outOfLives;
	}

	void decPlayerHealth(int x);

	int getStartingHealth()
	{
		return playerStartingHealth;
	}

	void setPlayerLives(unsigned int x)
	{
		playerLives = x;
	}
	unsigned int getPlayerLives()
	{
		return playerLives;
	}
	void setSelectedGun(unsigned int x)
	{
		selectedGun = x;
	}

	 int getSelectedGun()
	{
		return selectedGun;
	}

	void updateSprite();
	int	getPlayerHealth()	{ return playerHealth; }

	void setPlayerHealth(unsigned int x)
	{
		playerHealth = x;
	}

	void setStartingHealth(int x)
	{
		playerStartingHealth = x;
	}

	unsigned int getNextShotTime()
	{
		return nextShowCountdown;
	}

	void setNextShotTime(unsigned int x)
	{
		nextShowCountdown = x;
	}

	void decNextShotTime()
	{
		if (nextShowCountdown > 0)
		nextShowCountdown--;
	}

	void setCollidingWithBot(bool b)
	{
		collidingWithBot = b;
	}

	bool isCollidingWithBot()
	{
		return collidingWithBot;
	}

	void setStatus(wstring stat, int time);

	void reset()
	{
		selectedGun = gunSelection::DEFAULT;
		playerHealth = 200;
		nextShowCountdown = 2;
		collidingWithBot = false;
		playerLives = 3;
		playerStartingHealth = 200;
		outOfLives = false;
		outOfHealth = false;
		immuneCounter = 66;
		isImmune = false;
		statusLoop = 0;

		if (status != NULL)
		status->setCurrentState(L"NONE");

		if (flameThrower != NULL)
		flameThrower->setCurrentState(L"OFF");
		
	}
};



