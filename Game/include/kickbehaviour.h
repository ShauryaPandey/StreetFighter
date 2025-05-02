#pragma once
#include "Object.h"
bool KickAction_CanStart(Object* obj, uint32_t milliseconds);
void KickAction_OnStart(Object* obj, uint32_t milliseconds);
void KickAction_GameplayController(Object* obj, uint32_t milliseconds);
void KickAction_GameplayCleanup(Object* obj, uint32_t milliseconds);
void KickAction_AnimationController(Object* obj, uint32_t milliseconds);
void Kick_AnimOnStart(Object* obj, uint32_t milliseconds);
