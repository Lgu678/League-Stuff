#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
#include "ExplosionRayCast.h"

class Rocket :	public Bullet
{
private:
	int splashDmg = 30;

public:
	Rocket();
	~Rocket();

	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

