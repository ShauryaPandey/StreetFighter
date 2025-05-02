#pragma once
#include "Object.h"

bool PunchAction_CanStart(Object* object, uint32_t milliseconds);
void PunchAction_OnStart(Object* obj, uint32_t milliseconds);
void PunchAction_GameplayController(Object* obj, uint32_t milliseconds);
void PunchAction_GameplayCleanup(Object* obj, uint32_t milliseconds);
void PunchAction_AnimationController(Object* obj, uint32_t milliseconds);
void Punch_AnimOnStart(Object* obj, uint32_t milliseconds);