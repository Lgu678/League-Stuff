#pragma once
#include "Bullet.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
class ForceBullet :public Bullet
{
public:
	ForceBullet();
	~ForceBullet();
	void handleCollision(Game *game);
	void handleCollision(Game *game, AnimatedSprite* a);
	Bullet *clone(Game *game);
};

