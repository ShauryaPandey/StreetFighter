#pragma once
#include "Object.h"
bool JumpKick_CanStart(Object* obj, uint32_t milliseconds);
void JumpKick_GameplayOnStart(Object* obj, uint32_t milliseconds);
void JumpKick_GameplayController(Object* obj, uint32_t milliseconds);
void JumpKick_AnimationOnStart(Object* obj, uint32_t milliseconds);