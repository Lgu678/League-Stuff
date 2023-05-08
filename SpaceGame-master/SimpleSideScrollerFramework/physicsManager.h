#pragma once

#include "stdafx.h"
#include "src\sssf\gsm\sprite\AnimatedSprite.h"
#include "Player.h"
#include "Bullet.h"
#include "PhysicsCollisionListener.h"
#include "src\sssf\gsm\ai\Bot.h"
#include "MeleeBot.h"




class physicsManager
{

private:

	

	PhysicsCollisionListener *listener;

	b2World *levelSelectWorld;
	b2World *gameWorld;

	bool playerMoveLeft;
	bool playerMoveRight;
	bool playerMoveUp;
	bool playerMoveDown;
	bool viewportMoved;
	bool lavaBossHealth;
	bool lavaBossFight;

	b2Body *playerBody;

    bool mouseClicked;

	b2Vec2 clickedPoint;
	b2Vec2 mouseLoc;

	float viewportConverter;
	
	Game *game;

public:
	physicsManager();
	~physicsManager();

	enum collisionCatagory
	{
		WALL = 0x0001,
		PLAYER = 0x0002,
		ENEMY = 0x0004,
		PLAYER_BULLET = 0x0008,
		ENEMY_BULLET = 0x0010,
		BOT_DETECTOR = 0x0020,
		BOT_DAMAGE_RADIUS = 0x0040,
		BOT_RANGEATTACK_RADIUS = 0x080,
		BOT_DODGE = 0x0100,


	};

	float32 timeStep;     
	int32 velocityIterations;   
	int32 positionIterations;   
	float pixelScaling;

	b2World *getGameWorld()
	{
		return gameWorld;
	}

	float getPixelScaling()
	{
		return pixelScaling;
	}

	b2Body* getPlayerBody()
	{
		return playerBody;
	}

	void setMouseClicked(bool b)
	{
		mouseClicked = b;
	}

	b2Vec2 getMouseClickedPoint()
	{
		return clickedPoint;
	}

	void setMouseVec(float x, float y);
	void setMouseLoc(float x, float y);

	void initLevelSelectWorld();
	void initGameWorld();
	void clearGameWorld();
	void registerGameToListener();

	void initMageOrb(AnimatedSprite *orb, float x, float y, Bot *m);
	void activateMageOrb(AnimatedSprite *orb, b2Vec2 push);
	void deactivateMageOrb(AnimatedSprite *orb);

	

	void deleteSpriteBody(AnimatedSprite *sprite);

	void setGame(Game *g)
	{
		game = g;
	}

	void initSpriteBodyForLevelSelect(AnimatedSprite *sprite, float x, float y);

	void levelSelectWorldStep();

	void initBullet(Bullet *sprite);
	void initEnemyBullet(Bullet *sprite);
	void initLavaBullet(Bullet *sprite);
	void initLavaBurstBullet(Bullet *sprite);
	void activateBullet(Bullet *sprite);
	void activateEnemyBullet(Bullet *sprite, float x, float y);
	void activateMageLightning(Bullet *b, Bot *m, float angle);

	/*Takes 3 bullets to activate and produce a tri spread like effect*/
	void activateBulletSpread(Bullet *b1, Bullet *b2, Bullet *b3);

	void deactivateBullet(Bullet *sprite);

	void initWallForGame(float x, float y);

	void initTriangleWallForGame(b2Vec2 verts[],int32 count);

	void gameWorldStep();

	void initPlayer(Player *player, float x, float y);

	void resetPlayer(Player *p);

	void movePlayerLeft();
	void movePlayerRight();
	void movePlayerUp();
	void movePlayerDown();
	void viewportM();

	void initMeleeBot(Bot *bot, int bX, int bY, int aR);
	void initLavaBoss(Bot *bot, int bX, int bY, int aR);
	void initAndromaliusBoss(Bot *bot, int bX, int bY, int aR);
	void initMageBoss(Bot *bot, int bX, int bY, int aR);
	void initRangeBot(Bot *bot, int bX, int bY, int aR);
	void activateBot(Bot *bot, float x, float y);
	void deactivateBot(Bot *bot);

	void testResetGameWorld();

	
};

