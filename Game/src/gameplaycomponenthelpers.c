#include "gameplaycomponent.h"
#include "baseTypes.h"
#include "Object.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>

/// <summary>
/// When you want to put a new action as current action.
/// </summary>
/// <param name="component"></param>
/// <param name="action"></param>
void UpdateCurrentAction(GameplayComponent* component, Action* action)
{
	assert(component != NULL);
	assert(action != NULL);
	assert(component->currentAction.action == NULL); //we should not override without first removing
	component->currentAction.action = action;
	component->currentAction.remainingTime = action->duration;
	component->currentAction.action->status = Starting; //starting phase

}
/// <summary>
/// Tries to remove the current action if it has finished.
/// </summary>
/// <param name="component"></param>
/// <param name="object"></param>
/// <param name="milliseconds"></param>
void TryRemoveCurrentAction(GameplayComponent* component, Object* object, uint32_t milliseconds) //basically checks if action time is finished
{
	//CHECK If action has finished
	if (component->currentAction.remainingTime <= 0)
	{
		assert(component->currentAction.action != NULL);

		if (component->currentAction.action->cleanupFunc != NULL)
		{
			component->currentAction.action->cleanupFunc(object, milliseconds);
		}
		component->currentAction.action->status = Finished;
		component->currentAction.action = NULL;
		component->currentAction.remainingTime = 0;
	}
}
/// <summary>
/// Calls the cleanup function and removes it. 
/// </summary>
/// <param name="component"></param>
/// <param name="object"></param>
/// <param name="milliseconds"></param>
void ForceRemoveCurrentAction(GameplayComponent* component, Object* object, uint32_t milliseconds)
{
	if (component->currentAction.action->cleanupFunc != NULL)
	{
		component->currentAction.action->cleanupFunc(object, milliseconds); //to make sure it gracefully exits and leaves the correct state for the next action
	}

	component->currentAction.action->status = Blocked;
	component->currentAction.action = NULL;
	component->currentAction.remainingTime = 0;
}
