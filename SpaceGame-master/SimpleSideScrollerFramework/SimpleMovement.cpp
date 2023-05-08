#include "SimpleMovement.h"


SimpleMovement::SimpleMovement(b2Body* body, int movementType)
{
	_body = body;
	ticksWalked = 0;
	maxWalkTicks = DEFAULT_MAX_WALK_TICKS;
	currentMovementType = movementType;

	if (movementType == SQUARE)
		orientation = "north";
	else if (movementType == LINEAR_HORIZONTAL)
		orientation = "east";
	else if (movementType == DIAMOND)
		orientation = "north-east";
	else//LINEAR_VERTICAL or whatever else
		orientation = "north";

	
}
SimpleMovement::SimpleMovement(b2Body* body, int movementType, int mWT)
{
	//SimpleMovement(body, movementType);
	_body = body; 
	ticksWalked = 0;
	maxWalkTicks = DEFAULT_MAX_WALK_TICKS;
	currentMovementType = movementType; 

	maxWalkTicks = mWT; 

	if (movementType == SQUARE)
		orientation = "north";
	else if (movementType == LINEAR_HORIZONTAL)
		orientation = "east";
	else if (movementType == DIAMOND)
		orientation = "north-east";
	else//LINEAR_VERTICAL or whatever else
		orientation = "north";
}

SimpleMovement::~SimpleMovement()
{
}

void SimpleMovement::changeMovementType(int movementType)
{
	currentMovementType = movementType;
}

void SimpleMovement::processNextMovement()//Square
{
	if (currentMovementType == SQUARE)
		square();
	else if (currentMovementType == LINEAR_HORIZONTAL)
		linearHorizontal();
	else if (currentMovementType == LINEAR_VERTICAL)
		linearVertical();
	else if (currentMovementType == DIAMOND)
		diamond();
	//Else IDLE
}
void SimpleMovement::diamond()
{
	b2Vec2 theForce;
	if (ticksWalked < maxWalkTicks)
	{
		if (orientation.find("north-east") != -1)
			theForce = b2Vec2(1, 1);
		else if (orientation.find("south-east") != -1)
			theForce = b2Vec2(1, -1);
		else if (orientation.find("south-west") != -1)
			theForce = b2Vec2(-1, -1);
		else//north-west
			theForce = b2Vec2(-1, 1);
		_body->SetLinearVelocity(theForce);
	}
	else
	{
		ticksWalked = 0;
		if (orientation.find("north-east") != -1)
			orientation = "south-east";
		else if (orientation.find("south-east") != -1)
			orientation = "south-west";
		else if (orientation.find("south-west") != -1)
			orientation = "north-west";
		else//if north-west
			orientation = "north-east";
	}
	++ticksWalked;
}

void SimpleMovement::linearHorizontal()
{
	b2Vec2 theForce;
	if (ticksWalked < maxWalkTicks)
	{
		if (orientation.find("east") != -1)
			theForce = b2Vec2(1, 0);
		else
			theForce = b2Vec2(-1, 0);
		_body->SetLinearVelocity(theForce);
	}
	else
	{
		ticksWalked = 0;
		if (orientation.find("east") != -1)
			orientation = "west";
		else
			orientation = "east";
	}
	++ticksWalked;
}

void SimpleMovement::linearVertical()
{
	b2Vec2 theForce;
	if (ticksWalked < maxWalkTicks)
	{
		if (orientation.find("north") != -1)
			theForce = b2Vec2(0, 1);
		else
			theForce = b2Vec2(0, -1);
		_body->SetLinearVelocity(theForce);
	}
	else
	{
		ticksWalked = 0;
		if (orientation.find("north") != -1)
			orientation = "south";
		else
			orientation = "north";
	}
	++ticksWalked;
}

void SimpleMovement::square()
{
	b2Vec2 theForce;
	if (ticksWalked < maxWalkTicks)
	{

		if (orientation.find("north") != -1)
			theForce = b2Vec2(0, -1);
		else if (orientation.find("south") != -1)
			theForce = b2Vec2(0, 1);
		else if (orientation.find("east") != -1)
			theForce = b2Vec2(1, 0);
		else//west
			theForce = b2Vec2(-1, 0);
		_body->SetLinearVelocity(theForce);
	}
	else
	{
		ticksWalked = 0;//Reset timer
		if (orientation.find("north") != -1)
			orientation = "east";
		else if (orientation.find("east") != -1)
			orientation = "south";
		else if (orientation.find("south") != -1)
			orientation = "west";
		else//west
			orientation = "north";
	}
	++ticksWalked;
}


