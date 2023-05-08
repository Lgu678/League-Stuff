#pragma once
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "src\sssf\game\Game.h"
#include "stdafx.h"

class Effect :	public AnimatedSprite
{

protected:

	wstring type;
	float angle;

public:
	Effect();
	~Effect();

	void update();
	wstring getType()
	{
		return type;
	}
	void reset();
	float getAngle(){ return angle; }
	void setAngle(float f){ angle = f; }

	virtual Effect* clone(Game *game) = 0;
};

