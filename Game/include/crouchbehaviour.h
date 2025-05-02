#pragma once
#include "Object.h"
bool Crouch_CanStart(Object* obj, uint32_t milliseconds);
void Crouch_GameplayOnStart(Object* obj, uint32_t milliseconds);
void Crouch_GameplayController(Object* obj, uint32_t milliseconds);
void Crouch_AnimOnStart(Object* obj, uint32_t milliseconds);
void Crouch_GameplayCleanup(Object* obj, uint32_t milliseconds);