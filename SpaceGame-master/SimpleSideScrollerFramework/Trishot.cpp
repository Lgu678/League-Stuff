#include "Trishot.h"
#include "src\sssf\gsm\state\GameStateManager.h"

Trishot::Trishot()
{
	speed = 15;
	initCollide = false;
	type = L"TRISHOT";
	primaryDamage = 25;
}


Trishot::~Trishot()
{
}

void Trishot::handleCollision(Game *game)
{
	initCollide = true;
}




Bullet*	Trishot::clone(Game *game)
{
	Trishot *clone = new Trishot();
	game->getGSM()->getPhyiscs()->initBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
