#pragma once
#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
//Jumping and punching
bool DirectionalJumpPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_UP) == true && (inputKeyPressed(VK_RIGHT) == true || inputKeyPressed(VK_LEFT) == true) 
		&& inputKeyPressed(VK_P)) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_DirectionalJumpPunch].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void DirectionalJumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(gameplayComponentPtr != NULL);

	if (inputKeyPressed(VK_RIGHT) == true)
	{
		*characterDirectionPtr = Facing_Right;
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}
}

void DirectionalJumpPunch_GameplayController(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized;

	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_DirectionalJumpPunch].len == 5); //Making sure that the anim is 2 frames long
	//Getting length information directly from the frames instead of using hard values
	Frame f1 = animComponentPtr->animations[KenAnim_DirectionalJumpPunch].frames[Frame_0];
	Frame f2 = animComponentPtr->animations[KenAnim_DirectionalJumpPunch].frames[Frame_1];
	Frame f3 = animComponentPtr->animations[KenAnim_DirectionalJumpPunch].frames[Frame_2];
	Frame f4 = animComponentPtr->animations[KenAnim_DirectionalJumpPunch].frames[Frame_3]; //attack frame
	Frame f5 = animComponentPtr->animations[KenAnim_DirectionalJumpPunch].frames[Frame_4];

	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0]; //reference frame
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	//breaking frames uniformly 1.0/5
	//Change object size
	if (timePassedNormalized < 0.2f)
	{
		float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
		float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else if (timePassedNormalized < 0.4f)
	{
		float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
		float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else if (timePassedNormalized < 0.6f)
	{
		float modifiedHeight = f3.BottomRight.y - f3.topLeft.y;
		float modifiedLength = f3.BottomRight.x - f3.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else if (timePassedNormalized < 0.8f)
	{
		float modifiedHeight = f4.BottomRight.y - f4.topLeft.y;
		float modifiedLength = f4.BottomRight.x - f4.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else
	{
		float modifiedHeight = f5.BottomRight.y - f5.topLeft.y;
		float modifiedLength = f5.BottomRight.x - f5.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	//Motion manipulation
	if (*characterDirectionPtr == Facing_Right)
	{
		MoveObjectTransform(obj, jumpXFactor * obj->velocity.x * milliseconds, 0);
	}
	else
	{
		MoveObjectTransform(obj, -1.0f * jumpXFactor * obj->velocity.x * milliseconds, 0);
	}

	if (timePassedNormalized < 0.5f)
	{
		MoveObjectTransform(obj, 0, -1.0f * jumpYFactor * obj->velocity.y * milliseconds);
	}
	else
	{
		if (obj->position.y + obj->size.y / 2.0f < g_groundLevel)
		{
			MoveObjectTransform(obj, 0, jumpFallYFactor * obj->velocity.y * milliseconds);
		}
		else
		{
			MoveObjectToGround(obj);
		}
	}

	//Activate/Deactivate attack component
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	CharacterGameplayState attackState = { Attack,Jump };
	CharacterGameplayState noneState = { None,Jump };
	assert(characterDirectionPtr != NULL);
	if (timePassedNormalized > 0.6f && timePassedNormalized < 0.8f)
	{
		if (!TryChangeGameplayState(character, attackState))
		{
			return;
		}
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
		attackComponent->impactPoint.y = obj->position.y ;
		attackComponent->priority = lowPriority;
		attackComponent->isActive = true;
	}
	else
	{
		TryChangeGameplayState(character, noneState);
		attackComponent->isActive = false;
	}
}

void DirectionalJumpPunch_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_DirectionalJumpPunch].len == 5);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);
	FacingDirection* characterDirection = GetCharacterFacingDirection(character);
	assert(characterDirection != NULL);
	if (*characterDirection == Facing_Right)
	{
		SetDirection(animComponentPtr, Animation_Right);
	}
	else if (*characterDirection == Facing_Left)
	{
		SetDirection(animComponentPtr, Animation_Left);
	}
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_DirectionalJumpPunch, componentPtr->currentAction.action->duration);
}
