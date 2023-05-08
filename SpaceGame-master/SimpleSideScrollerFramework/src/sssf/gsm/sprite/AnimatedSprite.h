/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "../../../../stdafx.h"

#include "../../gsm/sprite/AnimatedSpriteType.h"
#include "../../gui/Viewport.h"




class AnimatedSprite
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	bool markedForDeath;
	bool damageImune;
	bool outOfLives;
	bool doubleJump;
	int playerLives;
	unsigned int imuneTime;

	// ITS POSSIBLE SOME SPRITES WILL NOT HAVE BOX2D BODIES AS THERE IS NOT NEED TO
	// IN THAT CASE WE SIMPLE GIVE IT DEFAULT RENDERING COORDS
	float defaultX;
	float defaultY;

	b2Body *body;

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;				}
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}
	bool				getMarkedForDeath() { return markedForDeath;    }
	bool				getDamageImune()	{ return damageImune;       }
	int					getImmuneTime()		{ return imuneTime; }
	b2Body*				getBody()			{ return body; }
	float				getDefaultX()			{ return defaultX; }
	float				getDefaultY()			{ return defaultY; }

	void setDefaultX(float x)
	{
		defaultX = x;
	}

	void setDefaultY(float y)
	{
		defaultY = y;
	}

	void setBody(b2Body *b)
	{
		body = b;
	}

	

	void setImmuneTime(unsigned int x)
	{
		imuneTime = x;
	}



	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void setMarkedForDeath(bool death)
	{		markedForDeath = death;				}

	

	void setDamageImune(bool d)
	{
		damageImune = d;
	}




	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
	void resetSprite();
	// THE ONLY POINT OF THIS IS TO ALLOW FOR DYNAMIC CASTING
	virtual void test();
};