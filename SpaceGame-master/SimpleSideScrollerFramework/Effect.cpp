#include "Effect.h"


Effect::Effect()
{
	body = NULL;
	angle = 0.0f;
}


Effect::~Effect()
{
}

void Effect::reset()
{
	this->animationCounter = 0;
	this->frameIndex = 0;
	this->angle = 0.0f;
}

void Effect::update()
{
	AnimatedSprite::updateSprite();
}
