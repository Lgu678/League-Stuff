#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class AndromaliusBullet :public Bullet
{

private:

	int detonateTime;

public:

	AndromaliusBullet();
	~AndromaliusBullet();

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

