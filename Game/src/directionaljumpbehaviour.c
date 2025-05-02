#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

/// <summary>
/// This behaviour - the character jumps in a direction as governed by press of arrow keys or alternatively a and d for other character.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
/// <returns></returns>
bool DirectionalJump_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_UP) == true 
		&& (inputKeyPressed(VK_RIGHT)==true || inputKeyPressed(VK_LEFT)==true) 
		&& (!inputKeyPressed(VK_P) && !inputKeyPressed(VK_Z))) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump) //if character is already jumping then don't trigger this behaviour
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_DirectionalJump].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void DirectionalJump_GameplayOnStart(Object* obj, uint32_t milliseconds)
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
		*characterDirectionPtr = Facing_Right; //THis is a forward facing jump. So if you press the right arrow key the character starts facing right and jumps there
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}
}

void DirectionalJump_GameplayController(Object* obj, uint32_t milliseconds)
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
	assert(animComponentPtr->animations[KenAnim_DirectionalJump].len == 4); //Making sure that the anim is 2 frames long
	//Getting length information directly from the frames instead of using hard values
	Frame f1 = animComponentPtr->animations[KenAnim_DirectionalJump].frames[Frame_0]; //Getting all the frames and changing size of objject on a relative scale to the idle frame
	Frame f2 = animComponentPtr->animations[KenAnim_DirectionalJump].frames[Frame_1];
	Frame f3 = animComponentPtr->animations[KenAnim_DirectionalJump].frames[Frame_2];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	//Change object size
	if (timePassedNormalized < 0.4f || timePassedNormalized>0.8f)
	{
		float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
		float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

	}
	else if (timePassedNormalized < 0.5f)
	{
		float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
		float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else if (timePassedNormalized > 0.5f)
	{
		float modifiedHeight = f3.BottomRight.y - f3.topLeft.y;
		float modifiedLength = f3.BottomRight.x - f3.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	//Motion manipulation
	//Horizontal movement
	if (*characterDirectionPtr == Facing_Right)
	{

		MoveObjectTransform(obj, jumpXFactor * obj->velocity.x * milliseconds, 0); //Prefer to use this function since it also checks clamping for us
	}
	else
	{

		MoveObjectTransform(obj, -1.0f*jumpXFactor * obj->velocity.x * milliseconds, 0);
	}
	//Vertical movement
	if (timePassedNormalized < 0.5f)
	{
		MoveObjectTransform(obj, 0, -1.0f * jumpYFactor * obj->velocity.y * milliseconds);
	}
	else //fall down
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
//Uniform duration animation
void DirectionalJump_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_DirectionalJump].len == 4);
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
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_DirectionalJump, componentPtr->currentAction.action->duration);
}