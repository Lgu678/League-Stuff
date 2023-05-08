#include "Bullet.h"


Bullet::Bullet()
{
	speed = 0.0f;
	primaryDamage = 0;
	damageType = 'P';//Primary
}


Bullet::~Bullet()
{
}


void Bullet::update()
{
	AnimatedSprite::updateSprite();

}

/*void Bullet::handleCollision()
{
	initCollide = true;
}

void Bullet::clone(Game *game)
{
	
}*/