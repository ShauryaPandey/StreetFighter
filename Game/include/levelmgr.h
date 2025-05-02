#pragma once
#include "baseTypes.h"
#include "hadouken.h"

#ifdef __cplusplus
extern "C" {
#endif

#define groundOffset 400
#define kenPosX 500
#define RyuPosX 200
#define TimerMaxTime 40
#define TimerSize 25
typedef struct leveldef_t {
    Bounds2D fieldBounds;
    uint32_t fieldColor;
    uint32_t numBalls;
    uint32_t numFaces;
} LevelDef;

typedef struct hadouken_t Hadouken;
typedef struct character_t Character;
typedef void (*PlayStartScreenSoundCB)();
typedef void (*StopStartScreenSoundCB)();
typedef void (*PlayFightSoundCB)();
typedef void (*PlayRoundSoundCB)();
/// <summary>
/// Hadoukens can be spawned by the characters in the game, once one is spawned this struct is used to track real time information about it.
/// </summary>
typedef struct activeHadouken_t //Onl holds addresses not responsible for creating or freeing any memory
{
	Character* owner;
	Hadouken* hadouken;
}ActiveHadouken;

typedef struct level_t Level;

void levelMgrInit();
void levelMgrShutdown();
Level *levelMgrLoad(const LevelDef* levelDef);
void levelMgrUnload(Level* level);
ActiveHadouken* GetActiveHadoukens(Level* lvl, uint8_t* len);
Hadouken* CreateActiveHadouken(Level* lvl, int idx, Character* character, hadoukenNotifyOwnerFunc notifyOwnerFunc, Coord2D position, float velocityX);
bool TryCompleteRound(Level* lvl);
uint8_t GetCurrentRound(Level* lvl);
bool IsGameOver(Level* lvl);
void resetLevel(Level* level);
void resetGame(Level* level);
void StartCurrentRound(Level* lvl);
bool IsRoundOver(Level* lvl);
void PrintWord(char* str, int len, float startX, float startY, float letterSize);
char* TryGetVictorName(Level* lvl);

typedef struct roundTimer_t RoundTimer;
RoundTimer* GetRoundTimer(Level* lvl);
RoundTimer* GetExitTimer(Level* lvl);
bool isTimerOver(RoundTimer* timer);
void drawTimer(RoundTimer* timer);
void updateTimer(RoundTimer* timer, uint8_t milliseconds);
void resetTimer(RoundTimer* timer);
void InitRoundTimer(RoundTimer* timer);
void InitExitTimer(RoundTimer* timer);
void resetExitTimer(RoundTimer* timer);
#ifdef __cplusplus
}
#endif