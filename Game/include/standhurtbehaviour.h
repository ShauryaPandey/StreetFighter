#pragma once
#include "Object.h"
bool StandHurt_CanStart(Object* obj, uint32_t milliseconds);
void StandHurt_OnStart(Object* obj, uint32_t milliseconds);
void StandHurt_GameplayController(Object* obj, uint32_t milliseconds);
void StandHurt_GameplayCleanup(Object* obj, uint32_t milliseconds);
void StandHurt_AnimationController(Object* obj, uint32_t milliseconds);
void StandHurt_AnimOnStart(Object* obj, uint32_t milliseconds);

typedef void (*PlayBigDamageSoundCB)();