#pragma once
#include "Effect.h"
class SkullAttackEffect : public Effect
{
public:
	SkullAttackEffect();
	~SkullAttackEffect();

	Effect* clone(Game *game);
};

