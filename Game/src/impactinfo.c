#include <stdlib.h>
#include "impactinfo.h"
#include <assert.h>

/// <summary>
/// Initializes the impact info component. On all characters.
/// </summary>
/// <param name="info"></param>
void ImpactInfoInit(ImpactInfo* info)
{
	assert(info != NULL);
	info->other = NULL;
	info->newState = NULL;
}

/// <summary>
/// Has the value of the impact value changed.
/// </summary>
/// <param name="info"></param>
/// <returns></returns>
bool IsImpactInfoDirty(ImpactInfo* info)
{
	assert(info != NULL);
	if (info->other != NULL)
	{
		if (info->newState != NULL)
		{
			return true;
		}
		assert(false); // this means there is an other collider but not a new state
	}
	return false;
}

void ImpactInfoDeInit(ImpactInfo* info)
{
	assert(info != NULL);
	if (info->other != NULL)
	{
		//Dont free this as the object is managed by object manager
		info->other = NULL;
	}
	if (info->newState != NULL)
	{
		free(info->newState);
		info->newState = NULL;
	}
}

void ImpactInfoSet(ImpactInfo* info, CharacterGameplayBaseState baseState, CharacterGameplaySubState subState, Object* otherObj)
{
	assert(info != NULL);
	assert(otherObj != NULL);
	CharacterGameplayState* newState = malloc(sizeof(CharacterGameplayState)); //free this malloc in the obj delete
	assert(newState != NULL);
	newState->baseState = baseState;
	newState->subState = subState;
	info->newState = newState;
	info->other = otherObj;
}