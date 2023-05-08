/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "../../../../stdafx.h"
#include "../../graphics/GameGraphics.h"
#include "../../gsm/ai/Bot.h"
#include "../../gsm/ai/BotRecycler.h"
#include "../../gsm/sprite/AnimatedSprite.h"
#include "../../gsm/sprite/AnimatedSpriteType.h"
#include "../../../../ShootingStar.h"
#include "../../../../Player.h"
#include "../../../../MageBoss.h"
#include "../../../../Bullet.h"
#include "../../../../BulletRecycler.h"
#include "../../../../Effect.h"
#include "../../../../EffectRecycler.h"

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;
	int numKills;
	BotRecycler recycler;
	BulletRecycler bulletRecycler;
	
	EffectRecycler effectRecycler;

	//ACTUAL BULLETS BEING FIRED
	list <Bullet*> activeBullets;

	

	list <ShootingStar*> stars;

	list <Effect*> activeEffects;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	Player player;

//	MageBoss mageBoss;

	//AnimatedSprite health;

	//vector<AnimatedSprite*> lifes;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager()		{ numKills = 0; }
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	int getNumKills() { return numKills; }
	BotRecycler *getBotRecycler(){ return &recycler; }
	BulletRecycler *getBulletRecycler(){ return &bulletRecycler; }
//	BulletRecycler *getEnemyBulletRecycler(){ return &enemyBulletRecycler; }
	EffectRecycler *getEffectRecycler(){ return &effectRecycler; }


	int						getNumberOfSprites()	{ return bots.size();		}
	Player*			getPlayer()				{ return &player;			}
//	MageBoss*		getMageBoss()				{ return &mageBoss; }
	//AnimatedSprite*			getHealth()				{ return &health;			}
	list<ShootingStar*>::iterator	getStarsIterator()		{ return stars.begin(); }
	list<ShootingStar*>::iterator	getEndOfStarsIterator()	{ return stars.end(); }

	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}


	list<Bullet*>::iterator	getActiveBulletsIterator()		{ return activeBullets.begin(); }
	list<Bullet*>::iterator	getEndOfActiveBulletsIterator()	{ return activeBullets.end(); }

	list<Effect*>::iterator	getEffectsIterator()		{ return activeEffects.begin(); }
	list<Effect*>::iterator	getEndOfEffectsIterator()	{ return activeEffects.end(); }

	//vector<AnimatedSprite*>::iterator	getLifesIterator()		{ return lifes.begin(); }
	//vector<AnimatedSprite*>::iterator	getEndOfLifesIterator()	{ return lifes.end(); }

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addActiveBullet(Bullet *bullet);
	void				addActiveEnemyBullet(Bullet *bullet);
	void				addEffect(Effect *effect);
	void				addStar(ShootingStar *starToAdd);
	//void				addLife(AnimatedSprite *lifeToAdd);
	void				addSpriteItemsToRenderList(Game *game);
	void				addShootingStarsToRenderList(Game *game);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	Bot*				removeBot(Bot *botToRemove);
	ShootingStar*		removeStar(Bot *starToRemove);
	void				unloadSprites(GameStateManager *gsm);
	void				update(Game *game);
	void				updateShootingStars(Game *game);
	void				resetBots(Game *game);
	//AnimatedSprite*		getLifeSprite(int id);
};