#include "Andromalius.h"
#include "AndroMaliusBullet.h"
#include <cmath>
#include <math.h>


Andromalius::Andromalius()
{
	//movementPattern = new SimpleMovement(this->body, SimpleMovement::SQUARE);
	bossHealth = 600;
	moveTicks = 0;
	maxMoveTicks = 40;
	selectedGun = gunSelection::ANDROMALIUS_BULLET;
	fireCooldown = 30;
	bulletCooldown = 50;
	type = L"ANDROMALIUS_BOSS";
	lastOrientation = "right";
	bulletFire = -1;
	pivot = false;
	fired = false;
	dashCooldown = 15;
	srand(time(NULL));
	lightningCooldown = 3;
	lightningAngle = 0.0f;
}



Andromalius::~Andromalius()
{
	
}

void Andromalius::changeAnimationState()
{
	if (this->getBody()->GetLinearVelocity().x > 0)//East
	{
		if (this->isPlayerInRadius())
			this->setCurrentState(L"RIGHT_ATTACK");
		else
			this->setCurrentState(L"RIGHT");
	}
	else if (this->getBody()->GetLinearVelocity().x < 0)
	{
		if (this->isPlayerInRadius())
			this->setCurrentState(L"LEFT_ATTACK");
		else
			this->setCurrentState(L"LEFT");
	}
}

void Andromalius::nextMovement()
{

	if (this->isPlayerInRadius())
	{
		this->getBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->setCurrentState(L"DOWN");
		return;
	}
	

	moveTicks++;
	if (lastOrientation.compare("right") == 0)
		this->getBody()->SetLinearVelocity(b2Vec2(1, 0));//East
	else
		this->getBody()->SetLinearVelocity(b2Vec2(-1, 0));//West
	if (moveTicks > maxMoveTicks)
	{
		if (lastOrientation.compare("right") == 0)
			lastOrientation = "left";
		else
			lastOrientation = "right";
		moveTicks = 0;
	}


}


void Andromalius::think(Game *game)
{
	//this->setCurrentState(L"LEFT");
	
	int pX = this->getBody()->GetPosition().x;
	int pY = this->getBody()->GetPosition().y;
			nextMovement();
			changeAnimationState();
			
			if (this->isPlayerInRadius() || this->wasJustShot())
			{

				if (this->getBulletCooldown() <= 0)
				{
					this->setSelectedGun(ANDROMALIUS_BULLET);
					Bullet *b = game->getGSM()->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"ANDROMALIUS_BULLET");
					AndromaliusBullet *andromaliusBullet = dynamic_cast<AndromaliusBullet*>(b);
					andromaliusBullet->reset();
					andromaliusBullet->setDamageType('P');
					andromaliusBullet->setCurrentState(L"PRIMARY_FIRE");

					game->getGSM()->getPhyiscs()->activateEnemyBullet(andromaliusBullet, 0, -50);
					game->getGSM()->getSpriteManager()->addActiveBullet(andromaliusBullet);
					this->setBulletCooldown(20);
					bulletFire = 10;
					fired = true;
				}
				else
				{
					this->decBulletCooldown();
				}				

				//b2Vec2 vel = this->body->GetLinearVelocity();
			}
			else
			{
				//changeAnimationState();
			}

			//return;
	

}

void Andromalius::dash(Game *game)
{
	b2Body *botBody = this->getBody();
	b2Vec2 finalPos;
	bool validX = false;
	bool validY = false;
	while (!validX || !validY)
	{
		int r = (rand() % (2 - 0 + 1));
		int r2 = (rand() % (2 - 0 + 1));
		int dX = 1;
		int dY = 1;
		if (r < 1)
			dX = -1;
		else
			dX = 1;
		if (r2 < 1)
			dY = -1;
		else
			dY = 1;
		int maxV = rangeY;
		int maxH = rangeX;
		int distV = ((rand() % (maxV - 1))) + 1;
		int distH = ((rand() % (maxH - 1))) + 1;
		distV *= dY;
		distH *= dX;
		float xLowerBound = this->initPos.x - rangeX;
		float xUpperBound = this->initPos.x + rangeX;
		float yLowerBound = this->initPos.y - rangeY;
		float yUpperBound = this->initPos.y + rangeY;
		float curPosX = botBody->GetPosition().x;
		float curPosY = botBody->GetPosition().y;
		float testPosX = curPosX + distH;
		float testPosY = curPosY + distV;
		if (testPosX < xUpperBound && testPosX > xLowerBound)
			validX = true;
		if (testPosY < yUpperBound && testPosY > yLowerBound)
			validY = true;
		finalPos.x = testPosX;
		finalPos.y = testPosY;
	}
	botBody->SetTransform(finalPos, botBody->GetAngle());

	dashCooldown = 30;

}

Bot* Andromalius::clone(Game *game)
{
	Andromalius *newBot = new Andromalius();
	newBot->setHealth(this->getHealth());
	game->getGSM()->getPhyiscs()->initAndromaliusBoss(newBot, 40, 40, 50);
	//newBot->movementPattern->setBody(newBot->getBody());
	newBot->registerGame(this->game);
	newBot->setRangeX(this->rangeX);
	newBot->setRangeY(this->rangeY);
	return newBot;
}