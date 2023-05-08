#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class Fireball :public Bullet
{
public:

	Fireball();
	~Fireball();

	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

