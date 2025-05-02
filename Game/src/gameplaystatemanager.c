#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

#include "input.h"
#include "baseTypes.h"
#include "character.h"
#include "gameplaycomponent.h"
#include "gameplaystatemanager.h"
#include "Object.h"

// #To-Do : find a way to define transitions and based on that return true or false if the request is possible like a formal state machine pattern.

/// <summary>
/// The character can be in various states to make sure there is one centralized point where
/// those states are changed this function does that job. It also lets the caller know whether 
/// the requested state change is done or not.
/// </summary>
/// <param name="character"></param>
/// <param name="newState"></param>
/// <returns></returns>
bool TryChangeGameplayState(Character* character, CharacterGameplayState newState)
{
	CharacterGameplayState* currentState = GetCharacterGameplayState(character);
	if (currentState->baseState == Hurt)
	{
		GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
		if (componentPtr->actions[Ken_BasicHurt].payload != NULL
			|| componentPtr->actions[Ken_StandHurt].payload != NULL
			|| componentPtr->actions[Ken_BlockHurt].payload!=NULL)
		{
			return false;
		}
	}
	if (currentState->subState != newState.subState)
	{
		UpdateCharacterSize(character, newState.subState);
	}
	assert(currentState != NULL);
	currentState->baseState = newState.baseState;
	currentState->subState = newState.subState;
	return true;
}