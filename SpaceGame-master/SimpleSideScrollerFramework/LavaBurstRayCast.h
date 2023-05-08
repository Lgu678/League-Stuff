#pragma once
#include <Box2D\Box2D.h>
#include "stdafx.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "Player.h"

class LavaBurstRayCast : public b2RayCastCallback
{

private:

	vector <b2Body*> bodiesFound;
	vector <b2Body*> ignoreBodies;

public:
	LavaBurstRayCast();
	~LavaBurstRayCast();

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);
	vector <b2Body*>::iterator  getBodiesFoundBegin(){ return bodiesFound.begin(); }
	vector <b2Body*>::iterator  getBodiesFoundEnd(){ return bodiesFound.end(); }


	void setIgnoreBody(b2Body *ib)
	{
		ignoreBodies.push_back(ib);
	}
};

