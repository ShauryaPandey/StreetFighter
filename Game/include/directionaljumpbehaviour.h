#pragma once
#include "Object.h"
bool DirectionalJump_CanStart(Object* obj, uint32_t milliseconds);
void DirectionalJump_GameplayOnStart(Object* obj, uint32_t milliseconds);
void DirectionalJump_GameplayController(Object* obj, uint32_t milliseconds);
void DirectionalJump_AnimationOnStart(Object* obj, uint32_t milliseconds);