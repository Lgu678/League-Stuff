#pragma once
#include "Bullet.h"
#include "stdafx.h"

static const unsigned int RECYCLABLE_BULLET_INCREMENT = 20;

class BulletRecycler
{

private:
	map<wstring, list<Bullet *> *> recyclableBullets;
	map<wstring, Bullet *> registeredBulletTypes;

public:
	BulletRecycler();
	~BulletRecycler();
	void addMoreBullets(Game *game, list<Bullet *> *bulletsNeededList, wstring bulletType, unsigned int numToAdd);
	void initRecyclableBullets(Game *game, wstring bulletType, unsigned int maxBulletsNeeded);
	bool isRegisteredBulletType(wstring bulletType);
	void recycleBullet(wstring bulletType, Bullet* bulletToRecycle);
	void registerBulletType(wstring bulletType, Bullet *sampleBullet);
	Bullet* retrieveBullet(Game *game, wstring bulletType);

	void reset(GameStateManager *gsm);
};

