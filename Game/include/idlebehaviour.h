#pragma once
#include "Object.h"
bool IdleAction_CanStart(Object* object, uint32_t milliseconds);
void IdleAction_OnStart(Object* obj, uint32_t milliseconds);
void IdleAction_AnimationController(Object* obj, uint32_t milliseconds);