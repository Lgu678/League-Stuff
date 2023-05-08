#pragma once

#include "../../../../stdafx.h"
#include "../../game/Game.h"
#include "../../gsm/sprite/AnimatedSprite.h"

class Bot : public AnimatedSprite
{
protected:

	int health;
	bool playerInRadius;
	bool justShot;
	wstring type;

public:
	Bot()	{}
	~Bot()	{}

	bool wasJustShot()
	{
		return justShot;
	}

	wstring getType()
	{
		return type;
	}

	bool isPlayerInRadius()
	{
		return playerInRadius;
	}

	void setPlayerInRadius(bool b)
	{
		playerInRadius = b;
	}

	int getHealth()
	{
		return health;
	}

	void setHealth(int x)
	{
		health = x;
	}

	void setJustShot(bool b)
	{
		justShot = b;
	}



	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
	virtual Bot* clone(Game *game)=0;
	virtual void reset(Game *game)=0;
};