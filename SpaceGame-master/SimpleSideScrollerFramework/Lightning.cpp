#include "Lightning.h"
#include "src\sssf\gsm\state\GameStateManager.h"


Lightning::Lightning()
{
	speed = 20;
	initCollide = false;
	type = L"LIGHTNING";
	primaryDamage = 5;
}


Lightning::~Lightning()
{
}

void Lightning::handleCollision(Game *game)
{
	initCollide = true;
}

Bullet*	Lightning::clone(Game *game)
{
	Lightning *clone = new Lightning();
	game->getGSM()->getPhyiscs()->initEnemyBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}