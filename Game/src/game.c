#include "baseTypes.h"
#include "input.h"
#include "application.h"
#include "framework.h"

#include "levelmgr.h"
#include "objmgr.h"
#include <stdio.h>
#include <assert.h>

/// <summary>
/// Function declarations and typedefs and extern global var declarations.
/// </summary>
static void _gameInit();
static void _gameShutdown();
static void _gameDraw();
static void _gameUpdate(uint32_t milliseconds);
void backgroundDraw();
void StartScreenDraw();
void endScreenShow(char* victorName);
void RoundDisplayDraw(uint32_t roundNumber);
typedef struct healthBar_t HealthBar;
extern HealthBar* gHealth1;
extern HealthBar* gHealth2;
extern PlayStartScreenSoundCB g_startScreenCB;
extern StopStartScreenSoundCB g_stopStartScreenCB;

/// <summary>
/// Defining bounds and other information about the level. Not made by me, I inherited this from the base game.
/// </summary>
static LevelDef _levelDefs[] = {
	{
		{{0, 0}, {1024, 448}},		// fieldBounds
		0x00ff0000,					// fieldColor
		20,							// numBalls
		10							// numFaces
	}
};
static Level* _curLevel = NULL;
/// <summary>
/// This acts like a state machine, helping maintain the state of the game.
/// </summary>
typedef enum gameState_t
{
	Game_Starting,
	Game_Running,
	Game_RoundChange,
	Game_Finished,
	Game_Count,
	Game_Invalid = -1
}GameState;
/// <summary>
/// This is like a state machine that keeps track of which state the game is in. 
/// Its data is used to make decisions about transitioning.
/// </summary>
typedef struct gameStateManager_t
{
	GameState gameState;
	uint32_t time;
}GameStateManager;

static uint32_t roundDisplayTime = 2000;
static uint32_t roundStartsInTime = 1000;
static GameStateManager gameStateManager;

/// @brief Program Entry Point (WinMain). Part of the base game framework.
/// @param hInstance 
/// @param hPrevInstance 
/// @param lpCmdLine 
/// @param nCmdShow 
/// @return 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char GAME_NAME[] = "Framework1";

	Application* app = appNew(hInstance, GAME_NAME, _gameDraw, _gameUpdate);

	if (app != NULL)
	{
		GLWindow* window = fwInitWindow(app);
		if (window != NULL)
		{
			_gameInit();

			bool running = true;
			while (running)
			{
				printf("Hello");
				running = fwUpdateWindow(window);
			}

			_gameShutdown();
			fwShutdownWindow(window);
		}

		appDelete(app);
	}
}

/// @brief Initialize code to run at application startup. Part of the base game framework.
static void _gameInit()
{
	const uint32_t MAX_OBJECTS = 500;
	objMgrInit(MAX_OBJECTS);
	levelMgrInit();
	gameStateManager.gameState = Game_Starting;
	gameStateManager.time = 0;
	_curLevel = levelMgrLoad(&_levelDefs[0]);
}

/// @brief Cleanup the game and free up any allocated resources. Part of the base game framework.
static void _gameShutdown()
{
	levelMgrUnload(_curLevel);

	levelMgrShutdown();
	objMgrShutdown();
}

/// @brief Draw everything to the screen for current frame
void _healthBarDraw(/*HealthBar* bar*/);
static void _gameDraw() 
{
	if (gameStateManager.gameState == Game_Starting)
	{
		StartScreenDraw();
	}
	else if(gameStateManager.gameState == Game_Running)
	{
		backgroundDraw();
		_healthBarDraw(gHealth1);
		_healthBarDraw(gHealth2);
		objMgrDraw();
		drawTimer(GetRoundTimer(_curLevel));
	}
	else if (gameStateManager.gameState == Game_RoundChange)
	{
		RoundDisplayDraw(GetCurrentRound(_curLevel));
	}
	else if (gameStateManager.gameState == Game_Finished)
	{
		//Display finish screen
		endScreenShow(TryGetVictorName(_curLevel));
		drawTimer(GetExitTimer(_curLevel));
	}
}

/// @brief Perform updates for all game objects, for the elapsed duration
/// @param milliseconds 
static void _gameUpdate(uint32_t milliseconds)
{
	// ESC exits the program
	if (inputKeyPressed(VK_ESCAPE))
	{
		// TODO 
		//TerminateApplication(window);
	}

	// F1 toggles fullscreen
	if (inputKeyPressed(VK_F1))
	{
		// TODO 
		//ToggleFullscreen(window);
	}
	if (gameStateManager.gameState == Game_Starting)
	{
		if (g_startScreenCB != NULL)
		{
			g_startScreenCB();
		}
		if (inputKeyPressed(VK_SPACE))
		{
			gameStateManager.gameState = Game_RoundChange;
			if (g_stopStartScreenCB != NULL)
			{
				g_stopStartScreenCB();
			}
		}
	}
	if (gameStateManager.gameState == Game_RoundChange) //we transition to next round
	{
		gameStateManager.time += milliseconds;
		if (gameStateManager.time > roundDisplayTime)
		{
			gameStateManager.time = 0;
			gameStateManager.gameState = Game_Running;
			StartCurrentRound(_curLevel); //This is required for setthing the current round variable for the first time when game starts
		}
	}
	if (gameStateManager.gameState == Game_Running)
	{
		objMgrUpdate(milliseconds);
		updateTimer(GetRoundTimer(_curLevel),milliseconds);
		TryCompleteRound(_curLevel); //Sets the variable which says round over

		if (IsRoundOver(_curLevel))
		{
			gameStateManager.time += milliseconds; //wait for some time before switching
			if (gameStateManager.time < roundStartsInTime)
			{
				return;
			}
			gameStateManager.time = 0;

			if (IsGameOver(_curLevel)) //Is there a victor?
			{

				gameStateManager.gameState = Game_Finished;
			}
			else //no? then prepare for next round
			{
				resetLevel(_curLevel); //reset state and prepare for next level also sets that variable which says new round starting
				gameStateManager.gameState = Game_RoundChange;
			}
		}
	}
	if (gameStateManager.gameState == Game_Finished)
	{
		updateTimer(GetExitTimer(_curLevel), milliseconds);
		if (isTimerOver(GetExitTimer(_curLevel)))
		{
			resetGame(_curLevel);
			gameStateManager.gameState = Game_Starting;
		}
	}
}

typedef struct hadouken_t Hadouken;

/// <summary>
/// This should not free any memory.
/// This callback should be with the hadouken object
/// </summary>
/// <param name="hadouken"></param>
void GameDeInitActiveHadouken(Hadouken* hadouken)
{
	//Is this present in the list of active hadoukens?
	uint8_t numActiveHadoukens = 0;
	ActiveHadouken* activeHadoukens = GetActiveHadoukens(_curLevel, &numActiveHadoukens);
	assert(numActiveHadoukens == 2);
	int idx = -1;
	for (int i = 0; i < numActiveHadoukens; i++)
	{
		if (activeHadoukens[i].hadouken == hadouken)
		{
			idx = i;
			break;
		}
	}
	//If not assert
	assert(idx != -1);
	assert(idx < numActiveHadoukens);
	activeHadoukens[idx].hadouken = NULL;
	activeHadoukens[idx].owner = NULL;
	//null to active hadouken fields
}

typedef struct character_t Character;

/// <summary>
/// This callback should be with the character object which it will use to spawn
/// hadouken in the correpsonding behaviour. 
/// </summary>
/// <param name="character"></param>
/// <param name="position"></param>
/// <param name="velocityX"></param>
void GameSpawnHadouken(Character* character, Coord2D position, float velocityX)
{
	uint8_t numActiveHadoukens = 0;
	ActiveHadouken* activeHadoukens = GetActiveHadoukens(_curLevel, &numActiveHadoukens);
	assert(numActiveHadoukens == 2);
	int idx = -1;
	//Is the owner already owning a hadouken ?
	for (int i = 0; i < numActiveHadoukens; i++)
	{
		if (activeHadoukens[i].owner == NULL)
		{
			assert(activeHadoukens[i].hadouken == NULL); //If this assert hits that means there is a ligering hadouken somehow
			idx = i;
		}

		if (activeHadoukens[i].owner == character)
		{
			assert(activeHadoukens[i].hadouken != NULL);//It should have an associated hadouken
			return; //Cant spawn
		}
	}
	assert(idx != -1);
	assert(idx < numActiveHadoukens);

	Hadouken* hadouken = CreateActiveHadouken(_curLevel, idx, character,GameDeInitActiveHadouken, position, velocityX); // we pass the function pointer of deinit hadouken to the hadouken object

	activeHadoukens[idx].owner = character;
	activeHadoukens[idx].hadouken = hadouken;
}

/// <summary>
/// In the behaviour the character must check if there is an active hadouken in which case it can not spawn one more.
/// </summary>
/// <param name="character"></param>
/// <returns></returns>
bool GameCanSpawnHadouken(Character* character)
{
	uint8_t numActiveHadoukens = 0;
	ActiveHadouken* activeHadoukens = GetActiveHadoukens(_curLevel, &numActiveHadoukens);
	assert(numActiveHadoukens == 2);
	int idx = -1;

	for (int i = 0; i < numActiveHadoukens; i++)
	{
		if (activeHadoukens[i].owner == character)
		{
			assert(activeHadoukens[i].hadouken != NULL);//It should have an associated hadouken which means it can not spawn
			return false; //Cant spawn
		}
	}
	return true;
}