#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"

#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

//When the character blocks a hit they need to be moved back a bit while blocking in stand or crouch

/// <summary>
/// If the state is block hurt this behaviour can trigger
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
/// <returns></returns>
bool BlockHurt_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	if (currentStatePtr->baseState == BlockHurt)
	{
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_BlockHurt].payload != NULL)
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// Only affecting character health.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void BlockHurt_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	ModifyCharacterHealth(character, BlockHurtValue);
}

/// <summary>
/// Moves the character back when blocking an attack successfully.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void BlockHurt_GameplayController(Object* obj, uint32_t milliseconds)
{
	static const float movementFactor = 0.1f;
	//Move character in opposite direction to their facing direction
	//Logic similar to walking
	assert(obj != NULL);
	Character* character = (Character*)obj;
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	if (*characterDirectionPtr == Facing_Left) //then move right
	{
		MoveObjectTransform(obj, movementFactor*obj->velocity.x * milliseconds, 0);
	}
	else if (*characterDirectionPtr == Facing_Right) //move left
	{
		obj->position.x -= obj->velocity.x;
		MoveObjectTransform(obj, -1.0f* movementFactor * obj->velocity.x * milliseconds, 0);
	}
}

/// <summary>
/// Like all cleanups of all behaviours this cleanup must also clear its payload. 
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void BlockHurt_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	assert(gameplayComponentPtr->actions[Ken_BlockHurt].payload != NULL);
	DeInitActionPayload(gameplayComponentPtr, Ken_BlockHurt);
}