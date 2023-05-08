#pragma once
#include "Effect.h"
#include "stdafx.h"
#include "src\sssf\game\Game.h"
class EffectRecycler
{

	const int RECYCLABLE_EFFECT_INCREMENT = 20;

private:
	map<wstring, list<Effect *> *> recyclableEffects;
	map<wstring, Effect *> registeredEffectTypes;

public:
	EffectRecycler();
	~EffectRecycler();
	void addMoreEffects(Game *game, list<Effect *> *EffectsNeededList, wstring EffectType, unsigned int numToAdd);
	void initRecyclableEffects(Game *game, wstring EffectType, unsigned int maxEffectsNeeded);
	bool isRegisteredEffectType(wstring EffectType);
	void recycleEffect(wstring EffectType, Effect* EffectToRecycle);
	void registerEffectType(wstring EffectType, Effect *sampleEffect);
	Effect* retrieveEffect(Game *game, wstring EffectType);

	void reset(GameStateManager *gsm);

};

