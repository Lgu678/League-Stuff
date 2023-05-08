#pragma once
#include "stdafx.h"
#include "physicsManager.h"
#define DEFAULT_MAX_WALK_TICKS 20

class SimpleMovement
{
public:
	SimpleMovement(b2Body* body, int movementType);
	SimpleMovement(b2Body* body, int movementType, int mWT);
	void setBody(b2Body* body) { _body = body; }
	~SimpleMovement();	
	void processNextMovement();
	void changeMovementType(int movementType);
	static enum { SQUARE, CIRCLE, TRIANGLE, LINEAR_HORIZONTAL, LINEAR_VERTICAL, IDLE, DIAMOND};
private:
	
	void square();
	void circular();
	void trianglular();
	void linearHorizontal();
	void linearVertical();
	void diamond();

	b2Body* _body;
	string orientation;
	int currentMovementType;
	int ticksWalked, maxWalkTicks;
};

