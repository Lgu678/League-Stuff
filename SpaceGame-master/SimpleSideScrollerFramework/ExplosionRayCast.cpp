#include "ExplosionRayCast.h"


ExplosionRayCast::ExplosionRayCast()
{
}


ExplosionRayCast::~ExplosionRayCast()
{
}


float ExplosionRayCast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	b2Body *body = fixture->GetBody();
	void *userData = fixture->GetBody()->GetUserData();

	// IF THE BODY IS A WALL OR SOMETHING STATIC
	if (userData == NULL)
	{
		return -1.0;
	}

	//CHECK IF THE BODY ALREADY EXISTS IN LIST OF BODIES TO PROCESS
	if (find(bodiesFound.begin(), bodiesFound.end(), body) != bodiesFound.end())
	{
		// IT WAS FOUND AND WE DONT NEED TO DO ANYTHING
		return -1.0;
	}

	if (find(ignoreBodies.begin(), ignoreBodies.end(), body) != ignoreBodies.end())
	{
		// ITS IN THE IGNORE LIST DONT DO ANYTHING
		return -1.0;
	}
	
	
		bodiesFound.push_back(body);
		return 1.0f;
}


