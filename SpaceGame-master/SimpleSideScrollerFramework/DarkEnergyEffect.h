#pragma once
#include "Effect.h"
class DarkEnergyEffect : public Effect
{
public:
	DarkEnergyEffect();
	~DarkEnergyEffect();

	Effect* clone(Game *game);
};

