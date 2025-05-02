#pragma once
#include "Object.h"
bool JumpPunch_CanStart(Object* obj, uint32_t milliseconds);
void JumpPunch_GameplayOnStart(Object* obj, uint32_t milliseconds);
void JumpPunch_GameplayController(Object* obj, uint32_t milliseconds);
void JumpPunch_AnimationOnStart(Object* obj, uint32_t milliseconds);