#pragma once
#include "Object.h"
bool WalkAction_CanStart(Object* object, uint32_t milliseconds);
void WalkAction_OnStart(Object* obj, uint32_t milliseconds);
void WalkAction_GameplayController(Object* obj, uint32_t milliseconds);
void WalkAction_AnimationController(Object* obj, uint32_t milliseconds);
