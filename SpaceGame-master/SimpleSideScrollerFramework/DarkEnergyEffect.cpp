#include "DarkEnergyEffect.h"


DarkEnergyEffect::DarkEnergyEffect()
{
	body = NULL;
	type = L"DARKENERGY";
}


DarkEnergyEffect::~DarkEnergyEffect()
{

}

Effect* DarkEnergyEffect::clone(Game *game)
{
	DarkEnergyEffect *clone = new DarkEnergyEffect();
	clone->setSpriteType(this->getSpriteType());
	clone->setCurrentState(this->getCurrentState());
	clone->setAlpha(255);
	return clone;
}
