#include "SkullAttackEffect.h"


SkullAttackEffect::SkullAttackEffect()
{
	body = NULL;
	type = L"SKULLATTACKEFFECT";
}


SkullAttackEffect::~SkullAttackEffect()
{
}

Effect* SkullAttackEffect::clone(Game *game)
{
	SkullAttackEffect *clone = new SkullAttackEffect();
	clone->setSpriteType(this->getSpriteType());
	clone->setCurrentState(this->getCurrentState());
	clone->setAlpha(255);
	return clone;
	
}
