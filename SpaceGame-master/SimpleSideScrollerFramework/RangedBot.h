#pragma once
#include "src\sssf\gsm\ai\Bot.h"
#include "src\sssf\game\Game.h"
class RangedBot :
	public Bot
{

protected:


	unsigned int dmgCountDown;

	bool playerInRange;

public:
	RangedBot();
	~RangedBot();

	virtual void think(Game *game) = 0;
	virtual Bot* clone(Game *game) = 0;


	bool isInAttackRange()
	{
		return playerInRange;
	}

	void setInAttackRange(bool b)
	{
		playerInRange = b;
	}

	void reset(Game *game)
	{}
};

