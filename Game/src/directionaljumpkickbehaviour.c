#pragma once
#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

/// <summary>
/// The character can jump and kick in a direction so like press right, up and kick then character does this action.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
/// <returns></returns>
bool DirectionalJumpKick_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_UP) == true && (inputKeyPressed(VK_RIGHT) == true || inputKeyPressed(VK_LEFT) == true)
		&& inputKeyPressed(VK_Z)) //S is pressed
	{
		//if it is in jump state already then false
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump) //If already in air we dont want to activate this behaviour otherwise we can have stacking of jumping behaviours. Can be a feature but comes with its own set of challenges if not handled well can make the game look distasterful
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_DirectionalJumpKick].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void DirectionalJumpKick_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump }; //Not attack as you can see since the character only attacks in a specific frame
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

void DirectionalJumpKick_GameplayController(Object* obj, uint32_t milliseconds)
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
	assert(animComponentPtr->animations[KenAnim_DirectionalJumpKick].len == 5); //Making sure that the anim is 2 frames long
	//Getting length information directly from the frames instead of using hard values
	Frame f1 = animComponentPtr->animations[KenAnim_DirectionalJumpKick].frames[Frame_0]; //Getting all the frames using which we can modify object size from animation data
	Frame f2 = animComponentPtr->animations[KenAnim_DirectionalJumpKick].frames[Frame_1];
	Frame f3 = animComponentPtr->animations[KenAnim_DirectionalJumpKick].frames[Frame_2];
	Frame f4 = animComponentPtr->animations[KenAnim_DirectionalJumpKick].frames[Frame_3]; //attack frame
	Frame f5 = animComponentPtr->animations[KenAnim_DirectionalJumpKick].frames[Frame_4];

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
		MoveObjectTransform(obj, jumpXFactor * obj->velocity.x * milliseconds, 0); //prefer this since it fixes clamping for us
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
			MoveObjectToGround(obj); //make sure the object is on the groud. 
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
			attackComponent->impactPoint.x += ImpactPointKickOffset;
		}
		else
		{
			attackComponent->impactPoint.x = obj->position.x - obj->size.x / 2.0f;
			//temp code
			attackComponent->impactPoint.x -= ImpactPointKickOffset;
		}
		attackComponent->impactPoint.y = obj->position.y + obj->size.y / 2.0f;
		attackComponent->priority = mediumPriority;
		attackComponent->isActive = true; //WHen active the impact information is parsed 
	}
	else
	{
		TryChangeGameplayState(character, noneState);
		attackComponent->isActive = false;
	}
}

void DirectionalJumpKick_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_DirectionalJumpKick].len == 5);
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
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_DirectionalJumpKick, componentPtr->currentAction.action->duration);
}





