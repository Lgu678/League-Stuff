#pragma once
#include "src\sssf\gsm\ai\Bot.h"
#include "src\sssf\game\Game.h"
class MeleeBot :public Bot
{


public:
	MeleeBot();
	~MeleeBot();

	virtual void think(Game *game)=0;
	virtual Bot* clone(Game *game)=0;
	void reset(Game *game);

	bool isReadyToDamagePlayer()
	{
		return readyToDamagePlayer;
	}
	//MELEE ATTACK RADIUS
	bool isInAttackRadius()
	{
		return playerInAttackRadius;
	}

	void setInAttackRadius(bool b)
	{
		playerInAttackRadius = b;
	}

	void setReadyToDamagePlayer(bool b)
	{
		readyToDamagePlayer = b;
	}

protected:
	
	// THIS WILL ENABLE US TO DAMAGE THE PLAYER ONLY AT SPECIFIC MOMENTS IN TIME WHEN THE BOT IS ATTACKING
	bool readyToDamagePlayer;

	//TEST FOR NOW
	unsigned int dmgCountDown;

	// FOR MELEE BOTS THE PLAYER IS IN MELEE ATTACK RANGE
	bool playerInAttackRadius;
};

