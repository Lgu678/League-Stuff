#include "LavaBall.h"
#include "src\sssf\gsm\state\GameStateManager.h"

LavaBall::LavaBall()
{
	speed = 10;
	initCollide = false;
	type = L"LAVA_BALL";
	primaryDamage = 10;
	//secondaryDamage = 30;
	//cooldown = 30;
}


LavaBall::~LavaBall()
{
}

void LavaBall::handleCollision(Game *game)
{
	initCollide = true;
}

void LavaBall::update(Game *game)
{
	AnimatedSprite::updateSprite();

	if (this->getCurrentState() == L"PRIMARY_FIRE")
	{
		int size = this->getSpriteType()->getSequenceSize(L"PRIMARY_FIRE");
		int index = this->getFrameIndex();
		if (this->getFrameIndex() == size - 2)
		{
			this->setCurrentState(L"FULLSIZE");
		}
	}
}


Bullet*	LavaBall::clone(Game *game)
{
	LavaBall *clone = new LavaBall();
	game->getGSM()->getPhyiscs()->initLavaBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
