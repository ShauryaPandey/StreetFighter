#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

bool Retract_CanStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(gameplayComponentPtr != NULL);
	assert(currentStatePtr != NULL);
	if (obj->collider != NULL)
	{
		if (gameplayComponentPtr->actions[Ken_Retract].payload != NULL)
		{
			if (currentStatePtr->baseState == Retract)
			{
				return true; //If there was a collision and there is another collider then retract
			}
		}
	}
	return false;
}

void Retract_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	//Action is still valid condition?
	Character* character = (Character*)obj;
	assert(character != NULL);

	//Get other object
	if (obj->collider != NULL)
	{
		float diff = 0.0f;
		float shift = 0.0f;
		// Is the overlap on the right or the left
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		Payload* payload = gameplayComponentPtr->currentAction.action->payload;
		assert(payload != NULL);
		RetractPayload* retractPayload = (RetractPayload*)payload;
		assert(retractPayload != NULL);
		Object* other = retractPayload->other;/*obj->collider->info.other*/
		assert(other != NULL);
		diff = other->position.x - obj->position.x;
		if (diff > 0)
		{
			//Shift left
			shift = obj->size.x / 2.0f + other->size.x / 2.0f - diff;
			shift += (float)RetractShift;
			MoveObjectTransform(obj, -1.0f * shift, 0);
		}
		else if (diff < 0)
		{
			//Shift right
			shift = obj->size.x / 2.0f + other->size.x / 2.0f + diff;
			shift += (float)RetractShift;
			MoveObjectTransform(obj, 1.0f * shift, 0);

		}
	}
}

void Retract_GameplayController(Object* obj, uint32_t milliseconds)
{
}

void RetractAction_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	DeInitActionPayload(gameplayComponentPtr, Ken_Retract);
}