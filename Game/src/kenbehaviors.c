#include "character.h"
#include "gameplaycomponent.h"
#include "gameplaystatemanager.h"
#include "idlebehaviour.h"
#include "walkbehaviour.h"
#include "punchbehaviour.h"
#include "kickbehaviour.h"
#include "retractbehaviour.h"
#include "crouchbehaviour.h"
#include "crouchpunchbehaviour.h"
#include "crouchkickbehaviour.h"
#include "crouchblockbehaviour.h"
#include "standblockbehaviour.h"
#include "standhurtbehaviour.h"
#include "basichurtbehaviour.h"
#include "blockhurtbehaviour.h"
#include "hadoukenbehaviour.h"
#include "jumpbehaviour.h"
#include "jumppunchbehaviour.h"
#include "jumpkickbehaviour.h"
#include "directionaljumpbehaviour.h"
#include "directionaljumppunchbehaviour.h"
#include "directionaljumpkickbehaviour.h"
#include "keycodes.h"
#include "input.h"
#include <windows.h>
#include <assert.h>

bool DeathBehaviour_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	if (IsCharacterDead(character))
	{
		return true;
	}
	return false;
}

void DeathBehaviour_OnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	componentPtr->actions[Ken_Death].duration = UINT32_MAX - 1;
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_Death].len == 1);
	Frame f1 = animComponentPtr->animations[KenAnim_Death].frames[Frame_0];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;

	float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
	float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
	ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

	//Modify transform so that object is on ground
	MoveObjectToGround(obj);
}

void DeathBehaviour_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Death, componentPtr->currentAction.action->duration);
}

bool VictoryBehaviour_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	if (isCharacterWon(character))
	{
		return true;
	}
	return false;
}

void VictoryBehaviour_OnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	componentPtr->actions[Ken_Victory].duration = UINT32_MAX - 1;
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(animComponentPtr->animations[KenAnim_Victory].len == 1);
	Frame f1 = animComponentPtr->animations[KenAnim_Victory].frames[Frame_0];
	Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
	float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
	float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;

	float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
	float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
	ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
	//modify transfrom
	MoveObjectToGround(obj);
}

void VictoryBehaviour_AnimationOnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
	assert(componentPtr != NULL);
	assert(animComponentPtr != NULL);
	Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_Victory, componentPtr->currentAction.action->duration);
}

void InitKenGameplayComponent(Object* obj)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
	assert(componentPtr != NULL);
	//Create payload. Should I make a sub payload for Ken??
	componentPtr->payload.position = obj->position;
	componentPtr->payload.size = obj->size;

	componentPtr->len = Ken_Count;
	componentPtr->actions = malloc(componentPtr->len * sizeof(Action));
	ActionsInit(componentPtr); // Sets every action property of each of the actions to NULL/0
	assert(componentPtr->actions != NULL);
	//i ==0 punching
	componentPtr->actions[Ken_Punch].duration = 200;
	componentPtr->actions[Ken_Punch].canStartFunc = PunchAction_CanStart;
	componentPtr->actions[Ken_Punch].gameplayOnStartFunc = PunchAction_OnStart;
	componentPtr->actions[Ken_Punch].gameplayFunc = PunchAction_GameplayController;
	componentPtr->actions[Ken_Punch].animationFunc = PunchAction_AnimationController;
	componentPtr->actions[Ken_Punch].animationOnStartFunc = Punch_AnimOnStart;
	componentPtr->actions[Ken_Punch].cleanupFunc = PunchAction_GameplayCleanup;
	componentPtr->actions[Ken_Punch].priority = 10;

	//i == 1 walking
	componentPtr->actions[Ken_Walk].duration = 200;
	componentPtr->actions[Ken_Walk].canStartFunc = WalkAction_CanStart;
	componentPtr->actions[Ken_Walk].gameplayOnStartFunc = WalkAction_OnStart;
	componentPtr->actions[Ken_Walk].gameplayFunc = WalkAction_GameplayController;
	componentPtr->actions[Ken_Walk].animationFunc = WalkAction_AnimationController;
	componentPtr->actions[Ken_Walk].cleanupFunc = NULL;
	componentPtr->actions[Ken_Walk].priority = 5;

	// i == 2 Idle
	componentPtr->actions[Ken_Idle].duration = 500;
	componentPtr->actions[Ken_Idle].canStartFunc = IdleAction_CanStart;
	componentPtr->actions[Ken_Idle].gameplayOnStartFunc = IdleAction_OnStart;
	//component->actions[Ken_Idle].gameplayOnStartFunc = IdleAction_OnStart;
	componentPtr->actions[Ken_Idle].gameplayFunc = NULL;
	componentPtr->actions[Ken_Idle].animationFunc = IdleAction_AnimationController;
	componentPtr->actions[Ken_Idle].cleanupFunc = NULL;
	componentPtr->actions[Ken_Idle].priority = 1;

	//i ==  3 Kicking
	componentPtr->actions[Ken_Kick].duration = 300;
	componentPtr->actions[Ken_Kick].canStartFunc = KickAction_CanStart;
	componentPtr->actions[Ken_Kick].gameplayOnStartFunc = KickAction_OnStart;
	componentPtr->actions[Ken_Kick].gameplayFunc = KickAction_GameplayController;
	componentPtr->actions[Ken_Kick].animationFunc = KickAction_AnimationController;
	componentPtr->actions[Ken_Kick].animationOnStartFunc = Kick_AnimOnStart;
	componentPtr->actions[Ken_Kick].cleanupFunc = KickAction_GameplayCleanup;
	componentPtr->actions[Ken_Kick].priority = 10;


	//i == 5 Retract
	componentPtr->actions[Ken_Retract].duration = 100;
	componentPtr->actions[Ken_Retract].canStartFunc = Retract_CanStart;
	componentPtr->actions[Ken_Retract].gameplayOnStartFunc = Retract_GameplayOnStart;
	componentPtr->actions[Ken_Retract].gameplayFunc = Retract_GameplayController;
	componentPtr->actions[Ken_Retract].animationFunc = NULL/*WalkAction_AnimationController*/;
	componentPtr->actions[Ken_Retract].cleanupFunc = RetractAction_GameplayCleanup;
	componentPtr->actions[Ken_Retract].priority = 15;

	//i == 6 Stand hurt
	componentPtr->actions[Ken_StandHurt].duration = 1000;
	componentPtr->actions[Ken_StandHurt].canStartFunc = StandHurt_CanStart;
	componentPtr->actions[Ken_StandHurt].gameplayOnStartFunc = StandHurt_OnStart;
	componentPtr->actions[Ken_StandHurt].gameplayFunc = StandHurt_GameplayController;
	componentPtr->actions[Ken_StandHurt].cleanupFunc = StandHurt_GameplayCleanup;
	componentPtr->actions[Ken_StandHurt].animationFunc = StandHurt_AnimationController;
	componentPtr->actions[Ken_StandHurt].animationOnStartFunc = StandHurt_AnimOnStart;
	componentPtr->actions[Ken_StandHurt].priority = 20;

	componentPtr->actions[Ken_BasicHurt].duration = 500;
	componentPtr->actions[Ken_BasicHurt].canStartFunc = BasicHurt_CanStart;
	componentPtr->actions[Ken_BasicHurt].gameplayOnStartFunc = BasicHurt_OnStart;
	componentPtr->actions[Ken_BasicHurt].gameplayFunc = /*StandHurt_GameplayController*/NULL;
	componentPtr->actions[Ken_BasicHurt].cleanupFunc = BasicHurt_GameplayCleanup;
	componentPtr->actions[Ken_BasicHurt].animationFunc = NULL;
	componentPtr->actions[Ken_BasicHurt].animationOnStartFunc = BasicHurt_AnimOnStart;
	componentPtr->actions[Ken_BasicHurt].priority = 15;

	//i ==7
	componentPtr->actions[Ken_Block].duration = 1000;
	componentPtr->actions[Ken_Block].canStartFunc = StandBlock_CanStart;
	componentPtr->actions[Ken_Block].animationOnStartFunc = StandBlock_AnimOnStart;
	componentPtr->actions[Ken_Block].gameplayOnStartFunc = StandBlock_GameplayOnStart;
	componentPtr->actions[Ken_Block].priority = 10;

	//i ==8
	componentPtr->actions[Ken_BlockHurt].duration = 200;
	componentPtr->actions[Ken_BlockHurt].canStartFunc = BlockHurt_CanStart;
	componentPtr->actions[Ken_BlockHurt].gameplayFunc = BlockHurt_GameplayController;
	componentPtr->actions[Ken_BlockHurt].cleanupFunc = BlockHurt_GameplayCleanup;
	componentPtr->actions[Ken_BlockHurt].priority = 20;

	componentPtr->actions[Ken_Crouch].duration = 500;
	componentPtr->actions[Ken_Crouch].canStartFunc = Crouch_CanStart;
	componentPtr->actions[Ken_Crouch].gameplayOnStartFunc = Crouch_GameplayOnStart;
	componentPtr->actions[Ken_Crouch].gameplayFunc = Crouch_GameplayController;
	componentPtr->actions[Ken_Crouch].cleanupFunc = Crouch_GameplayCleanup;
	componentPtr->actions[Ken_Crouch].animationFunc = Crouch_AnimOnStart;
	componentPtr->actions[Ken_Crouch].priority = 8;

	componentPtr->actions[Ken_CrouchPunch].duration = 200;
	componentPtr->actions[Ken_CrouchPunch].canStartFunc = CrouchPunch_CanStart;
	componentPtr->actions[Ken_CrouchPunch].gameplayOnStartFunc = CrouchPunch_GameplayOnStart;
	componentPtr->actions[Ken_CrouchPunch].gameplayFunc = CrouchPunch_GameplayController;
	componentPtr->actions[Ken_CrouchPunch].cleanupFunc = CrouchPunch_GameplayCleanup;
	componentPtr->actions[Ken_CrouchPunch].animationOnStartFunc = CrouchPunch_AnimOnStart;
	componentPtr->actions[Ken_CrouchPunch].priority = 12;

	componentPtr->actions[Ken_CrouchKick].duration = 400;
	componentPtr->actions[Ken_CrouchKick].canStartFunc = CrouchKick_CanStart;
	componentPtr->actions[Ken_CrouchKick].gameplayOnStartFunc = CrouchKick_GameplayOnStart;
	componentPtr->actions[Ken_CrouchKick].gameplayFunc = CrouchKick_GameplayController;
	componentPtr->actions[Ken_CrouchKick].cleanupFunc = CrouchKick_GameplayCleanup;
	componentPtr->actions[Ken_CrouchKick].animationOnStartFunc = CrouchKick_AnimOnStart;
	componentPtr->actions[Ken_CrouchKick].priority = 12;

	componentPtr->actions[Ken_CrouchBlock].duration = 1000;
	componentPtr->actions[Ken_CrouchBlock].canStartFunc = CrouchBlock_CanStart;
	componentPtr->actions[Ken_CrouchBlock].gameplayOnStartFunc = CrouchBlock_GameplayOnStart;
	componentPtr->actions[Ken_CrouchBlock].animationOnStartFunc = CrouchBlock_AnimOnStart;
	componentPtr->actions[Ken_CrouchBlock].priority = 12;


	componentPtr->actions[Ken_Hadouken].duration = 2000;
	componentPtr->actions[Ken_Hadouken].canStartFunc = Hadouken_CanStart;
	componentPtr->actions[Ken_Hadouken].gameplayOnStartFunc = Hadouken_OnStart;
	componentPtr->actions[Ken_Hadouken].gameplayFunc = Hadouken_GameplayController;
	componentPtr->actions[Ken_Hadouken].cleanupFunc = Hadouken_GameplayCleanup;
	componentPtr->actions[Ken_Hadouken].animationFunc = Hadouken_AnimationController;
	componentPtr->actions[Ken_Hadouken].priority = 14;

	componentPtr->actions[Ken_StraightJump].duration = 1000;
	componentPtr->actions[Ken_StraightJump].canStartFunc = Jump_CanStart;
	componentPtr->actions[Ken_StraightJump].gameplayOnStartFunc = Jump_GameplayOnStart;
	componentPtr->actions[Ken_StraightJump].gameplayFunc = Jump_GameplayController;
	componentPtr->actions[Ken_StraightJump].animationOnStartFunc = Jump_AnimationOnStart;
	componentPtr->actions[Ken_StraightJump].priority = 11;
	
	componentPtr->actions[Ken_JumpPunch].duration = 1000;
	componentPtr->actions[Ken_JumpPunch].canStartFunc = JumpPunch_CanStart;
	componentPtr->actions[Ken_JumpPunch].gameplayOnStartFunc = JumpPunch_GameplayOnStart;
	componentPtr->actions[Ken_JumpPunch].gameplayFunc = JumpPunch_GameplayController;
	componentPtr->actions[Ken_JumpPunch].animationOnStartFunc = JumpPunch_AnimationOnStart;
	componentPtr->actions[Ken_JumpPunch].priority = 12;
	
	componentPtr->actions[Ken_JumpKick].duration = 1000;
	componentPtr->actions[Ken_JumpKick].canStartFunc = JumpKick_CanStart;
	componentPtr->actions[Ken_JumpKick].gameplayOnStartFunc = JumpKick_GameplayOnStart;
	componentPtr->actions[Ken_JumpKick].gameplayFunc = JumpKick_GameplayController;
	componentPtr->actions[Ken_JumpKick].animationOnStartFunc = JumpKick_AnimationOnStart;
	componentPtr->actions[Ken_JumpKick].priority = 12;

	componentPtr->actions[Ken_DirectionalJump].duration = 1000;
	componentPtr->actions[Ken_DirectionalJump].canStartFunc = DirectionalJump_CanStart;
	componentPtr->actions[Ken_DirectionalJump].gameplayFunc = DirectionalJump_GameplayController;
	componentPtr->actions[Ken_DirectionalJump].gameplayOnStartFunc = DirectionalJump_GameplayOnStart;
	componentPtr->actions[Ken_DirectionalJump].animationOnStartFunc = DirectionalJump_AnimationOnStart;
	componentPtr->actions[Ken_DirectionalJump].priority = 12;

	componentPtr->actions[Ken_DirectionalJumpPunch].duration = 1000;
	componentPtr->actions[Ken_DirectionalJumpPunch].canStartFunc = DirectionalJumpPunch_CanStart;
	componentPtr->actions[Ken_DirectionalJumpPunch].gameplayOnStartFunc = DirectionalJumpPunch_GameplayOnStart;
	componentPtr->actions[Ken_DirectionalJumpPunch].gameplayFunc = DirectionalJumpPunch_GameplayController;
	componentPtr->actions[Ken_DirectionalJumpPunch].animationOnStartFunc = DirectionalJumpPunch_AnimationOnStart;
	componentPtr->actions[Ken_DirectionalJumpPunch].priority = 13;

	componentPtr->actions[Ken_DirectionalJumpKick].duration = 1000;
	componentPtr->actions[Ken_DirectionalJumpKick].canStartFunc = DirectionalJumpKick_CanStart;
	componentPtr->actions[Ken_DirectionalJumpKick].gameplayOnStartFunc = DirectionalJumpKick_GameplayOnStart;
	componentPtr->actions[Ken_DirectionalJumpKick].gameplayFunc = DirectionalJumpKick_GameplayController;
	componentPtr->actions[Ken_DirectionalJumpKick].animationOnStartFunc = DirectionalJumpKick_AnimationOnStart;
	componentPtr->actions[Ken_DirectionalJumpKick].priority = 13;

	componentPtr->actions[Ken_Death].duration = 1000;
	componentPtr->actions[Ken_Death].canStartFunc = DeathBehaviour_CanStart;
	componentPtr->actions[Ken_Death].gameplayOnStartFunc = DeathBehaviour_OnStart;
	componentPtr->actions[Ken_Death].animationOnStartFunc = DeathBehaviour_AnimationOnStart;
	componentPtr->actions[Ken_Death].priority = 100;

	componentPtr->actions[Ken_Victory].duration = 1000;
	componentPtr->actions[Ken_Victory].canStartFunc = VictoryBehaviour_CanStart;
	componentPtr->actions[Ken_Victory].gameplayOnStartFunc = VictoryBehaviour_OnStart;
	componentPtr->actions[Ken_Victory].animationOnStartFunc = VictoryBehaviour_AnimationOnStart;
	componentPtr->actions[Ken_Victory].priority = 100;
}

bool Player2_WalkAction_CanStart(Object* object, uint32_t milliseconds);
bool Player2_PunchAction_CanStart(Object* object, uint32_t milliseconds);
void Player2_WalkAction_OnStart(Object* obj, uint32_t milliseconds);
bool Player2_KickAction_CanStart(Object* object, uint32_t milliseconds);
bool Player2_StandBlock_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_Crouch_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_CrouchPunch_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_CrouchKick_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_CrouchBlock_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_Hadouken_CanStart(Object* obj, uint32_t milliseconds);
void Player2_Hadouken_OnStart(Object* obj, uint32_t milliseconds);
bool Player2_DirectionalJump_CanStart(Object* obj, uint32_t milliseconds);
void Player2_DirectionalJump_GameplayOnStart(Object* obj, uint32_t milliseconds);
bool Player2_Jump_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_JumpPunch_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_JumpKick_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_DirectionalJumpPunch_CanStart(Object* obj, uint32_t milliseconds);
bool Player2_DirectionalJumpKick_CanStart(Object* obj, uint32_t milliseconds);
void Player2_DirectionalJumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds);
void Player2_DirectionalJumpKick_GameplayOnStart(Object* obj, uint32_t milliseconds);

void InitRiyuGameplayComponent(Object* obj)
{
	assert(obj != NULL);
	InitKenGameplayComponent(obj);
	Character* character = (Character*)obj;
	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	gameplayComponentPtr->actions[Riyu_Idle].duration = 1000;
	gameplayComponentPtr->actions[Riyu_Walk].canStartFunc = Player2_WalkAction_CanStart;
	gameplayComponentPtr->actions[Riyu_Walk].gameplayOnStartFunc = Player2_WalkAction_OnStart;
	gameplayComponentPtr->actions[Riyu_Punch].canStartFunc = Player2_PunchAction_CanStart;
	gameplayComponentPtr->actions[Riyu_Kick].canStartFunc = Player2_KickAction_CanStart;
	gameplayComponentPtr->actions[Riyu_Block].canStartFunc = Player2_StandBlock_CanStart;
	gameplayComponentPtr->actions[Riyu_Crouch].canStartFunc = Player2_Crouch_CanStart;
	gameplayComponentPtr->actions[Riyu_CrouchPunch].canStartFunc = Player2_CrouchPunch_CanStart;
	gameplayComponentPtr->actions[Riyu_CrouchKick].canStartFunc = Player2_CrouchKick_CanStart;
	gameplayComponentPtr->actions[Riyu_CrouchBlock].canStartFunc = Player2_CrouchBlock_CanStart;
	gameplayComponentPtr->actions[Riyu_Hadouken].canStartFunc = Player2_Hadouken_CanStart;
	gameplayComponentPtr->actions[Riyu_Hadouken].gameplayOnStartFunc = Player2_Hadouken_OnStart;
	gameplayComponentPtr->actions[Riyu_StraightJump].canStartFunc = Player2_Jump_CanStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJump].canStartFunc = Player2_DirectionalJump_CanStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJump].gameplayOnStartFunc = Player2_DirectionalJump_GameplayOnStart;
	gameplayComponentPtr->actions[Riyu_JumpPunch].canStartFunc = Player2_JumpPunch_CanStart;
	gameplayComponentPtr->actions[Riyu_JumpKick].canStartFunc = Player2_JumpKick_CanStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJumpPunch].canStartFunc = Player2_DirectionalJumpPunch_CanStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJumpPunch].gameplayOnStartFunc = Player2_DirectionalJumpPunch_GameplayOnStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJumpKick].canStartFunc = Player2_DirectionalJumpKick_CanStart;
	gameplayComponentPtr->actions[Riyu_DirectionalJumpKick].gameplayOnStartFunc = Player2_DirectionalJumpKick_GameplayOnStart;
}

bool Player2_WalkAction_CanStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	if (currentStatePtr->subState == Jump)
	{
		return false;
	}
	if (inputKeyPressed(0x44)) //D is pressed
	{
		return true;
	}
	else if (inputKeyPressed(0x41)) //A is pressed
	{
		return true;
	}
	return false;
}

void Player2_WalkAction_OnStart(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	CharacterGameplayState WalkState = { None,Stand };
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	TryChangeGameplayState(character, WalkState);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);

	if (inputKeyPressed(0x44)) //Direction key is pressed - d
	{
		//This code should be moved to onStart

		*characterDirectionPtr = Facing_Right;
	}
	else if (inputKeyPressed(0x41)) //A is pressed
	{
		*characterDirectionPtr = Facing_Left;
	}
	else
	{
	}
}

bool Player2_PunchAction_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_F)) //Key F is pressed
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

bool Player2_KickAction_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_G)) //G
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

bool Player2_StandBlock_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_6))
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

bool Player2_Crouch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_S) == true) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_Crouch].payload == NULL)
		{
			return true;
		}

	}
	return false;
}

bool Player2_CrouchPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_S) && inputKeyPressed(VK_F)/*Key P is pressed*/)
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

bool Player2_CrouchKick_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_S) && inputKeyPressed(VK_G)/*Key S and G is pressed*/)
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

bool Player2_CrouchBlock_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_S) && inputKeyPressed(VK_6)/*Key S and G is pressed*/)
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

bool Player2_Hadouken_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_S) && (inputKeyPressed(VK_D) || inputKeyPressed(VK_A)) && inputKeyPressed(VK_F))
	{
		Character* character = (Character*)obj;
		GameplayComponent* gameplayComponent = GetCharacterGameplayComponent(character);
		if (gameplayComponent->currentAction.action == &gameplayComponent->actions[Riyu_Hadouken])
		{
			return false;
		}
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
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

extern PlayHadoukenSound _hadoukenSoundCB;
void Player2_Hadouken_OnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(characterDirectionPtr != NULL);
	CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
	assert(currentStatePtr != NULL);
	CharacterGameplayState attackState = { Attack,Stand };
	if (!TryChangeGameplayState(character, attackState))
	{
		return;
	}
	AttackComponent* attackComponent = GetCharacterAttackComponent(character);
	attackComponent->isActive = false;

	if (inputKeyPressed(VK_D))
	{
		*characterDirectionPtr = Facing_Right;
	}
	else if (inputKeyPressed(VK_A))
	{
		*characterDirectionPtr = Facing_Left;
	}
	else
	{
		//Strange case - dunno what to do here
	}

	HadoukenPayload* payload = malloc(sizeof(HadoukenPayload));
	assert(payload != NULL);
	payload->isFired = false;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	assert(gameplayComponentPtr != NULL);
	TryDeInitActionPayload(gameplayComponentPtr, Riyu_Hadouken); //if existing payload delete that
	InitActionPayload(gameplayComponentPtr, Riyu_Hadouken, (Payload*)payload);
	if (_hadoukenSoundCB != NULL)
	{
		_hadoukenSoundCB(character);
	}
}

bool Player2_DirectionalJump_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true
		&& (inputKeyPressed(VK_D) == true || inputKeyPressed(VK_A) == true)
		&& (!inputKeyPressed(VK_F) && !inputKeyPressed(VK_G))) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump) //if character is already jumping then dont trigger this behaviour
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Riyu_DirectionalJump].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

void Player2_DirectionalJump_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(gameplayComponentPtr != NULL);

	if (inputKeyPressed(VK_D) == true)
	{
		*characterDirectionPtr = Facing_Right;
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}
}

bool Player2_Jump_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Riyu_StraightJump].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

bool Player2_JumpPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true && inputKeyPressed(VK_F)
		&& (!inputKeyPressed(VK_D) && !inputKeyPressed(VK_A)))
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Riyu_JumpPunch].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

bool Player2_JumpKick_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true && inputKeyPressed(VK_G) 
		&& (!inputKeyPressed(VK_D) && !inputKeyPressed(VK_A)))
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_JumpKick].payload == NULL)
		{
			return true;
		}
	}
	return false;
}

bool Player2_DirectionalJumpPunch_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true && (inputKeyPressed(VK_D) == true || inputKeyPressed(VK_A) == true)
		&& inputKeyPressed(VK_F)) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump)
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Riyu_DirectionalJumpPunch].payload == NULL)
		{
			return true;
		}
	}
	return false;
}
void Player2_DirectionalJumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(gameplayComponentPtr != NULL);

	if (inputKeyPressed(VK_D) == true)
	{
		*characterDirectionPtr = Facing_Right;
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}
}

bool Player2_DirectionalJumpKick_CanStart(Object* obj, uint32_t milliseconds)
{
	if (inputKeyPressed(VK_W) == true && (inputKeyPressed(VK_D) == true || inputKeyPressed(VK_A) == true)
		&& inputKeyPressed(VK_G)) //S is pressed
	{
		Character* character = (Character*)obj;
		CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
		if (currentStatePtr->subState == Jump) //If already in air we dont want to activate this behaviour otherwise we can have stacking of jumping behaviours. Can be a feature but comes with its own set of challenges if not handled well can make the game look distasterful
		{
			return false;
		}
		GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
		assert(gameplayComponentPtr != NULL);
		if (gameplayComponentPtr->actions[Ken_DirectionalJumpKick].payload == NULL)
		{
			return true;
		}
	}
	return false;
}


void Player2_DirectionalJumpKick_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;

	GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
	CharacterGameplayState jumpState = { None,Jump };
	TryChangeGameplayState(character, jumpState);
	FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
	assert(gameplayComponentPtr != NULL);

	if (inputKeyPressed(VK_D) == true)
	{
		*characterDirectionPtr = Facing_Right;
	}
	else
	{
		*characterDirectionPtr = Facing_Left;
	}
}