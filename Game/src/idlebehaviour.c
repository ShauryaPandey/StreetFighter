#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include <assert.h>

bool IdleAction_CanStart(Object* object, uint32_t milliseconds)
{
	return true;
}

void IdleAction_OnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	CharacterGameplayState IdleState = { None,Stand };
	TryChangeGameplayState(character, IdleState);
	UpdateCharacterSize(character, Stand);
	if (!IsObjectOnGround(obj))
	{
		MoveObjectToGround(obj);
	}
}

void Idle_GameplayController(Object* obj, uint32_t milliseconds)
{
	//Modify object size as frames update
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized;

	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	//So ryu has 3 idle frames and Ken has 4 frames

	//Getting length information directly from the frames instead of using hard values
	//There can be max 4 frames of idle
	Frame frames[4];
	int numFrames = animComponentPtr->animations[KenAnim_Idle].len;
	for (int i = 0; i < numFrames; i++)
	{
		frames[i] = animComponentPtr->animations[KenAnim_Idle].frames[i];
	}
	//Frame 1 and 4 are the same
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;
	float startTime = 0.0f;
	float endTime = startTime + 1.0f / animComponentPtr->animations[KenAnim_Idle].len;
	int i = 0;
	while (endTime <= 1.0f)
	{
		endTime = startTime + 1.0f / animComponentPtr->animations[KenAnim_Idle].len;
		if (timePassedNormalized > startTime && timePassedNormalized < endTime)
		{
			Frame f1 = frames[i];
			float increasedHeight = f1.BottomRight.y - f1.topLeft.y;
			float reducedLength = f1.BottomRight.x - f1.topLeft.x;
			ObjChangeSizeAbs(obj, (reducedLength / originalLength) * CharacterStandWidth, (increasedHeight / originalHeight) * CharacterStandHeight);
		}
		startTime = endTime;
	}
	++i;
}
void IdleAction_AnimationController(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	AnimationComponent* animComponent = GetCharacterAnimationComponent(character);
	GameplayComponent* gameplayComponent = GetCharacterGameplayComponent(character);
	assert(animComponent != NULL);
	assert(gameplayComponent != NULL);

	assert(gameplayComponent->currentAction.action->duration > 1);
	Ken_PlayAnimation_Uniform(animComponent, KenAnim_Idle, gameplayComponent->currentAction.action->duration);
}
