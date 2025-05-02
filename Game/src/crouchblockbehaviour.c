#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>
//In this behaviour the character blocks while crouching

bool CrouchBlock_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_DOWN) && inputKeyPressed(VK_2))
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

void CrouchBlock_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	CharacterGameplayState crouchBlockState = { Block,Crouch };
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	TryChangeGameplayState(character, crouchBlockState); // #To-Do : This also changes size. Move it from this function and do it explicitly here

	//Modify object size using the animation frames data and idle frame as refernce
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_CrouchBlock].len == 1);
	Frame f1 = animComponentPtr->animations[KenAnim_CrouchBlock].frames[Frame_0];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
	float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
	ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

	//Set defense component
	DefenseComponent* defenseComponentPtr = GetCharacterDefenseComponent(character);
	assert(defenseComponentPtr != NULL);
	defenseComponentPtr->range = Defense_Full;
}

//Uniform frame times
void CrouchBlock_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_CrouchBlock, componentPtr->currentAction.action->duration);
}

