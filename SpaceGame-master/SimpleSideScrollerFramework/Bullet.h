#pragma once
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "stdafx.h"
#include "src\sssf\game\Game.h"

class Bullet :	public AnimatedSprite
{

protected:

	float speed;
	bool initCollide = false;
	wstring type;
	int primaryDamage;
	//int secondaryDamage;
	char damageType;

public:
	Bullet();
	~Bullet();

	virtual void update();
	int getPrimaryDamage()
	{
		return primaryDamage;
	}

	int getDamage()
	{
		return primaryDamage;
	}
	void setDamageType(char c) { damageType = c; }
	//int getSecondaryDamage() { return secondaryDamage; }
	wstring getType()
	{
		return type;
	}

	bool hasCollided()
	{
		return initCollide;
	}

	void setCollided(bool b)
	{
		initCollide = b;
	}

	float getSpeed()
	{
		return speed;
	}

	void setSpeed(float s)
	{
		speed = s;
	}

	void setPrimaryDamage(int x)
	{
		primaryDamage = x;
	}
	
	//void setSecondaryDamage(int x) { secondaryDamage = x};
	
	// WE WILL HAVE MANY TYPES OF BULLETS THAT WILL DO DIFFERANT THINGS ON IMPACT, THE DEFAULT BEHAVOIR THOUGH
	// IS TO SIMPLY DISSAPEAR AND IF IT HIS AN ENEMY DAMAGE IT
	virtual void handleCollision(Game *game)=0;
	virtual Bullet* clone(Game *game)=0;
		
	
};

