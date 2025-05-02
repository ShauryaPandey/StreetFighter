#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>

bool PunchAction_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_P)) //Key P is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		return true;
	}
	return false;
}

void PunchAction_OnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	CharacterGameplayState PunchState = { Attack,Stand };
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	if (!TryChangeGameplayState(character, PunchState))
	{
		return;
	}

	//Set attack component
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	if (*characterDirectionPtr == Facing_Right)
	{
		attackComponent->impactPoint.x = obj->position.x + obj->size.x / 2.0f;
		attackComponent->impactPoint.x += ImpactPointPunchOffset;

	}
	else
	{
		attackComponent->impactPoint.x = obj->position.x - obj->size.x / 2.0f;
		attackComponent->impactPoint.x -= ImpactPointPunchOffset;

	}
	attackComponent->impactPoint.y = obj->position.y - (0.3f * obj->size.y); 
	attackComponent->priority = lowPriority;
	attackComponent->isActive = true;

}

void PunchAction_GameplayController(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	CharacterGameplayState attackState = { Attack,Stand };
	if (!TryChangeGameplayState(character, attackState))
	{
		return;
	}
	
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized;

	if (timePassedNormalized < 0.5f)
	{
		//first frame
		ObjChangeSizeAbs(obj, (75.0f / 63.0f) * CharacterStandWidth, CharacterStandHeight);
	}
	else
	{
		ObjChangeSizeAbs(obj, CharacterStandWidth, CharacterStandHeight);
	}
}

void PunchAction_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	attackComponent->isActive = false;
}

void PunchAction_AnimationController(Object* obj, uint32_t milliseconds)
{
}

void Punch_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	assert(gameplayComponentPtr->currentAction.action->duration > 1);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr != NULL);
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Punching, gameplayComponentPtr->currentAction.action->duration);
}