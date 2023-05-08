#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class Trishot : public Bullet
{
public:
	Trishot();
	~Trishot();


	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

