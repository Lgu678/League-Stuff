#pragma once
#include "Box2D\Dynamics\b2WorldCallbacks.h"
#include <Box2D\Box2D.h>
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Bullet.h"
#include "Player.h"
class PhysicsCollisionListener :	public b2ContactListener
{

private:
	Game *game;

public:
	PhysicsCollisionListener();
	~PhysicsCollisionListener();

	void BeginContact(b2Contact *contact);


	void EndContact(b2Contact *contact);

	void setGame(Game *g)
	{
		game = g;
	}

};

