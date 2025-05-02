#pragma once
#include <stdbool.h>
#include "Object.h"
#include "collider.h"
#include "baseTypes.h"

#define KenMaxActions 7
#define RetractShift  0.1

/// <summary>
/// This enum helps keep track of the animation state Ken is currently in. 
/// </summary>
typedef enum kenGameplayActions_t
{
	Ken_Punch = 0,
	Ken_Walk = 1,
	Ken_Idle = 2,
	Ken_Kick = 3,
	Ken_JumpKick = 4,
	Ken_Retract = 5,
	Ken_StandHurt = 6,
	Ken_BasicHurt = 7,
	Ken_Block = 8,
	Ken_BlockHurt = 9,
	Ken_Crouch = 10,
	Ken_CrouchPunch = 11,
	Ken_CrouchKick = 12,
	Ken_CrouchBlock = 13,
	Ken_Hadouken = 14,
	Ken_StraightJump = 15,
	Ken_DirectionalJump = 16,
	Ken_JumpPunch = 17,
	Ken_DirectionalJumpPunch = 18,
	Ken_DirectionalJumpKick = 19,
	Ken_Death = 20,
	Ken_Victory = 21,
	Ken_Count,
	Ken_Invalid = -1
}KenGameplayActions;

/// <summary>
/// This enum helps keep track the animation state Ryu is in.
/// </summary>
typedef enum RiyuGameplayActions_t
{
	Riyu_Punch = 0,
	Riyu_Walk = 1,
	Riyu_Idle = 2,
	Riyu_Kick = 3,
	Riyu_JumpKick = 4,
	Riyu_Retract = 5,
	Riyu_StandHurt = 6,
	Riyu_BasicHurt = 7,
	Riyu_Block = 8,
	Riyu_BlockHurt = 9,
	Riyu_Crouch = 10,
	Riyu_CrouchPunch = 11,
	Riyu_CrouchKick = 12,
	Riyu_CrouchBlock = 13,
	Riyu_Hadouken = 14,
	Riyu_StraightJump=15,
	Riyu_DirectionalJump = 16,
	Riyu_JumpPunch = 17,
	Riyu_DirectionalJumpPunch = 18,
	Riyu_DirectionalJumpKick = 19,
	Riyu_Death = 20,
	Riyu_Victory = 21,
	Riyu_Count,
	Riyu_Invalid = -1
}RiyuGameplayActions;

typedef enum facingDirection_t
{
	Facing_Right,
	Facing_Left
}FacingDirection;

typedef enum actionStatus_t
{
	Blocked,
	Running,
	Starting,
	Finished
} ActionStatus;
typedef struct gameplayComponent_t GameplayComponent;


/// <summary>
/// Each action will have the following funcs, can it start? What is the first thing it should do once it starts,
/// What should it do every frame, any first thing to do from animation pov, any continuous update changes to animation,
/// and when finishing action do you want something done? 
/// </summary>
typedef void (*GameplayLogic)(Object* object, uint32_t milliseconds); // Gameplay function pointer type
typedef void (*GameplayOnStart)(Object* object, uint32_t milliseconds); // Gameplay function pointer type
typedef void (*AnimationLogic)(Object* character, uint32_t milliseconds);
typedef void (*AnimationOnStart)(Object* character, uint32_t milliseconds);
typedef void (*GameplayCleanup)(Object* object, uint32_t milliseconds);
typedef bool (*CanStart)(Object* object, uint32_t milliseconds);

/// <summary>
/// Might need to recover to old state or store some variables between function calls for the controller
/// Should each action have a payload or can it be per character? I made a design decision to let each action have a payload.
/// </summary>
typedef struct payload_t
{
	Coord2D position;
	Coord2D size;
	
}Payload; //An action can have a payload which is information action has during its execution that is needed for its execution.

/// <summary>
/// Payload for retract action.
/// </summary>
typedef struct retractPayload_t
{
	Payload super;
	Object* other;
}RetractPayload;

/// <summary>
/// Payload for jump kick action.
/// </summary>
typedef struct jumpKickPayload_t
{
	float y;
}JumpKickPayload;

/// <summary>
/// Payload for stand hurt action.
/// </summary>
typedef struct standHurtPayload_t
{
   //Add parent payload
	Payload super;
	float y;
	FacingDirection Originaldirection;
	Coord2D otherPosition;
}StandHurtPayload;
/// <summary>
/// Payload for hadouken action.
/// </summary>
typedef struct hadoukenPayload_t
{
	Payload super;
	bool isFired;
}HadoukenPayload;
/// <summary>
/// Paylod for hurt action.
/// </summary>
typedef struct hurtPayload_t
{
	Payload super;
	FacingDirection Originaldirection;
	Coord2D otherPosition;
}HurtPayload;
/// <summary>
/// You can think of an action as a base behaviour or as the actual thing that runs the functionality. 
/// Gameplay behaviors and animations can be plugged into actions.
/// </summary>
typedef struct action_t
{
	uint32_t priority; //using this switching or rather interruption happens
	uint32_t duration; //how many seconds this behaviour will run
	GameplayOnStart gameplayOnStartFunc;
	GameplayLogic gameplayFunc;
	AnimationLogic animationFunc;
	AnimationOnStart animationOnStartFunc;
	CanStart canStartFunc;
	GameplayCleanup cleanupFunc;
	Payload* payload; //runtime information to action
	ActionStatus status; //what is the state the behaviour/action is in currently
}Action;

/// <summary>
/// Holds the currently running action/behaviour and tracks time remaining. 
/// </summary>
typedef struct currentAction_t
{
	Action* action;
	int remainingTime;
}CurrentAction;

/// <summary>
/// Not used in current application but this looks like a possibility if I want to expand the game.
/// </summary>
typedef struct canStartActionsQueue_t
{
	Action* queue;
	int len;
	int currIndex;
}CanStartActionsQueue;

/// <summary>
/// Every character that wants to execute actions must own this component.
/// </summary>
typedef struct gameplayComponent_t
{
	Action* actions;
	Payload payload; //not needed
	int len;
	int canStartLen;
	CurrentAction currentAction;
}GameplayComponent;

void UpdateCurrentAction(GameplayComponent* component, Action* action);
void TryRemoveCurrentAction(GameplayComponent* component, Object* object, uint32_t milliseconds);
void ForceRemoveCurrentAction(GameplayComponent* component, Object* object, uint32_t milliseconds);

void GameplayController_Update(GameplayComponent* component, Object* object, uint32_t milliseconds);
void GameplayComponent_Init(GameplayComponent* component);
void GameplayComponent_DeInit(GameplayComponent* component);

//Retract payload
void RetractPayload_init(Payload* payload);
void ActionsInit(GameplayComponent* component);

//Action payload
void InitActionPayload(GameplayComponent* component, KenGameplayActions actionID, Payload* actionPayload);
void DeInitActionPayload(GameplayComponent* component, KenGameplayActions actionID);
void TryDeInitActionPayload(GameplayComponent* component, KenGameplayActions actionID);