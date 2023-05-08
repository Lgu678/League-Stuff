#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class LavaBurst :public Bullet
{
public:
	LavaBurst();
	~LavaBurst();


	void update();
	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

