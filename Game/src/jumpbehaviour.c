#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

bool Jump_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_UP) == true) //S is pressed
	{
		Character* character = (Character*)obj;
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_StraightJump].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void Jump_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
}

void Jump_GameplayController(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized;

	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_StraightJump].len == 3); //Making sure that the anim is 2 frames long
	//Getting length information directly from the frames instead of using hard values
	Frame f1 = animComponentPtr->animations[KenAnim_StraightJump].frames[Frame_0];
	Frame f2 = animComponentPtr->animations[KenAnim_StraightJump].frames[Frame_1];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;

	//Change size of box based on time and frame
	if (timePassedNormalized < 0.3f || timePassedNormalized>0.7f)
	{
		float increasedHeight = f1.BottomRight.y - f1.topLeft.y;
		float reducedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (reducedLength / originalLength) * CharacterStandWidth, (increasedHeight / originalHeight) * CharacterStandHeight);
	}
	else
	{
		float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
		float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
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

}

void Jump_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_StraightJump].len == 3);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_StraightJump, componentPtr->currentAction.action->duration);
}
