#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

bool JumpPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_UP) == true && inputKeyPressed(VK_P) &&(!inputKeyPressed(VK_RIGHT) && !inputKeyPressed(VK_LEFT)))
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_JumpPunch].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void JumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
}

void JumpPunch_GameplayController(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized; //calculating time that has passed out of duration in range of 0 to 1

	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_JumpPunch].len == 4); //Making sure that the anim is 2 frames long
	//Getting length information directly from the frames instead of using hard values
	Frame f1 = animComponentPtr->animations[KenAnim_JumpPunch].frames[Frame_0];
	Frame f2 = animComponentPtr->animations[KenAnim_JumpPunch].frames[Frame_1];
	Frame f3 = animComponentPtr->animations[KenAnim_JumpPunch].frames[Frame_2];
	//Frame 1 and 4 are the same
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;

	//Change size of box based on time and frame
	if (timePassedNormalized < 0.25f || timePassedNormalized>0.75f)
	{
		float increasedHeight = f1.BottomRight.y - f1.topLeft.y;
		float reducedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (reducedLength / originalLength) * CharacterStandWidth, (increasedHeight / originalHeight) * CharacterStandHeight);
	}
	else if(timePassedNormalized<0.5f)
	{
		float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
		float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else
	{
		float modifiedHeight = f3.BottomRight.y - f3.topLeft.y;
		float modifiedLength = f3.BottomRight.x - f3.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	//Control the motion
	if (timePassedNormalized < 0.5f) //going up
	{
		MoveObjectTransform(obj, 0, -1.0f * jumpYFactor * obj->velocity.y * milliseconds);
	}
	else//faling down
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

	//Activate/Deactive attack
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	CharacterGameplayState attackState = { Attack,Jump };
	CharacterGameplayState noneState = { None,Jump };
	assert(characterDirectionPtr != NULL);
	if (timePassedNormalized > 0.5f && timePassedNormalized < 0.75f)
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
		attackComponent->impactPoint.y = obj->position.y;
		attackComponent->priority = lowPriority;
		attackComponent->isActive = true;
	}
	else
	{
		TryChangeGameplayState(character, noneState);
		attackComponent->isActive = false;
	}
}

void JumpPunch_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_JumpPunch].len == 4);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_JumpPunch, componentPtr->currentAction.action->duration);
}