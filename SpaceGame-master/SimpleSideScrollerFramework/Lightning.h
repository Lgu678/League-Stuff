#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
class Lightning : public Bullet
{
public:
	Lightning();
	~Lightning();

	void update(Game *game);

	void handleCollision(Game *game);
	Bullet *clone(Game *game);
};

