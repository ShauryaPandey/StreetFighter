#pragma once
#include "animation.h"
#include "gameplaycomponent.h"
#include "gameplaystatemanager.h"
#include "Object.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CharacterStandHeight 200
#define CharacterStandWidth  100
#define CharacterCrouchHeight (0.67f*CharacterStandHeight)
#define KenSpriteSheetWidth 632
#define KenSpriteSheetHeight 1040
#define RyuSpriteSheetWidth 640
#define RyuSpriteSheetHeight 1200

	typedef struct character_t Character;

	typedef void (*PlayHadoukenSound)(Character* character);

	typedef int AttackPriority;
	static const AttackPriority lowPriority = 1; //basis this it is decided which attack triumphs should both attack each other at the same time and at valid impact points.
	static const AttackPriority mediumPriority = 2;
	static const AttackPriority highPriority = 3;

	/// <summary>
	/// Every character having the capability of attacking should own this. This has information about what is the impact point where it attacked, and what is the priority of the attack.
	/// </summary>
	typedef struct attackComponent_t
	{
		AttackPriority priority;
		Coord2D impactPoint;
		bool isActive;
	}AttackComponent;

	AttackComponent* GetCharacterAttackComponent(Character* character);

	typedef enum defenseRange_t
	{
		Defense_Full,
		Defense_Upper
	}DefenseRange;
	/// <summary>
	/// A character capable of defending should have this. This tells information about what is the current range
	/// in which the character is defending so that if the enemy attack has an impact point in that range this character can block it.
	/// </summary>
	typedef struct defenseComponent_t
	{
		bool isActive;
		DefenseRange range;
	}DefenseComponent;
	DefenseComponent* GetCharacterDefenseComponent(Character* character);
	void characterInitTextures();

	typedef void (*GameSpawnHadoukenFunc)(Character* character, Coord2D position, float velocityX);
	typedef bool (*GameCanSpawnHadoukenFunc)(Character* character);
	typedef struct healthBar_t HealthBar;
	char* GetCharacterName(Character* character);
	Character* characterNew(Bounds2D box, Coord2D* position,FacingDirection direction, const char* nameStr, GameSpawnHadoukenFunc spawnHadoukenFunc, GameCanSpawnHadoukenFunc canSpawnHadoukenFunc,HealthBar* bar);
	void characterDelete(Character* face);
	void UpdateCharacterSize(Character* character, CharacterGameplaySubState subState);
	AnimationComponent* GetCharacterAnimationComponent(Character* character);
	GameplayComponent* GetCharacterGameplayComponent(Character* character);
	CharacterGameplayState* GetCharacterGameplayState(Character* character);
	FacingDirection* GetCharacterFacingDirection(Character* character);
	void ModifyCharacterHealth(Character* character, uint8_t delta);
	bool IsCharacterDead(Character* character);
	bool isCharacterWon(Character* character);
	void CharacterWon(Character* character);
	void CharacterDead(Character* character);
	void characterReset(Character* character, float xPos, FacingDirection direction);
	void TrySpawnHadouken(Character* character, Coord2D position, float velocityX);
	bool CanSpawnHadouken(Character* character);
	static void _processImpactInfo(Object* obj);
	uint8_t GetCharacterHealth(Character* character);

	//Ken
	void _characterOnCollisionEnter(Object* thisObj, Object* otherObj);
	//Riyu

#ifdef __cplusplus
}
#endif