#include "EffectRecycler.h"
#include "src\sssf\gsm\state\GameStateManager.h"



EffectRecycler::EffectRecycler()
{
}


EffectRecycler::~EffectRecycler()
{
	
}

void EffectRecycler::reset(GameStateManager *gsm)
{

	

	map<wstring, list<Effect *> *>::iterator testIt = recyclableEffects.begin();
	while (testIt != recyclableEffects.end())
	{
		list<Effect*> *toDelete = testIt->second;
		list<Effect*>::iterator listIt = toDelete->begin();
		while (listIt != toDelete->end())
		{
			Effect *effect = *listIt;
			//game->getGSM()->getPhyiscs()->deleteSpriteBody(effect);
			delete effect;
			effect = NULL;
			toDelete->erase(listIt++);
		}
		toDelete->clear();
		delete toDelete;
		toDelete = NULL;
		recyclableEffects.erase(testIt++);


	}
	recyclableEffects.clear();


	map<wstring, Effect *>::iterator regEff = registeredEffectTypes.begin();
	while (regEff != registeredEffectTypes.end())
	{
		Effect *eff = regEff->second;
		//game->getGSM()->getPhyiscs()->deleteSpriteBody(eff);
		delete eff;
		eff = NULL;
		registeredEffectTypes.erase(regEff++);

	}

	registeredEffectTypes.clear();
}


void EffectRecycler::initRecyclableEffects(Game *game, wstring EffectType, unsigned int maxEffectsNeeded)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// EffectS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	map<wstring, list<Effect *> *>::iterator testIt = recyclableEffects.find(EffectType);
	if (testIt == recyclableEffects.end())
	{
		// MAKE THE RECYCLE LIST
		recyclableEffects[EffectType] = new list<Effect *>();

		// AND ADD THE EffectS
		addMoreEffects(game, recyclableEffects[EffectType], EffectType, maxEffectsNeeded);
	}
}

/*
It is assumed that a game will have many different types of Effects, thinking
in different ways, some of them even custom Effects defined by the game app rather
than by this framework. So, in order to make these Effects recyclable, we have to
register them with this recycler so that this recycler knows how to make new
ones as needed.
*/
void EffectRecycler::registerEffectType(wstring EffectType, Effect *sampleEffect)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// EffectS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	if (!isRegisteredEffectType(EffectType))
	{
		// REGISTER THE Effect
		registeredEffectTypes[EffectType] = sampleEffect;
	}
}

bool EffectRecycler::isRegisteredEffectType(wstring EffectType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED Effect TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	map<wstring, Effect *>::iterator testIt = registeredEffectTypes.find(EffectType);
	if (testIt == registeredEffectTypes.end())
		return false;
	else
		return true;
}


Effect* EffectRecycler::retrieveEffect(Game *game, wstring EffectType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED Effect TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	if (!isRegisteredEffectType(EffectType))
	{
		return NULL;
	}
	else
	{
		// GET THE CORRECT LIST OF Effect TYPES
		list<Effect *> *EffectsOfTypeWeNeed = recyclableEffects[EffectType];

		// MAKE SURE WE ARE NOT OUT OF THIS TYPE OF Effect
		if (EffectsOfTypeWeNeed->size() == 0)
			addMoreEffects(game, EffectsOfTypeWeNeed, EffectType, RECYCLABLE_EFFECT_INCREMENT);

		// NOW GET THE LAST ELEMENT
		Effect* EffectToReturn = EffectsOfTypeWeNeed->back();
		EffectsOfTypeWeNeed->pop_back();
		EffectToReturn->reset();
		return EffectToReturn;
	}
}

void EffectRecycler::recycleEffect(wstring EffectType, Effect* EffectToRecycle)
{
	if (isRegisteredEffectType(EffectType))
	{
		list<Effect *> *recycleEffects = recyclableEffects[EffectType];
		recycleEffects->push_back(EffectToRecycle);
	}
}

void EffectRecycler::addMoreEffects(Game *game, list<Effect *> *EffectsNeededList, wstring EffectType, unsigned int numToAdd)
{
	// NOW MAKE THE EffectS
	Effect* sampleEffect = registeredEffectTypes[EffectType];

	for (unsigned int i = 0; i < numToAdd; i++)
	{
		Effect *EffectClone = sampleEffect->clone(game);
		EffectsNeededList->push_back(EffectClone);
	}
}
