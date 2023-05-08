#pragma once
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "stdafx.h"

class ShootingStar : public AnimatedSprite
{
private:

	int alphaController;
	int alphaDelayer;

public:
	ShootingStar();
	~ShootingStar();

	

	void updateSprite();
};

