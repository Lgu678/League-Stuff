#pragma once
#include "MeleeBot.h"
#include "src\sssf\game\Game.h"
#include "SimpleMovement.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "src\sssf\gsm\sprite\SpriteManager.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Player.h"
class SkullBot : public MeleeBot
{
private:
	//int walkTicks;
	//string lastState;
	//int ticksMoved;
	SimpleMovement* movementPattern; 
public:
	SkullBot();
	~SkullBot();
	void changeAnimationState();
	void changeMovementType(int type) { movementPattern->changeMovementType(type); }
	//void setWalkTicks(int wT) { walkTicks = wT; }
	Bot* clone(Game *game);
	void think(Game *game);

	void reset(Game *game);

};

