#include <Windows.h>
#include <stdlib.h>
#include <gl/GLU.h>
#include <assert.h>
#include "baseTypes.h"
#include "levelmgr.h"
#include "field.h"
#include "ball.h"
#include "face.h"
#include "character.h"
#include "hadouken.h"
#include "objmgr.h"
#include "SOIL.h"
#include "sound.h"
#include "animation.h"
#include "basichurtbehaviour.h"
#include "standhurtbehaviour.h"
#include "kenbehaviours.h"
#include "healthbar.h"
#include <string.h>
#include "stdio.h"

/// <summary>
/// Extern and static global data.
/// </summary>
HealthBar* gHealth1; //These are the 2 health bars
HealthBar* gHealth2;
//Some declarations
extern PlayBigDamageSoundCB g_BigDamageSoundCB;
extern PlayPunchSoundCB g_punchSoundCB;
extern PlayHadoukenSound _hadoukenSoundCB;
extern PlayStartScreenSoundCB g_startScreenCB;
extern StopStartScreenSoundCB g_stopStartScreenCB;
PlayFightSoundCB g_fightSoundCB;
PlayRoundSoundCB g_roundSoundCB;

float g_groundLevel; //global var for ground level - referenced by other files too
static bool isRoundSoundPlaying = false;
static bool isTimerDownSoundPlaying = false;
static bool startScreenSoundPlaying = false;

void GameSpawnHadouken(Character* character, Coord2D position, float velocityX);
bool GameCanSpawnHadouken(Character* character);

static void _levelMgrTimerStartSound();

/// <summary>
/// Timer for the round. Updated every frame - based on this the round gets over
/// </summary>
typedef struct roundTimer_t
{
    float roundTime;
    Coord2D position;
    uint8_t size;
}RoundTimer;

/// <summary>
/// Initialized the timer we use to time the rounds.
/// </summary>
/// <param name="timer"></param>
void InitRoundTimer(RoundTimer* timer)
{
    assert(timer != NULL);
    Coord2D position = { 500,100 };
    timer->position = position;
    timer->size = TimerSize;
}

/// <summary>
/// The exit timer is played when the match(set of rounds like tennis) is over and we need to go back to the main screen.
/// </summary>
/// <param name="timer"></param>
void InitExitTimer(RoundTimer* timer)
{
	assert(timer != NULL);
	Coord2D position = { 700,100 };
	timer->position = position;
	timer->size = TimerSize;
}

/// <summary>
/// Resets it to the max time.
/// </summary>
/// <param name="timer"></param>
void resetTimer(RoundTimer* timer)
{
    timer->roundTime = TimerMaxTime;
    isTimerDownSoundPlaying = false;
}

void resetExitTimer(RoundTimer* timer)
{
    timer->roundTime = 10;
}

/// <summary>
/// Updates every frame the timer.
/// </summary>
/// <param name="timer"></param>
/// <param name="milliseconds"></param>
void updateTimer(RoundTimer* timer, uint8_t milliseconds)
{
    float secondsPassedInFrame = (float)milliseconds / 1000.0f;
    timer->roundTime -= secondsPassedInFrame; //Updating time left
    if (timer->roundTime < 0)
    {
        timer->roundTime = 0;
    }
    if ((int)timer->roundTime == 10)
    {
  //the intention was to start audio saying 10,9, 8 and so on but the clip doesnt seem comptaible with my timer frequency
    }
}

/// <summary>
/// Draw timer on screen.
/// </summary>
/// <param name="timer"></param>
void drawTimer(RoundTimer* timer)
{
    char timeInChar[5];
    snprintf(timeInChar, 10, "%d", (int)timer->roundTime);
    PrintWord(timeInChar,5,timer->position.x,timer->position.y,timer->size);
}

bool isTimerOver(RoundTimer* timer)
{
    if ((int)timer->roundTime == 0)
    {
        return true;
    }
    return false;
}

typedef struct level_t
{
    const LevelDef* def;
    RoundTimer timer;
    RoundTimer exitTimer; //The timer that we use once we have a victor then to return to the main screen and reset the game
    Field* field;
    Character* character;
    Character* secondCharacter;
    HealthBar* bar[2];
    ActiveHadouken activeHadoukens[2]; //this struct keeps track of active hadoukens in the scene and who is the owner of that hadouken
    uint8_t currentRound; //what is the current round number
    uint8_t char1_wins; //number of wins by character 1 so far
    uint8_t char2_wins;
    bool roundStarted;
} Level;

//Sounds
static int32_t _hadoukenSoundID = SOUND_NOSOUND;
static int32_t _backgroundSoundID = SOUND_NOSOUND;
static int32_t _fightSoundID = SOUND_NOSOUND;
static int32_t _roundSoundID = SOUND_NOSOUND;
static int32_t _timerDownSoundID = SOUND_NOSOUND;
static int32_t _punchSoundID = SOUND_NOSOUND;
static int32_t _bigAttackSoundID = SOUND_NOSOUND;
static int32_t _soundId = SOUND_NOSOUND;

ActiveHadouken* GetActiveHadoukens(Level* lvl, uint8_t* len)
{
    *len = 2;//no need but keeping design open to more than 2
    return lvl->activeHadoukens;
}
//Returns the timer that runs during the round
RoundTimer* GetRoundTimer(Level* lvl)
{
    assert(lvl != NULL);
    return &lvl->timer;
}

/// <summary>
/// Timer for the game exit after which game loads again from start screen.
/// </summary>
/// <param name="lvl"></param>
/// <returns></returns>
RoundTimer* GetExitTimer(Level* lvl)
{
    assert(lvl != NULL);
    return &lvl->exitTimer;
}
//Function declarations for all the sound playing functions so that they can be referenced in level mgr Init
static void _levelMgrPlaySound(Ball* ball);
static void _levelMgrPlayHadoukenSound(Character* character);
static void _levelMgrPlayStartScreenSound();
static void _levelMgrStopStartScreenSound();
static void _levelMgrPlayFightSound();
static void _levelMgrStartRoundSound();
static void _levelMgrStopRoundSound();
static void _levelMgrTimerStartSound();
static void _levelMgrPlayPunchSound();
static void _levelMgrPlayBigAttackSound();
void backgroundInitTexture();

/// @brief Initialize the level manager
void levelMgrInit()
{
    backgroundInitTexture();
    faceInitTextures();
    characterInitTextures();
    hadoukenInitTextures();
    gHealth1 = initHealthbar(200,100);
    gHealth2 = initHealthbar(800,100);
    _soundId = soundLoad("asset/beep.wav");
    _hadoukenSoundID = soundLoad("asset/hadouken-sound.wav");
    _backgroundSoundID = soundLoad("asset/street-fighter.wav");
    _fightSoundID = soundLoad("asset/fight.wav");
    _roundSoundID = soundLoad("asset/roundMusic2.wav");
    _timerDownSoundID = soundLoad("asset/street_mixdown.wav");
    _punchSoundID = soundLoad("asset/2AH.wav");
    _bigAttackSoundID = soundLoad("asset/33H.wav");
    _hadoukenSoundCB = _levelMgrPlayHadoukenSound;
    g_startScreenCB = _levelMgrPlayStartScreenSound;
    g_stopStartScreenCB = _levelMgrStopStartScreenSound;
    g_fightSoundCB = _levelMgrPlayFightSound;
    g_punchSoundCB = _levelMgrPlayPunchSound;
    g_BigDamageSoundCB = _levelMgrPlayBigAttackSound;
    ballSetCollideCB(_levelMgrPlaySound);
}

/// @brief Shutdown the level manager
void levelMgrShutdown()
{
    //ballClearCollideCB();
    soundUnload(_soundId);
}

/// @brief Loads the level and all required objects/assets
/// @param levelDef 
/// Inits all the components in the level such as the 2 characters, health bars, timer.
/// @return 
Level* levelMgrLoad(const LevelDef* levelDef)
{
    Level* level = malloc(sizeof(Level));
    if (level != NULL)
    {
        level->currentRound = 1;
        level->roundStarted = false;
        level->char1_wins = 0;
        level->char2_wins = 0;
        level->def = levelDef;
        InitRoundTimer(&level->timer);
        InitRoundTimer(&level->exitTimer);
        resetTimer(&level->timer);
        resetExitTimer(&level->exitTimer);
        g_groundLevel = boundsGetCenter(&levelDef->fieldBounds).y + 200;


        // the field provides the boundaries of the scene & encloses the faces & balls
        level->field = fieldNew(levelDef->fieldBounds, levelDef->fieldColor);

        level->activeHadoukens[0].owner = NULL;
        level->activeHadoukens[0].hadouken = NULL;
        level->activeHadoukens[1].owner = NULL;
        level->activeHadoukens[1].hadouken = NULL;
        //Init a character
        char ken[] = "Ken";
        Coord2D* firstCharPosition = malloc(sizeof(Coord2D));
        assert(firstCharPosition != NULL);
        firstCharPosition->x = kenPosX;
        firstCharPosition->y = g_groundLevel;
        level->character = characterNew(levelDef->fieldBounds, firstCharPosition, Facing_Left,ken, GameSpawnHadouken, GameCanSpawnHadouken,gHealth2);
        free(firstCharPosition); //Freeing the position memory
        InitKenAnimations(GetCharacterAnimationComponent(level->character));
        InitKenGameplayComponent((Object*)level->character);

        //Init second character
        char ryu[] = "Ryu";
        Coord2D* secondCharPosition = malloc(sizeof(Coord2D));
        assert(secondCharPosition != NULL);
        secondCharPosition->x = RyuPosX;
        secondCharPosition->y = g_groundLevel;
        
        level->secondCharacter = characterNew(levelDef->fieldBounds,secondCharPosition,Facing_Right,ryu,GameSpawnHadouken, GameCanSpawnHadouken,gHealth1);
		free(secondCharPosition);
        secondCharPosition = NULL;
        InitRyuAnimations(GetCharacterAnimationComponent(level->secondCharacter));
        InitRiyuGameplayComponent((Object*)level->secondCharacter);

    }
    return level;
}

/// @brief Unloads the level and frees up any assets associated
/// @param level 
void levelMgrUnload(Level* level)
{
    if (level != NULL) 
    {
        DeInitAnimations(GetCharacterAnimationComponent(level->character));
        DeInitAnimations(GetCharacterAnimationComponent(level->secondCharacter));
        GameplayComponent_DeInit(GetCharacterGameplayComponent(level->character));
        GameplayComponent_DeInit(GetCharacterGameplayComponent(level->secondCharacter));
        characterDelete(level->character);
        characterDelete(level->secondCharacter);
        if (level->activeHadoukens[0].hadouken != NULL) //so if unloaded level while a hadouken was in action, it wont get self deleted
        {
            //Maybe the game ended with a active hadouken
            hadoukenDelete(level->activeHadoukens[0].hadouken);
            level->activeHadoukens[0].hadouken = NULL;
        }
        if (level->activeHadoukens[1].hadouken != NULL)
        {
            hadoukenDelete(level->activeHadoukens[1].hadouken);
            level->activeHadoukens[1].hadouken = NULL;
        }
        
        fieldDelete(level->field);
    }
    free(level);
}

/// <summary>
/// Who won whats their  name?
/// </summary>
/// <param name="lvl"></param>
/// <returns></returns>
char* TryGetVictorName(Level* lvl)
{
    if (lvl->char1_wins > lvl->char2_wins)
    {
        return GetCharacterName(lvl->character);
    }
    else
    {
        return GetCharacterName(lvl->secondCharacter);
    }
}

/// <summary>
/// When starting new round we want to reinitialize some of the components such as in the characters.
/// This function is called then.
/// </summary>
/// <param name="level"></param>
void resetLevel(Level* level)
{
	if (level != NULL)
	{
        level->roundStarted = true;
        characterReset(level->character, kenPosX,Facing_Left);
        characterReset(level->secondCharacter, RyuPosX,Facing_Right);
        resetHealthBar(gHealth1);
        resetHealthBar(gHealth2); //set it back to 100
	}
}

/// <summary>
/// When the game ends we do this to restart the game.
/// </summary>
/// <param name="level"></param>
void resetGame(Level* level)
{
	if (level != NULL)
	{
		level->roundStarted = true;
		characterReset(level->character, kenPosX,Facing_Left);
		characterReset(level->secondCharacter, RyuPosX,Facing_Right);
		resetHealthBar(gHealth1);
		resetHealthBar(gHealth2); //set it back to 100
        level->currentRound = 1;
        level->char1_wins = 0;
        level->char2_wins = 0;
        resetExitTimer(&level->exitTimer);
	}
}

static void _levelMgrPlaySound(Ball* ball)
{
    soundPlay(_soundId);
}
//Used in hadouken behaviour when releasing the energy ball
static void _levelMgrPlayHadoukenSound(Character* character)
{
    soundPlay(_hadoukenSoundID);
}
//Not using now
static void _levelMgrTimerStartSound()
{
    assert(_timerDownSoundID != SOUND_NOSOUND);
    if (isTimerDownSoundPlaying == false)
    {
        soundPlay(_timerDownSoundID);
        isTimerDownSoundPlaying = true;
    }
}

static void _levelMgrPlayStartScreenSound()
{
    if (startScreenSoundPlaying == false)
    {
        assert(_backgroundSoundID != SOUND_NOSOUND);
        soundPlay(_backgroundSoundID);
        startScreenSoundPlaying = true;
    }
}

static void _levelMgrStopStartScreenSound()
{
    assert(_backgroundSoundID != SOUND_NOSOUND);
    soundStop(_backgroundSoundID); //Make sure to stop the previously running background sound
    startScreenSoundPlaying = false;
}

static void _levelMgrStartRoundSound()
{
    if (isRoundSoundPlaying == false)
    {
        assert(_roundSoundID != SOUND_NOSOUND);
        soundPlay(_roundSoundID);
        isRoundSoundPlaying = true;
    }
}

static void _levelMgrStopRoundSound()
{
    assert(_roundSoundID != SOUND_NOSOUND);
    soundStop(_roundSoundID);
    isRoundSoundPlaying = false;
}

static void _levelMgrPlayFightSound()
{
    assert(_fightSoundID != SOUND_NOSOUND);
    soundPlay(_fightSoundID);
}

static void _levelMgrPlayPunchSound()
{
    assert(_punchSoundID != SOUND_NOSOUND);
    soundPlay(_punchSoundID);
}

static void _levelMgrPlayBigAttackSound()
{
    assert(_bigAttackSoundID != SOUND_NOSOUND);
    soundPlay(_bigAttackSoundID);
}

/// <summary>
/// Character holds reference to this as func pointer, they create a new hadouken in their hadouken behaviour.
/// </summary>
/// <param name="lvl"></param>
/// <param name="idx"></param>
/// <param name="character"></param>
/// <param name="notifyOwnerFunc"></param>
/// <param name="position"></param>
/// <param name="velocityX"></param>
/// <returns></returns>
Hadouken* CreateActiveHadouken(Level* lvl, int idx, Character* character, hadoukenNotifyOwnerFunc notifyOwnerFunc, Coord2D position, float velocityX)
{
    lvl->activeHadoukens[idx].owner = character;
    Hadouken* hadouken = hadoukenNew(lvl->def->fieldBounds, &position, notifyOwnerFunc, velocityX); //notifyownerFunc is a functionpointer to be called when hadouken is destroyed wither on collision or exiting bounds, it must notify the owner that is no more there
    AnimationComponent* hadoukenAnimComponent =  GetHadoukenAnimationComponent(hadouken);
    assert(hadoukenAnimComponent != NULL);
    InitHadoukenAnimations(hadoukenAnimComponent);
    lvl->activeHadoukens[idx].hadouken = hadouken;
    return hadouken;
}

/// <summary>
/// This function checks if the round is completed - either a character won or if the timer got over.
/// </summary>
/// <param name="lvl"></param>
/// <returns></returns>
bool TryCompleteRound(Level* lvl)
{
    if (lvl->roundStarted == false)
    {
        return false;
    }
    if (IsCharacterDead(lvl->character))
    {
        //second character wins
        CharacterWon(lvl->secondCharacter);
        lvl->currentRound++;
        lvl->char2_wins++;
        lvl->roundStarted = false;
        _levelMgrStopRoundSound();
        return true;
    }
    if (IsCharacterDead(lvl->secondCharacter))
    {
        //first character wins
        CharacterWon(lvl->character);
        lvl->currentRound++;
        lvl->char1_wins++;
        lvl->roundStarted = false;
        _levelMgrStopRoundSound();
        return true;
    }
    if (isTimerOver(GetRoundTimer(lvl))) //if time got over we must check who has higher health - they win
    {
        uint8_t healthChar1 = GetCharacterHealth(lvl->character);
        uint8_t healthChar2 = GetCharacterHealth(lvl->secondCharacter);
        if (healthChar1 < healthChar2)
        {
            CharacterWon(lvl->secondCharacter);
            CharacterDead(lvl->character);
            lvl->char2_wins++;
        }
        else if(healthChar1> healthChar2)
        {
            CharacterWon(lvl->character);
            CharacterDead(lvl->secondCharacter);
            lvl->char1_wins++;
        }
        else
        {
            CharacterDead(lvl->character);
            CharacterDead(lvl->secondCharacter);
            //In this case both lost - no victor
        }
        lvl->roundStarted = false;
        lvl->currentRound++;
        _levelMgrStopRoundSound();
        return true; //round is over
    }
    return false;
}

/// <summary>
/// Checking after each round if we have a victor. We are doing whoever wins 2 first.
/// </summary>
/// <param name="lvl"></param>
/// <returns></returns>
bool IsGameOver(Level* lvl)
{
    if (lvl->char1_wins >= 2 || lvl->char2_wins >= 2)
    {
        return true;
    }
    return false;
}

uint8_t GetCurrentRound(Level* lvl)
{
    return lvl->currentRound;
}

/// <summary>
/// When we want to start the next round.
/// </summary>
/// <param name="lvl"></param>
void StartCurrentRound(Level* lvl) 
{
    lvl->roundStarted = true;
    if (g_fightSoundCB != NULL)
    {
        g_fightSoundCB();
        _levelMgrStartRoundSound(); //Sound that plays in the background during the round
    }
    resetTimer(&lvl->timer); //Timer needs to re-begin from 30 or whatever we set the timer duration to be
}

bool IsRoundOver(Level* lvl)
{
    return !lvl->roundStarted;
}