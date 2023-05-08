#include "physicsManager.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/Viewport.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/game/Game.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/world/World.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/world/WorldLayer.h"
#include"../SimpleSideScrollerFramework/src/sssf/gsm/sprite/SpriteManager.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "stdafx.h"

#define DEGTORAD 0.0174532925199432957f

#include <stdio.h>

physicsManager::physicsManager()
{
	initLevelSelectWorld();
	initGameWorld();
	playerBody = NULL;
}


physicsManager::~physicsManager()
{
	delete levelSelectWorld;
	delete gameWorld;
}

void physicsManager::testResetGameWorld()
{
	delete gameWorld;
	delete listener;
	initGameWorld();

}

void physicsManager::initGameWorld()
{
	b2Vec2 gravity(0, 0);
	gameWorld = new b2World(gravity);
	listener = new PhysicsCollisionListener();
	gameWorld->SetContactListener(listener);
	timeStep = 1 / 33.0;
	velocityIterations = 8;
	positionIterations = 3;
	pixelScaling = 0.009765625;
	viewportConverter = 3.102;
	lavaBossHealth = true;
	lavaBossFight = false;
}

void physicsManager::deleteSpriteBody(AnimatedSprite *sprite)
{
	gameWorld->DestroyBody(sprite->getBody());
	sprite->setBody(NULL);
}

void physicsManager::clearGameWorld()
{
	gameWorld->ClearForces();

	b2Body* body = gameWorld->GetBodyList();

	int counter = 0;

	while (body)

	{

		b2Body* b = body;

		body = body->GetNext();



		//GameActor* myActor = (GameActor*)b->GetUserData();

		//if (myActor->IsDead())

		//{

		gameWorld->DestroyBody(b);
		counter++;

		//}
	}

	int c = counter;

}

void physicsManager::registerGameToListener()
{
	listener->setGame(game);
}

void physicsManager::initLevelSelectWorld()
{
	b2Vec2 gravity(0, 0);
	levelSelectWorld = new b2World(gravity);
}

void physicsManager::setMouseVec(float x, float y)
{
	x = x * pixelScaling;
	y = y * pixelScaling;


	clickedPoint.x = x;
	clickedPoint.y = y;

}

void physicsManager::setMouseLoc(float x, float y)
{
	x = x * pixelScaling;
	y = y * pixelScaling;


	mouseLoc.x = x;
	mouseLoc.y = y;
}

void physicsManager::initSpriteBodyForLevelSelect(AnimatedSprite *sprite, float x, float y)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = sprite;
	myBodyDef.type = b2_dynamicBody;
	//myBodyDef.linearDamping = 1.0f;
	//myBodyDef.linearVelocity.x = -1.0f;
	//myBodyDef.linearVelocity.y = 1.0f;

	b2PolygonShape shape;
	shape.SetAsBox(1, 1);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;


	myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	sprite->setBody(levelSelectWorld->CreateBody(&myBodyDef));
	sprite->getBody()->ApplyForce(b2Vec2(-420, 405), sprite->getBody()->GetWorldCenter(), true);
	sprite->getBody()->CreateFixture(&fixtureDef);
	sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);

}

void physicsManager::initRangeBot(Bot *bot, int bX, int bY, int aR)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bot;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.active = false;



	b2PolygonShape shape;
	shape.SetAsBox((bX * pixelScaling) / 2, (bY * pixelScaling) / 2);


	b2FixtureDef detectFixture;
	b2CircleShape detector;
	detector.m_radius = 1000 * pixelScaling;
	detectFixture.shape = &detector;
	detectFixture.isSensor = true;
	detectFixture.filter.categoryBits = collisionCatagory::BOT_DETECTOR;
	uint16 mask1 = collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET;
	detectFixture.filter.maskBits = mask1;


	b2FixtureDef damageFixture;
	b2CircleShape damageDetector;
	damageDetector.m_radius = aR * pixelScaling;
	damageFixture.shape = &damageDetector;
	damageFixture.isSensor = true;
	damageFixture.filter.categoryBits = collisionCatagory::BOT_RANGEATTACK_RADIUS;
	damageFixture.filter.maskBits = mask1;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	//	fixtureDef.isSensor = false;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY;
	uint16 mask = collisionCatagory::WALL | collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET | collisionCatagory::ENEMY;
	fixtureDef.filter.maskBits = mask;

	myBodyDef.position.Set(0, 0);


	bot->setBody(gameWorld->CreateBody(&myBodyDef));
	bot->getBody()->CreateFixture(&fixtureDef);
	bot->getBody()->CreateFixture(&detectFixture);
	bot->getBody()->CreateFixture(&damageFixture);
}


void physicsManager::activateBot(Bot *bot, float x, float y)
{

	b2Vec2 pos;
	pos.x = x * pixelScaling;
	pos.y = y * pixelScaling;

	bot->getBody()->SetTransform(pos, 0.0);

 	if (bot->getType() == L"MAGE_BOSS")
	{
		MageBoss *b = dynamic_cast<MageBoss*>(bot);
		b->setInitPos(pos.x, pos.y);
	}


	bot->getBody()->SetActive(true);
}

void physicsManager::initMageOrb(AnimatedSprite *orb, float x, float y, Bot *m)
{
	MageBoss *b = dynamic_cast<MageBoss*>(m);
	if (b != 0)
	{
		b2Vec2 pos = m->getBody()->GetPosition();

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;

		

		b2CircleShape c;
		c.m_radius = .205f;
		bd.position.Set(x, y);

		b2FixtureDef fd;
		fd.friction = 0;
		fd.restitution = 0;
		fd.density = 5.0f;
		fd.shape = &c;
		fd.filter.categoryBits = collisionCatagory::ENEMY;
		fd.filter.maskBits = collisionCatagory::PLAYER_BULLET | collisionCatagory::PLAYER;



		b2Body *cb = gameWorld->CreateBody(&bd);
		cb->CreateFixture(&fd);

		orb->setBody(cb);

		cb->SetActive(false);

		b2RopeJointDef m_ropeDef;
		b2Joint* m_rope;

		m_ropeDef.bodyA = m->getBody();
		m_ropeDef.bodyB = cb;
		m_ropeDef.collideConnected = false;
		m_ropeDef.maxLength = .4394;
		m_ropeDef.localAnchorA.SetZero();
		m_ropeDef.localAnchorB.SetZero();

		m_rope = gameWorld->CreateJoint(&m_ropeDef);

	}
}

void physicsManager::activateMageOrb(AnimatedSprite *orb, b2Vec2 push)
{
	orb->getBody()->SetActive(true);
	orb->getBody()->ApplyLinearImpulse(push, orb->getBody()->GetWorldCenter(), true);

}

void physicsManager::deactivateMageOrb(AnimatedSprite *orb)
{
	b2Vec2 zero;
	zero.x = 0;
	zero.y = 0;
	orb->getBody()->SetLinearVelocity(zero);
	orb->getBody()->SetActive(false);
}

void physicsManager::initMageBoss(Bot *bot, int bX, int bY, int aR)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bot;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.active = false;



	b2PolygonShape shape;
	shape.SetAsBox((bX * pixelScaling) / 2, (bY * pixelScaling) / 2);

	b2FixtureDef dodgeBulletFixture;
	b2CircleShape dodgeBullet;
	dodgeBullet.m_radius = 300 * pixelScaling;
	dodgeBulletFixture.shape = &dodgeBullet;
	dodgeBulletFixture.isSensor = true;
	dodgeBulletFixture.filter.categoryBits = collisionCatagory::BOT_DODGE;
	uint16 mask2 = collisionCatagory::PLAYER_BULLET;
	dodgeBulletFixture.filter.maskBits = mask2;

	b2FixtureDef detectFixture;
	b2CircleShape detector;
	detector.m_radius = 100 * pixelScaling;
	detectFixture.shape = &detector;
	detectFixture.isSensor = true;
	detectFixture.filter.categoryBits = collisionCatagory::BOT_DETECTOR;
	uint16 mask1 = collisionCatagory::PLAYER;
	detectFixture.filter.maskBits = mask1;


	b2FixtureDef damageFixture;
	b2CircleShape damageDetector;
	damageDetector.m_radius = aR * pixelScaling;
	damageFixture.shape = &damageDetector;
	damageFixture.isSensor = true;
	damageFixture.filter.categoryBits = collisionCatagory::BOT_DAMAGE_RADIUS;
	damageFixture.filter.maskBits = mask1;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1000.0f;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY;
	uint16 mask = collisionCatagory::WALL | collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET;
	fixtureDef.filter.maskBits = mask;

	myBodyDef.position.Set(0, 0);


	bot->setBody(gameWorld->CreateBody(&myBodyDef));
	bot->getBody()->CreateFixture(&fixtureDef);
	bot->getBody()->CreateFixture(&detectFixture);
	bot->getBody()->CreateFixture(&damageFixture);
	bot->getBody()->CreateFixture(&dodgeBulletFixture);

}
void physicsManager::initLavaBoss(Bot *bot, int bX, int bY, int aR)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bot;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.active = false;



	b2PolygonShape shape;
	shape.SetAsBox((bX * pixelScaling) / 2, (bY * pixelScaling) / 2);

	b2FixtureDef detectFixture;
	b2CircleShape detector;
	detector.m_radius = 100 * pixelScaling;
	detectFixture.shape = &detector;
	detectFixture.isSensor = true;
	detectFixture.filter.categoryBits = collisionCatagory::BOT_DETECTOR;
	uint16 mask1 = collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET;
	detectFixture.filter.maskBits = mask1;


	b2FixtureDef damageFixture;
	b2CircleShape damageDetector;
	damageDetector.m_radius = aR * pixelScaling;
	damageFixture.shape = &damageDetector;
	damageFixture.isSensor = true;
	damageFixture.filter.categoryBits = collisionCatagory::BOT_DAMAGE_RADIUS;
	damageFixture.filter.maskBits = mask1;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 10000000000.0f;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY;
	uint16 mask = collisionCatagory::WALL | collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET | collisionCatagory::ENEMY;
	fixtureDef.filter.maskBits = mask;

	myBodyDef.position.Set(0, 0);


	bot->setBody(gameWorld->CreateBody(&myBodyDef));
	bot->getBody()->CreateFixture(&fixtureDef);
	bot->getBody()->CreateFixture(&detectFixture);
	bot->getBody()->CreateFixture(&damageFixture);


}


void physicsManager::initAndromaliusBoss(Bot *bot, int bX, int bY, int aR)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bot;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.active = false;



	b2PolygonShape shape;
	shape.SetAsBox((bX * pixelScaling) / 2, (bY * pixelScaling) / 2);

	b2FixtureDef detectFixture;
	b2CircleShape detector;
	detector.m_radius = 100 * pixelScaling;
	detectFixture.shape = &detector;
	detectFixture.isSensor = true;
	detectFixture.filter.categoryBits = collisionCatagory::BOT_DETECTOR;
	uint16 mask1 = collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET;
	detectFixture.filter.maskBits = mask1;


	b2FixtureDef damageFixture;
	b2CircleShape damageDetector;
	damageDetector.m_radius = aR * pixelScaling;
	damageFixture.shape = &damageDetector;
	damageFixture.isSensor = true;
	damageFixture.filter.categoryBits = collisionCatagory::BOT_DAMAGE_RADIUS;
	damageFixture.filter.maskBits = mask1;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 10000000000.0f;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY;
	uint16 mask = collisionCatagory::WALL | collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET | collisionCatagory::ENEMY;
	fixtureDef.filter.maskBits = mask;

	myBodyDef.position.Set(0, 0);


	bot->setBody(gameWorld->CreateBody(&myBodyDef));
	bot->getBody()->CreateFixture(&fixtureDef);
	bot->getBody()->CreateFixture(&detectFixture);
	bot->getBody()->CreateFixture(&damageFixture);


}


void physicsManager::initMeleeBot(Bot *bot, int bX, int bY, int aR)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bot;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.active = false;



	b2PolygonShape shape;
	shape.SetAsBox((bX * pixelScaling) / 2, (bY * pixelScaling) / 2);

	b2FixtureDef detectFixture;
	b2CircleShape detector;
	detector.m_radius = 100 * pixelScaling;
	detectFixture.shape = &detector;
	detectFixture.isSensor = true;
	detectFixture.filter.categoryBits = collisionCatagory::BOT_DETECTOR;
	uint16 mask1 = collisionCatagory::PLAYER;
	detectFixture.filter.maskBits = mask1;


	b2FixtureDef damageFixture;
	b2CircleShape damageDetector;
	damageDetector.m_radius = aR * pixelScaling;
	damageFixture.shape = &damageDetector;
	damageFixture.isSensor = true;
	damageFixture.filter.categoryBits = collisionCatagory::BOT_DAMAGE_RADIUS;
	damageFixture.filter.maskBits = mask1;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY;
	uint16 mask = collisionCatagory::WALL | collisionCatagory::PLAYER | collisionCatagory::PLAYER_BULLET | collisionCatagory::ENEMY;
	fixtureDef.filter.maskBits = mask;

	myBodyDef.position.Set(0, 0);


	bot->setBody(gameWorld->CreateBody(&myBodyDef));
	bot->getBody()->CreateFixture(&fixtureDef);
	bot->getBody()->CreateFixture(&detectFixture);
	bot->getBody()->CreateFixture(&damageFixture);


}

void physicsManager::deactivateBot(Bot *bot)
{
	b2Vec2 reset;
	reset.x = 0.0f;
	reset.y = 0.0f;
	bot->getBody()->SetLinearVelocity(reset);
	bot->getBody()->SetActive(false);
}

void physicsManager::initPlayer(Player *player, float x, float y)
{
	playerMoveLeft = false;
	playerMoveRight = false;
	playerMoveUp = false;
	playerMoveDown = false;

	b2BodyDef myBodyDef;
	myBodyDef.userData = player;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;
	myBodyDef.bullet = true;

	b2PolygonShape shape;
	shape.SetAsBox((32 * pixelScaling) / 2, (32 * pixelScaling) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;

	fixtureDef.filter.categoryBits = collisionCatagory::PLAYER;

	uint16 mask = collisionCatagory::ENEMY | collisionCatagory::WALL | collisionCatagory::ENEMY_BULLET | collisionCatagory::BOT_DAMAGE_RADIUS | collisionCatagory::BOT_DETECTOR;

	fixtureDef.filter.maskBits = mask;

	fixtureDef.friction = 0.0f;

	fixtureDef.restitution = 0.0f;

	myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	player->setBody(gameWorld->CreateBody(&myBodyDef));
	//player->getBody()->ApplyForce(b2Vec2(-420, 405), sprite->getBody()->GetWorldCenter(), true);
	player->getBody()->CreateFixture(&fixtureDef);


	playerBody = player->getBody();
	//sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);

}

void physicsManager::levelSelectWorldStep()
{
	levelSelectWorld->Step(timeStep, velocityIterations, positionIterations);
	levelSelectWorld->ClearForces();

	b2Body *node = levelSelectWorld->GetBodyList();
	while (node)
	{
		b2Body *b = node;
		node = node->GetNext();
		// WE CAN DO STUFF TO THE BODY HERE

		//float x = b->GetLinearVelocity().x;
		//float y = b->GetLinearVelocity().y;



	}
}

void physicsManager::movePlayerLeft()
{
	playerMoveLeft = true;
}

void physicsManager::movePlayerRight()
{
	playerMoveRight = true;
}

void physicsManager::movePlayerUp()
{
	playerMoveUp = true;
}

void physicsManager::movePlayerDown()
{
	playerMoveDown = true;
}

void physicsManager::viewportM()
{
	viewportMoved = true;
}
void physicsManager::gameWorldStep()
{


	b2Vec2 playerV = playerBody->GetLinearVelocity();
	Viewport *viewport = game->getGUI()->getViewport();
	AnimatedSprite *a = static_cast<AnimatedSprite*>(playerBody->GetUserData());
	Player *p = dynamic_cast<Player*>(a);

	playerV.x = 0;
	playerV.y = 0;

	if (playerMoveLeft)
	{
		playerV.x = -2;
	}
	if (playerMoveRight)
	{
		playerV.x = 2;
	}
	if (playerMoveUp)
	{
		playerV.y = 2;
	}
	if (playerMoveDown)
	{
		playerV.y = -2;
	}



	//if (p->isCollidingWithBot())
	//	playerV = playerBody->GetLinearVelocity();

	if (playerMoveLeft && playerMoveRight)
	{
		playerV.x = 0;
	}
	if (playerMoveUp && playerMoveDown)
	{
		playerV.y = 0;
	}

	//playerBody->GetLinearVelocity().x
	//playerBody->SetLinearVelocity(playerV);

	b2Vec2 playerPos = game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetPosition();
	//b2Vec2 playerPos = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition();
	int playerX = (playerPos.x / pixelScaling) - viewport->getViewportX();
	int playerY = (playerPos.y / pixelScaling) - viewport->getViewportY();
	int realX = (playerPos.x / pixelScaling);
	int realY = (playerPos.y / pixelScaling);
	boolean outsideX;
	boolean outsideY;

	if (lavaBossHealth == true)
	{
		if ((realX >= 1440 && realX <= 1630) && (realY >= 2912 && realY <= 3168 && lavaBossFight == false))
		{
			playerBody->SetLinearVelocity(playerV);
			list<Bot*>::iterator active = game->getGSM()->getSpriteManager()->getBotsIterator();
			if (viewport->getScrollSpeedX() > 0)
			{
				while (active != game->getGSM()->getSpriteManager()->getEndOfBotsIterator())
				{
					Bot *b = (*active);
					if (b->getType() == L"LAVA_BOSS")
					{
						b->setCurrentState(L"RISE");
						if (b->getFrameIndex() == 14)
						{
							viewport->setScrollSpeedX(0);
							viewport->moveViewport(0, 0, 3200, 3200);
							lavaBossFight = true;
						}
						else
						{
							viewport->setScrollSpeedX(viewportConverter * -1);
							viewport->moveViewport(viewportConverter * -1, 0, 3200, 3200);
						}
					}
					active++;
				}
				return;
			}
			else if (viewport->getScrollSpeedX() < 0)
			{
				while (active != game->getGSM()->getSpriteManager()->getEndOfBotsIterator())
				{
					Bot *b = (*active);
					if (b->getType() == L"LAVA_BOSS")
					{
						b->setCurrentState(L"RISE");
						if (b->getFrameIndex() == 14)
						{
							viewport->setScrollSpeedX(0);
							viewport->moveViewport(0, 0, 3200, 3200);
							lavaBossFight = true;
						}
						else
						{
							viewport->setScrollSpeedX(viewportConverter * 1);
							viewport->moveViewport(viewportConverter * 1, 0, 3200, 3200);
						}
					}
					active++;
				}
				return;
			}
		}
		else if (lavaBossFight == true)
		{
			playerBody->SetLinearVelocity(playerV);
			gameWorld->Step(timeStep, velocityIterations, positionIterations);
			gameWorld->ClearForces();
			playerMoveLeft = false;
			playerMoveRight = false;
			playerMoveUp = false;
			playerMoveDown = false;
			viewportMoved = false;
			mouseClicked = false;

			list<Bot*>::iterator active = game->getGSM()->getSpriteManager()->getBotsIterator();
			while (active != game->getGSM()->getSpriteManager()->getEndOfBotsIterator())
			{
				Bot *b = (*active);
				if (b->getType() == L"LAVA_BOSS")
				{
					if (b->getCurrentState() == L"DEAD")
					{
						viewport->setScrollSpeedX(0);
						viewport->moveViewport(0, 0, 3200, 3200);
						lavaBossFight = false;
					}
				}
				active++;
			}

			return;
		}
	}
	if ((viewport->getViewportX() <= 0.0f || viewport->getViewportX() >= 3200))
		outsideX = false;
	else if (playerX >= 492 && playerX <= 532)
		outsideX = false;
	else
		outsideX = true;

	if ((viewport->getViewportY() <= 0.0f || viewport->getViewportY() >= 3200))
		outsideY = false;
	else if (playerY >= 492 && playerY <= 532)
		outsideY = false;
	else
		outsideY = true;

	if (viewport->getViewportX() <= 0.0f || viewport->getViewportX() >= 3200)
	{
		if (playerX >= 507 && playerX <= 512)
		{
			if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().x != 0)
			{
				viewport->setScrollSpeedX(viewportConverter * playerBody->GetLinearVelocity().x);
				viewportMoved = true;
			}
		}
	}

	else if (playerX >= 472 && playerX <= 552)
	{
		if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().x != 0)
		{
			viewport->setScrollSpeedX(viewportConverter * playerBody->GetLinearVelocity().x);
			viewportMoved = true;
		}

	}
	else
	{
		viewport->setScrollSpeedX(0.0f);
	}

	if (viewport->getViewportY() <= 0.0f || viewport->getViewportY() >= 3200)
	{
		if (playerY >= 379 && playerY <= 389)
		{
			if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().y != 0)
			{
				viewport->setScrollSpeedY(viewportConverter * playerBody->GetLinearVelocity().y);
				viewportMoved = true;
			}
		}
	}
	else if (playerY >= 344 && playerY <= 424)
	{
		if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().y != 0)
		{
			viewport->setScrollSpeedY(viewportConverter * playerBody->GetLinearVelocity().y);
			viewportMoved = true;
		}
	}
	else
	{
		viewport->setScrollSpeedY(0.0f);
	}

	b2Vec2 rot;
	rot.x = (mouseLoc.x - playerBody->GetPosition().x);
	rot.y = (mouseLoc.y - playerBody->GetPosition().y);
	float angle = atan2f(rot.y, rot.x);


	playerBody->SetLinearVelocity(playerV);
	if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().x == 0)
		viewport->setScrollSpeedX(0);
	if (game->getGSM()->getPhyiscs()->getPlayerBody()->GetLinearVelocity().y == 0)
		viewport->setScrollSpeedY(0);

	if (viewportMoved == true)
	{
		viewport->moveViewport(viewport->getScrollSpeedX(), viewport->getScrollSpeedY(), 3200, 3200);
	}
	else if (outsideX || outsideY)
	{
		if (playerX < 510)
			viewport->setScrollSpeedX(viewportConverter * -2);
		else if (playerX > 520)
			viewport->setScrollSpeedX(viewportConverter * 2);
		else
			viewport->setScrollSpeedX(0.0f);

		if (playerY < 379)
			viewport->setScrollSpeedY(viewportConverter * -2);
		else if (playerY > 389)
			viewport->setScrollSpeedY(viewportConverter * 2);
		else
		{
			viewport->setScrollSpeedY(0.0f);
		}
		viewport->moveViewport(viewport->getScrollSpeedX(), viewport->getScrollSpeedY(), 3200, 3200);
	}

	

	/*if (playerBody->GetAngularVelocity() != 0)
		playerBody->SetAngularVelocity(playerBody->GetAngularVelocity()/2);*/
	gameWorld->Step(timeStep, velocityIterations, positionIterations);
	gameWorld->ClearForces();
	playerMoveLeft = false;
	playerMoveRight = false;
	playerMoveUp = false;
	playerMoveDown = false;
	viewportMoved = false;
	mouseClicked = false;
}

void physicsManager::initBullet(Bullet *bullet)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bullet;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;

	myBodyDef.bullet = true;
	myBodyDef.active = false;


	b2PolygonShape shape;
	if (bullet->getType().compare(L"FORCE_BULLET") == 0)
		shape.SetAsBox((10 * pixelScaling) / 2, (20 * pixelScaling) / 2);
	else
		shape.SetAsBox((10 * pixelScaling) / 2, (10 * pixelScaling) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	
	if(bullet->getType().compare(L"FORCE_BULLET") == 0)
		fixtureDef.density = 10.0f;
	else
		fixtureDef.density = 0.001f;

	fixtureDef.isSensor = false;
	fixtureDef.filter.categoryBits = collisionCatagory::PLAYER_BULLET;

	uint16 mask = collisionCatagory::ENEMY | collisionCatagory::WALL | collisionCatagory::BOT_DODGE;

	fixtureDef.filter.maskBits = mask;

	//myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	bullet->setBody(gameWorld->CreateBody(&myBodyDef));
	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;
	bullet->getBody()->SetLinearVelocity(vel);
	bullet->getBody()->CreateFixture(&fixtureDef);


	//sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);
}


void physicsManager::initLavaBullet(Bullet *bullet)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bullet;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;

	myBodyDef.bullet = true;
	myBodyDef.active = false;


	b2PolygonShape shape;
	shape.SetAsBox((75 * pixelScaling) / 2, (75 * pixelScaling) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1000000.0f;
	fixtureDef.isSensor = false;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY_BULLET;

	uint16 mask = collisionCatagory::PLAYER | collisionCatagory::WALL;

	fixtureDef.filter.maskBits = mask;

	//myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	bullet->setBody(gameWorld->CreateBody(&myBodyDef));
	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;
	bullet->getBody()->SetLinearVelocity(vel);
	bullet->getBody()->CreateFixture(&fixtureDef);


	//sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);
}

void physicsManager::initLavaBurstBullet(Bullet *bullet)
{
	/// static: zero mass, zero velocity, may be manually moved
	b2BodyDef myBodyDef;
	myBodyDef.userData = bullet;
	myBodyDef.type = b2_staticBody;
	myBodyDef.fixedRotation = true;

	myBodyDef.bullet = true;
	myBodyDef.active = false;


	b2PolygonShape shape;
	shape.SetAsBox(pixelScaling, pixelScaling);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.001f;
	fixtureDef.isSensor = false;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY_BULLET;

	uint16 mask = collisionCatagory::PLAYER | collisionCatagory::WALL;

	fixtureDef.filter.maskBits = mask;

	//myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	bullet->setBody(gameWorld->CreateBody(&myBodyDef));
	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;
	bullet->getBody()->SetLinearVelocity(vel);
	bullet->getBody()->CreateFixture(&fixtureDef);


	//sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);
}

void physicsManager::initEnemyBullet(Bullet *bullet)
{
	b2BodyDef myBodyDef;
	myBodyDef.userData = bullet;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.fixedRotation = true;

	myBodyDef.bullet = true;
	myBodyDef.active = false;


	b2PolygonShape shape;
	shape.SetAsBox((10 * pixelScaling) / 2, (10 * pixelScaling) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.001f;
	fixtureDef.isSensor = false;
	fixtureDef.filter.categoryBits = collisionCatagory::ENEMY_BULLET;

	uint16 mask = collisionCatagory::PLAYER | collisionCatagory::WALL;

	fixtureDef.filter.maskBits = mask;

	//myBodyDef.position.Set(x*pixelScaling, y*pixelScaling);

	bullet->setBody(gameWorld->CreateBody(&myBodyDef));
	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;
	bullet->getBody()->SetLinearVelocity(vel);
	bullet->getBody()->CreateFixture(&fixtureDef);


	//sprite->getBody()->SetTransform(sprite->getBody()->GetPosition(), 220 * DEGTORAD);
}

void physicsManager::activateEnemyBullet(Bullet *bullet, float x, float y)
{
	bullet->getBody()->SetActive(true);



	b2Vec2 posBoss;
	posBoss.x = x;
	posBoss.y = y;

	b2Vec2 posPlayer;
	posPlayer.x = (playerBody->GetPosition().x);
	posPlayer.y = (playerBody->GetPosition().y);

	bullet->getBody()->SetTransform(posBoss, 0.0);

	if (bullet->getType() == L"MAGE_BULLET")
	{
		b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
		vel.x = 0.0;
		vel.y = 0.0;
		//float angle = atan2f(vel.y, vel.x);
		bullet->getBody()->SetLinearVelocity(vel);

		b2Vec2 newPos;
		newPos.x = (playerBody->GetPosition().x) + (x * pixelScaling);
		newPos.y = (playerBody->GetPosition().y) + (y * pixelScaling);

		bullet->getBody()->SetTransform(newPos, bullet->getBody()->GetAngle());

		float dX = posPlayer.x - bullet->getBody()->GetPosition().x;
		float dY = posPlayer.y - bullet->getBody()->GetPosition().y;
	}
	else if (bullet->getType() == L"LAVA_BURST")
	{
		b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
		vel.x = 0;
		vel.y = 0;
		float angle = atan2f(vel.y, vel.x);
		bullet->getBody()->SetLinearVelocity(vel);

		bullet->getBody()->SetTransform(playerBody->GetPosition(), angle);
		//bullet->getBody()->SetTransform(bullet->getBody()->GetPosition(), angle);
	}
	else
	{
		b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
		vel.x = posPlayer.x - posBoss.x;
		vel.y = posPlayer.y - posBoss.y;
		float angle = atan2f(vel.y, vel.x);
		vel.Normalize();
		vel *= bullet->getSpeed();
		bullet->getBody()->SetLinearVelocity(vel);

		bullet->getBody()->SetTransform(bullet->getBody()->GetPosition(), angle);
	}
}

void physicsManager::activateMageLightning(Bullet *bullet, Bot *m, float angle)
{
	bullet->getBody()->SetActive(true);
	bullet->getBody()->SetTransform(m->getBody()->GetPosition(), 0.0f);

	b2Vec2 calc;
	calc.x = (cos(angle));
	calc.y = (sin(angle));
	calc.Normalize();

	if (angle > 90 && angle < 180)
	{
		calc.x *= -1.0f;
	}

	if (angle > 180 && angle < 270)
	{
		calc.x *= -1.0f;
		calc.y *= -1.0f;
	}

	if (angle > 270 && angle < 360)
	{
		calc.y *= -1.0f;
	}

	if (angle == 180)
	{
		calc.x *= -1.0f;
	}

	if (angle == 270)
	{
		calc.y *= -1.0f;
	}

	calc *= bullet->getSpeed();
	float angle2 = atan2f(calc.y, calc.x);
	bullet->getBody()->SetLinearVelocity(calc);
	angle = angle * DEGTORAD;
	bullet->getBody()->SetTransform(bullet->getBody()->GetPosition(), angle2);

}

void physicsManager::activateBullet(Bullet *bullet)
{

	bullet->getBody()->SetActive(true);

	// fix y
	float xOffset = 1;
	float yOffset = 1;

	if (clickedPoint.x < 0)
		xOffset = xOffset*-1.0;

	if (clickedPoint.y < 0)
		yOffset = yOffset*-1.0;

	b2Vec2 pos;
	
	if (bullet->getType().compare(L"FORCE_BULLET") == 0)//(-0.6f, -0.6f
	{
		pos.x = playerBody->GetPosition().x * xOffset;
		pos.x -= 0.6f;
	}
	else
		pos.x = playerBody->GetPosition().x * xOffset;
	if (bullet->getType().compare(L"FORCE_BULLET") == 0)
	{
		pos.y = playerBody->GetPosition().y * yOffset;
		pos.y -= 0.6f;
	}
	else
		pos.y = playerBody->GetPosition().y * yOffset;

	bullet->getBody()->SetTransform(pos, 0.0);

	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = (clickedPoint.x - playerBody->GetPosition().x);
	vel.y = (clickedPoint.y - playerBody->GetPosition().y);
	float angle = atan2f(vel.y, vel.x);
	vel.Normalize();
	vel *= bullet->getSpeed();
	bullet->getBody()->SetLinearVelocity(vel);


	bullet->getBody()->SetTransform(bullet->getBody()->GetPosition(), angle);



}

void physicsManager::activateBulletSpread(Bullet *b1, Bullet *b2, Bullet *b3)
{
	//B2 IS THE MIDDLE BULLET IN THE SPREAD THAT FIRES NORMALLY, 1 AND 3 GO SLIGHTLY TO LEFT AND RIGHT OF B2 RESPECTIVLY
	b2->getBody()->SetActive(true);
	b1->getBody()->SetActive(true);
	b3->getBody()->SetActive(true);

	//INIT B2 NORMALLY
	float xOffset = 1;
	float yOffset = 1;

	if (clickedPoint.x < 0)
		xOffset = xOffset*-1.0;

	if (clickedPoint.y < 0)
		yOffset = yOffset*-1.0;

	b2Vec2 pos;
	pos.x = playerBody->GetPosition().x * xOffset;
	pos.y = playerBody->GetPosition().y * yOffset;

	b1->getBody()->SetTransform(pos, 0.0);
	b2->getBody()->SetTransform(pos, 0.0);
	b3->getBody()->SetTransform(pos, 0.0);

	b2Vec2 vel = b2->getBody()->GetLinearVelocity();
	vel.x = (clickedPoint.x - playerBody->GetPosition().x);
	vel.y = (clickedPoint.y - playerBody->GetPosition().y);
	float angle = atan2f(vel.y, vel.x);
	vel.Normalize();
	vel *= b2->getSpeed();
	b2->getBody()->SetLinearVelocity(vel);
	b2->getBody()->SetTransform(b2->getBody()->GetPosition(), angle);

	b2Vec2 vel1;
	b2Vec2 vel3;

	if ((vel.x >= 0 && vel.y >= 0) || (vel.x < 0 && vel.y < 0))
	{
		vel1.x = vel.x - 1;
		vel1.y = vel.y + 1;

		vel3.x = vel.x + 1;
		vel3.y = vel.y - 1;
	}
	else
	{
		vel1.x = vel.x + 1;
		vel1.y = vel.y + 1;

		vel3.x = vel.x - 1;
		vel3.y = vel.y - 1;
	}



	float angle1 = atan2f(vel1.y, vel1.x);
	float angle3 = atan2f(vel3.y, vel3.x);

	vel1.Normalize();
	vel3.Normalize();

	vel1 *= b1->getSpeed();
	vel3 *= b3->getSpeed();

	b1->getBody()->SetLinearVelocity(vel1);
	b1->getBody()->SetTransform(b1->getBody()->GetPosition(), angle1);

	b3->getBody()->SetLinearVelocity(vel3);
	b3->getBody()->SetTransform(b3->getBody()->GetPosition(), angle3);

}

void physicsManager::deactivateBullet(Bullet *bullet)
{
	bullet->getBody()->SetActive(false);
	bullet->setCollided(false);
	b2Vec2 vel = bullet->getBody()->GetLinearVelocity();
	vel.x = 0.0;
	vel.y = 0.0;
	bullet->getBody()->SetLinearVelocity(vel);
}
// HARD CODED STUFF IN HERE FOR TESTING
void physicsManager::initTriangleWallForGame(b2Vec2 verts[], int32 count)
{

	/*for (int i = 0; i < count; i++)
	{
	float x = verts[i].x;
	float y = verts[i].y;

	verts[i].x = x * pixelScaling;
	verts[i].y = y * pixelScaling;
	}*/

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;

	b2Vec2 vertices[3];
	vertices[0].Set(0.0f, 0.0f);
	vertices[1].Set(0.5f, 0.0f);
	vertices[2].Set(0.5f, -0.25f);


	float centerX = (verts[0].x + verts[1].x + verts[2].x) / 3;
	float centerY = (verts[0].y + verts[1].y + verts[2].y) / 3;

	myBodyDef.position.Set(206 * pixelScaling, 242 * pixelScaling);

	b2PolygonShape triangle;
	triangle.Set(vertices, count);

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &triangle;
	myFixtureDef.density = 1;

	myFixtureDef.filter.categoryBits = collisionCatagory::WALL;

	b2Body* staticBody = gameWorld->CreateBody(&myBodyDef);
	staticBody->CreateFixture(&myFixtureDef);

}

void physicsManager::initWallForGame(float x, float y)
{
	float xPos = x * pixelScaling;
	float yPos = y * pixelScaling;

	float width = 32 * pixelScaling;
	float height = 32 * pixelScaling;

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(xPos, yPos);
	b2Body* staticBody = gameWorld->CreateBody(&myBodyDef);


	b2PolygonShape polygonShape;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	myFixtureDef.filter.categoryBits = collisionCatagory::WALL;


	polygonShape.SetAsBox(width / 4, (height) / 4);
	staticBody->CreateFixture(&myFixtureDef);


}

void physicsManager::resetPlayer(Player *p)
{
	float x = p->getInitX() * pixelScaling;
	float y = p->getInitY() * pixelScaling;

	p->getBody()->SetTransform(b2Vec2(x, y), p->getBody()->GetAngle());
}

