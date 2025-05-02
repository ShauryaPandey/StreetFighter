#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
//Punching + crouching behaviour
//Ken and Ryu will have different can starts mostly. 
bool CrouchPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_DOWN) && inputKeyPressed(VK_P)/*Key P is pressed*/)
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
//Runs at the start of the behaviour
void CrouchPunch_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	//Change state
	assert(obj != NULL);
	CharacterGameplayState crouchPunchState = { Attack,Crouch }; //Setting the correct state here
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	TryChangeGameplayState(character, crouchPunchState);
}

void CrouchPunch_GameplayController(Object* obj, uint32_t milliseconds)
{
	//Modify object size
	Character* character = (Character*)obj;
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_CrouchPunch].len == 2);
	Frame f1 = animComponentPtr->animations[KenAnim_CrouchPunch].frames[Frame_0]; //Getting information of each frame in this behaviour to help us in modifying size of the object accordingly
	Frame f2 = animComponentPtr->animations[KenAnim_CrouchPunch].frames[Frame_1];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0]; //Idle serves as reference frame for us which we use to change all other frames relatively
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized; //Time passed until this point out of 1.0

	if (timePassedNormalized < 0.5f)
	{
		float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
		float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}
	else
	{
		float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
		float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
		ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	}

	//Set attack impact point so then the attack collision will be checked against the impact point
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);

	if (timePassedNormalized < 0.5f)
	{
		assert(characterDirectionPtr != NULL);
		if (*characterDirectionPtr == Facing_Right)
		{
			attackComponent->impactPoint.x = obj->position.x + obj->size.x / 2.0f;
			attackComponent->impactPoint.x += ImpactPointPunchOffset; //Additional some increase so that the impact point can be felt

		}
		else
		{
			attackComponent->impactPoint.x = obj->position.x - obj->size.x / 2.0f;
			attackComponent->impactPoint.x -= ImpactPointPunchOffset;

		}
		attackComponent->impactPoint.y = obj->position.y + 50.0f;
		attackComponent->priority = lowPriority;
		attackComponent->isActive = true;
	}
	else 
	{
		attackComponent->isActive = false;
	}
}

void CrouchPunch_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_CrouchPunch, componentPtr->currentAction.action->duration);
}

void CrouchPunch_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	attackComponent->isActive = false; //as the behaviour ends we need to set attack component to inactive as it lasts only for the duration of the behaviour
}
