#include "LavaBurstRayCast.h"


LavaBurstRayCast::LavaBurstRayCast()
{
}


LavaBurstRayCast::~LavaBurstRayCast()
{
}

float LavaBurstRayCast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
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


	AnimatedSprite *s = static_cast<AnimatedSprite*>(userData);
	Player *p = dynamic_cast<Player*>(s);

	if (p != 0)
	{
		bodiesFound.push_back(body);
		return 1.0f;
	}
}
