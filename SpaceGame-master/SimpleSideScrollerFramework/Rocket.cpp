#include "Rocket.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "ExplosionEffect.h"
#include "Effect.h"
#include "EffectRecycler.h"

Rocket::Rocket()
{
	speed = 8;
	initCollide = false;
	type = L"ROCKET";
	primaryDamage = 50;
}


Rocket::~Rocket()
{
}

void Rocket::handleCollision(Game *game)
{
	// GET ALL BOTS IN RADIUS AROUND ROCKET

	float angle = 0.0f;
	initCollide = true;

	ExplosionRayCast cast;
	cast.setIgnoreBody(this->getBody());
	cast.setIgnoreBody(game->getGSM()->getPhyiscs()->getPlayerBody());

	float x = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x;
	float y = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y;
	
	float pixelScaling = game->getGSM()->getPhyiscs()->getPixelScaling();
	float32 L = 100 * pixelScaling;
	b2Vec2 point1(this->getBody()->GetPosition().x, this->getBody()->GetPosition().y);
	

	for (int i = 0; i < 180; i++)
	{
		b2Vec2 d(L * cosf(angle), L * sinf(angle));
		b2Vec2 point2 = point1 + d;
		game->getGSM()->getPhyiscs()->getGameWorld()->RayCast(&cast, point1, point2);
		angle += 2.0f * b2_pi / 180.0f;
	}

	vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
	while (bodyIT != cast.getBodiesFoundEnd())
	{
		b2Body *body = *(bodyIT);
		Bot *bot = static_cast<Bot*>(body->GetUserData());
		bot->setJustShot(true);
		bot->setHealth(bot->getHealth() - this->splashDmg);
		bodyIT++;
	}
	game->getAudio()->playSound(L"data\\sounds\\rocket_explosion.wav",false);

	//THIS IS CURRENTLY NOT GOOD CODE BECAUSE WE SHOULD NOT BE CONSTANTLY CONSTRUCTING
	//A RECYCLER WILL EVENTUALLY BE MADE FOR EFFECTS MOST LIKELY
	Effect *explosionSprite = game->getGSM()->getSpriteManager()->getEffectRecycler()->retrieveEffect(game, L"EXPLOSION");
	
	explosionSprite->setDefaultX(((this->getBody()->GetPosition().x) / pixelScaling)-20);
	explosionSprite->setDefaultY(((this->getBody()->GetPosition().y) / pixelScaling)-20);
	game->getGSM()->getSpriteManager()->addEffect(explosionSprite);


}

Bullet* Rocket::clone(Game *game)
{
	Rocket *clone = new Rocket();
	game->getGSM()->getPhyiscs()->initBullet(clone);
	clone->setSpriteType(this->getSpriteType());
	clone->setAlpha(255);
	return clone;
}