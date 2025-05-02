#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

/// <summary>
/// Crouching basic behaviour. The character plays the crouch animation and its size also reduces accordingly
/// Ken and Ryu can have separate canStart functions
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
/// <returns></returns>
bool Crouch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_DOWN) == true) //S is pressed
	{
		Character* character = (Character*)obj;
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		if (gameplayComponentPtr->actions[Ken_Crouch].payload == NULL)
		{
			return true;
		}
		
	}
	return false;
}

void Crouch_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	//Set state
	assert(obj != NULL);
	CharacterGameplayState crouchState = { None,Crouch };
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	TryChangeGameplayState(character, crouchState);
	//We need to get the animation frames and get the size of the frames using that we can modify size of the object in actual gameplay
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_Crouch].len == 1);
	Frame f1 = animComponentPtr->animations[KenAnim_Crouch].frames[Frame_0];
	//THe idle fram serves as reference for us. THis is seen in most behaviours for resizing
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
	float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
	ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
}

void Crouch_GameplayController(Object* obj, uint32_t milliseconds)
{
//Not needed
}

//Uniform animation duration
void Crouch_AnimOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);

	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Crouch, componentPtr->currentAction.action->duration);
}

void Crouch_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	//Not needed
}