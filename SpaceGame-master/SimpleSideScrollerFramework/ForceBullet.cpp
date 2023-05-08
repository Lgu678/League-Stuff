#include "ForceBullet.h"
#include "src\sssf\gsm\state\GameStateManager.h"

ForceBullet::ForceBullet()
{
	speed = 10;
	initCollide = false;
	type = L"FORCE_BULLET";
	primaryDamage = 4;
}


ForceBullet::~ForceBullet()
{
}

void ForceBullet::handleCollision(Game *game)
{
	initCollide = true;
}

void ForceBullet::handleCollision(Game *game, AnimatedSprite* a)
{

	b2Vec2 v = this->getBody()->GetLinearVelocity();

	float xDir = v.x / abs(v.x);
	float yDir = v.y / abs(v.y);
	
	//a->getBody()->ApplyForceToCenter(b2Vec2(this->getBody()->GetLinearVelocity().x, this->getBody()->GetLinearVelocity().y), true);
	a->getBody()->SetLinearVelocity(b2Vec2(0, 0));
	//a->getBody()->ApplyLinearImpulse(b2Vec2(this->getBody()->GetLinearVelocity().x + 150.0f, this->getBody()->GetLinearVelocity().y + 150.0f), b2Vec2(0, 0), true);
	a->getBody()->ApplyLinearImpulse(b2Vec2(150.0f * xDir, 150.0f * yDir), a->getBody()->GetWorldCenter(), true);
	//a->getBody()->SetLinearVelocity(b2Vec2(0, 0));
	initCollide = true;
}

Bullet*	ForceBullet::clone(Game *game)
{
	ForceBullet *clone = new ForceBullet();
	game->getGSM()->getPhyiscs()->initBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}
