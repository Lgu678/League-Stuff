#include "RangedBot.h"


RangedBot::RangedBot()
{
	this->setPlayerInRadius(false);
	this->setJustShot(false);
	playerInRange = false;
	dmgCountDown = 33;
}


RangedBot::~RangedBot()
{
}


