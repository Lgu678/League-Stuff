#include "ExplosionEffect.h"


ExplosionEffect::ExplosionEffect()
{
	body = NULL;
	type = L"EXPLOSION";
}


ExplosionEffect::~ExplosionEffect()
{
}

Effect* ExplosionEffect::clone(Game *game)
{
	ExplosionEffect *clone = new ExplosionEffect();
	clone->setSpriteType(this->getSpriteType());
	clone->setCurrentState(this->getCurrentState());
	clone->setAlpha(255);
	return clone;
}