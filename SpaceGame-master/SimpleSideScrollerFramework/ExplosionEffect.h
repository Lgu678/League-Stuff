#pragma once
#include "Effect.h"
class ExplosionEffect : public Effect
{
public:
	ExplosionEffect();
	~ExplosionEffect();

	Effect* clone(Game *game);
};

