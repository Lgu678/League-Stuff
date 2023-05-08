#include "Laser.h"
#include "src\sssf\gsm\state\GameStateManager.h"

Laser::Laser()
{
	speed = 10;
	initCollide = false;
	type = L"LASER";
	primaryDamage = 10;
	//secondaryDamage = 30;
}


Laser::~Laser()
{
}

void Laser::handleCollision(Game *game)
{
	initCollide = true;
}




Bullet*	Laser::clone(Game *game)
{
	Laser *clone = new Laser();
	game->getGSM()->getPhyiscs()->initBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
