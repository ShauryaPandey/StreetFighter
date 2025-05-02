#pragma once
#include "Object.h"

bool CrouchKick_CanStart(Object* obj, uint32_t milliseconds);
void CrouchKick_GameplayOnStart(Object* obj, uint32_t milliseconds);
void CrouchKick_GameplayController(Object* obj, uint32_t milliseconds);
void CrouchKick_AnimOnStart(Object* obj, uint32_t milliseconds);
void CrouchKick_GameplayCleanup(Object* obj, uint32_t milliseconds);