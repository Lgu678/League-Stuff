#include "Fireball.h"
#include "src\sssf\gsm\state\GameStateManager.h"

Fireball::Fireball()
{
	speed = 10;
	initCollide = false;
	type = L"FIREBALL";
	primaryDamage = 10;
	//secondaryDamage = 30;
	//cooldown = 30;
}


Fireball::~Fireball()
{
}

void Fireball::handleCollision(Game *game)
{
	initCollide = true;
}



Bullet*	Fireball::clone(Game *game)
{
	Fireball *clone = new Fireball();
	game->getGSM()->getPhyiscs()->initEnemyBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
