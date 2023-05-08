#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class Laser :public Bullet
{
public:
	Laser();
	~Laser();

	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

