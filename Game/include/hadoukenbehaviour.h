#pragma once
#include "Object.h"
bool Hadouken_CanStart(Object* obj, uint32_t milliseconds);
void Hadouken_OnStart(Object* obj, uint32_t milliseconds);
void Hadouken_GameplayController(Object* obj, uint32_t milliseconds);
void Hadouken_GameplayCleanup(Object* obj, uint32_t milliseconds);
void Hadouken_AnimationController(Object* obj, uint32_t milliseconds);