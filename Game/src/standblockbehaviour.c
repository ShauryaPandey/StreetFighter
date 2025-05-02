#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

//To-do: payload for gettin direction in which to move 
bool StandBlock_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_2)) // #To-Do : Put key codes in defines
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

void StandBlock_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	//If there is something to do as the action starts
	//Change gameplay state
	assert(obj != NULL);
	CharacterGameplayState standBlockState = { Block,Stand };
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	TryChangeGameplayState(character, standBlockState);

	//Modify object size
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_StandBlock].len == 1);
	Frame f1 = animComponentPtr->animations[KenAnim_StandBlock].frames[Frame_0];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
	float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
	ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

	//Set defense component
	DefenseComponent* defenseComponentPtr = GetCharacterDefenseComponent(character);
	assert(defenseComponentPtr != NULL);
	defenseComponentPtr->range = Defense_Upper;
}

void StandBlock_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_StandBlock, componentPtr->currentAction.action->duration);
}