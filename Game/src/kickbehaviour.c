#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

bool KickAction_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_Z)) //Z
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

void KickAction_OnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	CharacterGameplayState KickState = { Attack,Stand };
	Character* character = (Character*)obj;
	CharacterGameplayState* gameplayStatePtr = GetCharacterGameplayState(character);
	assert(gameplayStatePtr != NULL);
	if (!TryChangeGameplayState(character, KickState))
	{
		return;
	}
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);

		//Set attack component
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	if (*characterDirectionPtr == Facing_Right)
	{
		attackComponent->impactPoint.x = obj->position.x + obj->size.x / 2.0f;//at the edge of the frame
		attackComponent->impactPoint.x += ImpactPointKickOffset;

	}
	else
	{
		attackComponent->impactPoint.x = obj->position.x - obj->size.x / 2.0f;
		attackComponent->impactPoint.x -= ImpactPointKickOffset;

	}
	attackComponent->impactPoint.y = obj->position.y - (0.4f * obj->size.y); //explain this better why 0.4? want it slightly up and what feels right
	attackComponent->priority = mediumPriority;
	attackComponent->isActive = true;
}

void KickAction_GameplayController(Object* obj, uint32_t milliseconds)
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

	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_Kick].len == 2);
	Frame f1 = animComponentPtr->animations[KenAnim_Kick].frames[Frame_0];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;


	if (timePassedNormalized < 0.5f)
	{
		float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
		float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

	}
	else
	{
		ObjChangeSizeAbs(obj, CharacterStandWidth, CharacterStandHeight);
	}
}

void KickAction_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	attackComponent->isActive = false;
}

void KickAction_AnimationController(Object* obj, uint32_t milliseconds)
{
}

void Kick_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	assert(gameplayComponentPtr->currentAction.action->duration > 1);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr != NULL);
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Kick, gameplayComponentPtr->currentAction.action->duration);
}