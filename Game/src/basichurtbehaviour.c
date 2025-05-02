#include "character.h"
#include "basichurtbehaviour.h"
#include "standhurtbehaviour.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>

PlayPunchSoundCB g_punchSoundCB;
/*
* This behaviour is used when the damage is basic like a punch or a kick. The character shows a hurt reaction - separate in standind and crouching
* 
*/


/// <summary>
/// It can start if the state has been set to hurt and is having substate crouch or stand.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
/// <returns></returns>
bool BasicHurt_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	if (currentStatePtr->baseState == Hurt)
	{
		if (currentStatePtr->subState == Stand || currentStatePtr->subState == Crouch)
		{
			GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
			assert(gameplayComponentPtr != NULL);
			Payload* payload = gameplayComponentPtr->actions[Ken_BasicHurt].payload; 
			if (payload != NULL)
			{
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// As the behaviour starts, we  set the payload and attach it to the action.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void BasicHurt_OnStart(Object* obj, uint32_t milliseconds)
{
	//Find where attack came from and update direction
	assert(obj != NULL);

	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	Payload* payload = gameplayComponentPtr->actions[Ken_BasicHurt].payload;
	assert(payload != NULL);
	HurtPayload* subPayload = (HurtPayload*)payload;
	Coord2D otherPosition = subPayload->otherPosition;
	
	//Get other position 
    //if other is on my right my direction is right else left
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	if (otherPosition.x - obj->position.x > 0)//other is on my right
	{
		*characterDirectionPtr = Facing_Right; //This character must face the other when hurting
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}

	//Modify character size depending on if they are standing or crouching
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	if (currentStatePtr->subState == Stand)
	{
		Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];

		Frame basicHurtFrame = animComponentPtr->animations[KenAnim_BasicHurt].frames[Frame_0];
		float increasedLength = basicHurtFrame.BottomRight.x - basicHurtFrame.topLeft.x;
		float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
		ObjChangeSizeAbs(obj, (increasedLength / originalLength) * CharacterStandWidth, CharacterStandHeight);
	}
	else if (currentStatePtr->subState == Crouch)
	{
		Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];

		Frame crouchHurtFrame = animComponentPtr->animations[KenAnim_CrouchHurt].frames[Frame_0];
		float increasedLength = crouchHurtFrame.BottomRight.x - crouchHurtFrame.topLeft.x;
		float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
		ObjChangeSizeAbs(obj, (increasedLength / originalLength) * CharacterStandWidth, CharacterCrouchHeight);
	}
	else
	{
		assert(false); // It should not be in this block
	}

	//health
	ModifyCharacterHealth(character, BasicHurtValue);
	
	//Play sound
	if (g_punchSoundCB != NULL)
	{
		g_punchSoundCB();
	}
	//DONT Restore original direction in cleanup
	//Payload holds original position, direction
}

/// <summary>
/// Just play animation uniformly i.e divide action duration by number of frames and give that time to each frame. 
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void BasicHurt_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);
	assert(currentStatePtr != NULL);

	if (*characterDirectionPtr == Facing_Right)
	{
		SetDirection(animComponentPtr, Animation_Right);
	}
	else if (*characterDirectionPtr == Facing_Left)
	{
		SetDirection(animComponentPtr, Animation_Left);
	}

	if (currentStatePtr->subState == Stand)
	{
		Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_BasicHurt, componentPtr->currentAction.action->duration);
	}
	else if (currentStatePtr->subState == Crouch)
	{
		Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_CrouchHurt, componentPtr->currentAction.action->duration);
	}
	else
	{
		assert(false);
	}
	
}

void BasicHurt_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	Payload* payload = gameplayComponentPtr->currentAction.action->payload;
	assert(payload != NULL);
	HurtPayload* subPayload = (HurtPayload*)payload;
	//Free payload
	DeInitActionPayload(gameplayComponentPtr, Ken_BasicHurt);
}
