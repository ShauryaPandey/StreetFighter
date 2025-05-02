#include "gameplaycomponent.h"
#include "character.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>
#include "SOIL.h"

#include "baseTypes.h"
#include "face.h"
#include "Object.h"
#include "random.h"

/// <summary>
/// This is the function/system where current running action on the gameplay component is executed.
/// Other actions in the list are checked and best possible action's priority is measured against 
/// current running action to see if the current action can be interrupted.
/// </summary>
/// <param name="component"></param>
/// <param name="object"></param>
/// <param name="milliseconds"></param>
void GameplayController_Update(GameplayComponent* component, Object* object, uint32_t milliseconds)
{
	//execute current action
	if (component->currentAction.action != NULL)
	{
		if (component->currentAction.action->status == Starting) //If the action has an associated onStartFunc run them for gameplay and animation
		{
			if (component->currentAction.action->gameplayOnStartFunc != NULL) //if it has a start func run it
			{
				component->currentAction.action->gameplayOnStartFunc(object, milliseconds);
			}
			if (component->currentAction.action->animationOnStartFunc != NULL)
			{
				component->currentAction.action->animationOnStartFunc(object, milliseconds);
			}
			component->currentAction.action->status = Running; //After starting phase is done move to running phase
		}
		//Update every frame
		if (component->currentAction.action->status == Running) //As long as phase is running ececute the gameplay and anim controller funcs
		{
			if (component->currentAction.action->gameplayFunc != NULL)
			{
				component->currentAction.action->gameplayFunc(object, milliseconds);
			}
			if (component->currentAction.action->animationFunc != NULL)
			{
				component->currentAction.action->animationFunc(object, milliseconds);
			}
		}
		//update remaining time of current action
		component->currentAction.remainingTime -= milliseconds;

		TryRemoveCurrentAction(component, object, milliseconds); //If the current action's time is over it needs to be removed from current action
	}

	//Is there a new action ready to go
	//Which new behaviour  is the most suited to start
	Action* newAction = NULL;
	for (int i = 0; i < component->len; i++)
	{
		//assert(component->actions[i].canStartFunc != NULL);
		if (component->actions[i].canStartFunc == NULL) //dont execute this action/behaviour
		{
			continue;
		}
		if (component->actions[i].canStartFunc(object, milliseconds))
		{
			if (newAction == NULL)
			{
				newAction = &component->actions[i];
			}
			else
			{
				if (component->actions[i].priority > newAction->priority) //if true this one should be the newaction
				{
					newAction = &component->actions[i];
				}
			}
		}
	}
	
	//is new action there?
	if (newAction != NULL)
	{
		if (component->currentAction.action == NULL)
		{
			UpdateCurrentAction(component, newAction); //if no current action then make new action as current action
		}
		else
		{
			if (newAction->priority > component->currentAction.action->priority) //othereise compare priority
			{
				ForceRemoveCurrentAction(component, object, milliseconds); //FOrcibly stop the current action
				//update new current action
				UpdateCurrentAction(component, newAction);
			}
		}
	}
	//if no current action, update
	//else check priority over current action
}


/// <summary>
/// When initializing the component this should be used to init every field to starting value which is 0.
/// </summary>
/// <param name="component"></param>
void GameplayComponent_Init(GameplayComponent* component)
{
	assert(component != NULL);

	component->actions = NULL;
	component->len = 0;
	component->currentAction.action = NULL;
	component->currentAction.remainingTime = 0;
	component->payload.position.x = 0;
	component->payload.position.y = 0;
	component->payload.size.x = 0;
	component->payload.size.y = 0;
}

/// <summary>
/// Freeing up storage from inside the gameplay component 
/// </summary>
/// <param name="component"></param>
void GameplayComponent_DeInit(GameplayComponent* component)
{
	assert(component != NULL);
	for (int i = 0; i < component->len; i++)
	{
		if (component->actions[i].payload != NULL)
		{
			//If we did it right this means this actually has a payload that we must free
			free(component->actions[i].payload);
		}
	}
	free(component->actions);
}

void RetractPayload_init(Payload* payload)
{
	RetractPayload* retractPayload = (RetractPayload*)payload;

	retractPayload->other = NULL;
}

/// <summary>
/// Once actions have been malloced all those actions need to reset to some default state, this function does that. 
/// </summary>
/// <param name="component"></param>
void ActionsInit(GameplayComponent* component)
{
	assert(component != NULL);
	int n = component->len;
	assert(component->actions != NULL);
	for (int i = 0; i < n; i++)
	{
		component->actions[i].animationFunc = NULL;
		component->actions[i].canStartFunc = NULL;
		component->actions[i].gameplayOnStartFunc = NULL;
		component->actions[i].animationOnStartFunc = NULL;
		component->actions[i].cleanupFunc = NULL;
		component->actions[i].gameplayFunc = NULL;
		component->actions[i].payload = NULL;
		component->actions[i].priority = 0;
		component->actions[i].duration = 0;
		component->actions[i].status = Finished;
	}
}

/// <summary>
///When attaching a payload to an action this function gets called. 
/// </summary>
/// <param name="component"></param>
/// <param name="actionID"></param>
/// <param name="actionPayload"></param>
void InitActionPayload(GameplayComponent* component, KenGameplayActions actionID, Payload* actionPayload)
{
	if (actionID > Ken_Invalid && actionID < Ken_Count)
	{
		assert(actionPayload != NULL);
		assert(component != NULL);
		assert(component->actions != NULL);
		assert(component->actions[actionID].payload == NULL); //there is an existing payload and we are looking to init a new one?
		component->actions[actionID].payload = actionPayload;
	}
}

/// <summary>
/// When action is over or needs to be restarted or stopped previously allocated payload is handled here.
/// </summary>
/// <param name="component"></param>
/// <param name="actionID"></param>
void DeInitActionPayload(GameplayComponent* component, KenGameplayActions actionID)
{
	if (actionID > Ken_Invalid && actionID < Ken_Count)
	{
		assert(component != NULL);
		assert(component->actions != NULL);
		assert(component->actions[actionID].payload != NULL); //there is no payload then what do we remove?
		free(component->actions[actionID].payload);
		component->actions[actionID].payload = NULL;
	}
}

/// <summary>
/// Difference from above function is that this tries to deinit if there is none 
/// it just gently exits whereas the above function asserts.
/// </summary>
/// <param name="component"></param>
/// <param name="actionID"></param>
void TryDeInitActionPayload(GameplayComponent* component, KenGameplayActions actionID)
{
	if (actionID > Ken_Invalid && actionID < Ken_Count)
	{
		assert(component != NULL);
		assert(component->actions != NULL);
		if (component->actions[actionID].payload != NULL) //already has a payload
		{
			// Free that one first
			free(component->actions[actionID].payload);
			component->actions[actionID].payload = NULL;
		}
	}
}