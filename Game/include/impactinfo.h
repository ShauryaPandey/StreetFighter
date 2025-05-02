#include "Object.h"
#include "gameplaystatemanager.h"

/// <summary>
/// Meant to hold data that is sufficient to process impact of collision.
/// </summary>
typedef struct impactInfo_t
{
	Object* other;
	CharacterGameplayState* newState;

}ImpactInfo;

void ImpactInfoInit(ImpactInfo* info);
bool IsImpactInfoDirty(ImpactInfo* info);
void ImpactInfoDeInit(ImpactInfo* info);
void ImpactInfoSet(ImpactInfo* info, CharacterGameplayBaseState baseState, CharacterGameplaySubState subState, Object* otherObj);