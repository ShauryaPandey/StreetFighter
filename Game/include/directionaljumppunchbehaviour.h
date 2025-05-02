#pragma once
#include "Object.h"
bool DirectionalJumpPunch_CanStart(Object* obj, uint32_t milliseconds);
void DirectionalJumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds);
void DirectionalJumpPunch_GameplayController(Object* obj, uint32_t milliseconds);
void DirectionalJumpPunch_AnimationOnStart(Object* obj, uint32_t milliseconds);