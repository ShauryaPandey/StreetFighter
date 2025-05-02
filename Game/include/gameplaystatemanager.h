#pragma once

/// <summary>
/// A combination of base state and substate should be able to describe what the character is doing at any time. 
/// </summary>
typedef enum characterGameplayBaseState_t
{
	Attack,
	Block,
	Retract,
	Hurt,
	Dead,
	BlockHurt,
	None
}CharacterGameplayBaseState;

/// <summary>
/// Sub state that in conjunction with the base state conveys an exact picture of what the character is doing.
/// </summary>
typedef enum characterGameplaySubState_t
{
	Jump,
	Crouch,
	Stand
}CharacterGameplaySubState;

/// <summary>
/// This struct contains data that tells what the state of the character is by 
/// using CharacterGameplayBaseState and CharacterGameplaySubState combination.
/// </summary>
typedef struct characterGameplayState_t
{
	CharacterGameplayBaseState baseState;
	CharacterGameplaySubState subState;
} CharacterGameplayState;
typedef struct character_t Character;
bool TryChangeGameplayState(Character* character, CharacterGameplayState newState);