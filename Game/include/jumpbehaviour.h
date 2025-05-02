#pragma once
#include "Object.h"
bool Jump_CanStart(Object* obj, uint32_t milliseconds);
void Jump_GameplayOnStart(Object* obj, uint32_t milliseconds);
void Jump_GameplayController(Object* obj, uint32_t milliseconds);
void Jump_AnimationOnStart(Object* obj, uint32_t milliseconds);