#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

extern PlayHadoukenSound _hadoukenSoundCB;
bool Hadouken_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_DOWN) && (inputKeyPressed(VK_RIGHT) || inputKeyPressed(VK_LEFT)) && inputKeyPressed(VK_P))
	{
		Character* character = (Character*)obj;
		GameplayComponent* gameplayComponent = GetCharacterGameplayComponent(character);
		if (gameplayComponent->currentAction.action == &gameplayComponent->actions[Ken_Hadouken])
		{
			return false;
		}
		CharacterGameplayState* currentStatePtr =  GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		if (CanSpawnHadouken(character))
		{
			return true;
		}
	}
	return false;
}

void Hadouken_OnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	CharacterGameplayState attackState = { Attack,Stand };
	TryChangeGameplayState(character, attackState);
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	attackComponent->isActive = false;

	if (inputKeyPressed(VK_RIGHT))
	{
		*characterDirectionPtr = Facing_Right;
	}
	else if (inputKeyPressed(VK_LEFT))
	{
		*characterDirectionPtr = Facing_Left;
	}
	else
	{
		//Strange case - Probably a time for assert
	}

	HadoukenPayload* payload = malloc(sizeof(HadoukenPayload));
	assert(payload != NULL);
	payload->isFired = false;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	TryDeInitActionPayload(gameplayComponentPtr, Ken_Hadouken); //if existing payload delete that
	InitActionPayload(gameplayComponentPtr, Ken_Hadouken, (Payload*)payload);
	
	if (_hadoukenSoundCB != NULL)
	{
		_hadoukenSoundCB(character);
	}
}

void Hadouken_GameplayController(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	Payload* payload = gameplayComponentPtr->actions[Ken_Hadouken].payload;
	assert(payload != NULL);
	HadoukenPayload* subPayload = (HadoukenPayload*)payload;

	//Calculate time gone by and spawn hadouken at right moment
	uint32_t duration = gameplayComponentPtr->currentAction.action->duration;
	uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
	float remainingTimeNormalized = (float)remainingTime / (float)duration;
	float timePassedNormalized = 1.0f - remainingTimeNormalized;

	if (timePassedNormalized > 0.7f && subPayload->isFired==false)
	{
		Coord2D hadoukenPos;
		float velocity = 1.0f;
		if (*characterDirectionPtr == Facing_Right)
		{
			hadoukenPos = obj->position;
			hadoukenPos.x += obj->size.x;
		}
		else
		{
			hadoukenPos = obj->position;
			hadoukenPos.x -= obj->size.x;
			velocity *= -1.0f;
		}
		hadoukenPos.y -= 5*HadoukenSpawnYOffset+5;
		TrySpawnHadouken(character, hadoukenPos, velocity);
		subPayload->isFired = true;
	}
}

void Hadouken_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	assert(gameplayComponentPtr->actions[Ken_Hadouken].payload != NULL);
	//Free payload
	DeInitActionPayload(gameplayComponentPtr, Ken_Hadouken);

}

void Hadouken_AnimationController(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	assert(gameplayComponentPtr->currentAction.action->duration > 1);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr != NULL);
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Hadouken, gameplayComponentPtr->currentAction.action->duration);

	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);

	if (*characterDirectionPtr == Facing_Right)
	{
		SetDirection(animComponentPtr, Animation_Right);
	}
	else if (*characterDirectionPtr == Facing_Left)
	{
		SetDirection(animComponentPtr, Animation_Left);
	}
}

//Player spawns a ball that spawns at a location provided by the character.Upon spawning the object continues moving in the direction given to it upon init until it exits the screen
//Character has reference to the hadouken.
//Hadouken is freed when it collides or exits screen bounds