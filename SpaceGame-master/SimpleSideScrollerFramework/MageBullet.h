#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class MageBullet :public Bullet
{

private:

	int detonateTime;

public:

	MageBullet();
	~MageBullet();

	void detonate(Game *game);
	void decDetonateTime();
	int getDetonateTime()
	{
		return detonateTime;
	}

	void reset();

	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

