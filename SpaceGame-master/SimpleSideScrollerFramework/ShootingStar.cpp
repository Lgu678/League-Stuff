#include "ShootingStar.h"


ShootingStar::ShootingStar()
{
	alphaController = -15;
	alphaDelayer = 2;
}


ShootingStar::~ShootingStar()
{
}

void ShootingStar::updateSprite()
{
	AnimatedSprite::updateSprite();

	/*if (alphaDelayer == 0)
	{

		if (this->alpha == 255)
		{
			alphaController = -15;
		}

		if (this->alpha <= 240)
		{
			alphaController = 15;
		}

		this->setAlpha(this->alpha + alphaController);
		alphaDelayer = 2;
	}
	else
		alphaDelayer--;*/
}