#pragma once
#include "Object.h"
bool CrouchPunch_CanStart(Object* obj, uint32_t milliseconds);
void CrouchPunch_GameplayOnStart(Object* obj, uint32_t milliseconds);
void CrouchPunch_AnimOnStart(Object* obj, uint32_t milliseconds);
void CrouchPunch_GameplayController(Object* obj, uint32_t milliseconds);
void CrouchPunch_GameplayCleanup(Object* obj, uint32_t milliseconds);