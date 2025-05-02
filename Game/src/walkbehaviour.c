#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>

bool WalkAction_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	if (currentStatePtr->subState == Jump)
	{
		return false;
	}

	if (inputKeyPressed(VK_RIGHT)) //Direction key is pressed
	{
		return true;
	}
	else if (inputKeyPressed(VK_LEFT))
	{
		return true;
	}
	return false;
}

void WalkAction_OnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	CharacterGameplayState WalkState = { None,Stand };
	CharacterGameplayState* currentState = GetCharacterGameplayState(character);
	assert(currentState != NULL);
	TryChangeGameplayState(character, WalkState);
	UpdateCharacterSize(character, Stand);
	if (!IsObjectOnGround(obj))
	{

	}
	FacingDirection* direction = GetCharacterFacingDirection(character);
	assert(direction != NULL);
	if (inputKeyPressed(VK_RIGHT)) //Direction key is pressed
	{
		*direction = Facing_Right;
	}
	else if (inputKeyPressed(VK_LEFT))
	{
		*direction = Facing_Left;
	}
	else
	{
	}
}

void WalkAction_GameplayController(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	FacingDirection* direction = GetCharacterFacingDirection(character);
	if (*direction == Facing_Left)
	{
		float deltaX = -0.5f * obj->velocity.x * milliseconds;
		MoveObjectTransform(obj, deltaX, 0);
	}
	else if (*direction == Facing_Right)
	{
		float deltaX = 0.5f*obj->velocity.x * milliseconds;
		MoveObjectTransform(obj, deltaX, 0);
	}
}

void WalkAction_AnimationController(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	AnimationComponent* animComponent = GetCharacterAnimationComponent(character);
	assert(animComponent != NULL);
	GameplayComponent* gameplayComponent = GetCharacterGameplayComponent(character);
	assert(gameplayComponent != NULL);
	AnimationDirection animDirection;
	//Default state any input can change this but if not this stays
	assert(gameplayComponent->currentAction.action->duration > 1);
	Ken_PlayAnimation_Uniform(animComponent, KenAnim_Walking, gameplayComponent->currentAction.action->duration);
	FacingDirection* characterDirection = GetCharacterFacingDirection(character);
	assert(characterDirection != NULL);
	if (*characterDirection == Facing_Right)
	{
		animDirection = Animation_Right;
		SetDirection(animComponent, animDirection);
	}
	else if (*characterDirection == Facing_Left)
	{
		animDirection = Animation_Left;
		SetDirection(animComponent, animDirection);
	}
}