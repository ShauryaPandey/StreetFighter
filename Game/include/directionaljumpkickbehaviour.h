#pragma once
#include "Object.h"
bool DirectionalJumpKick_CanStart(Object* obj, uint32_t milliseconds);
void DirectionalJumpKick_GameplayOnStart(Object* obj, uint32_t milliseconds);
void DirectionalJumpKick_GameplayController(Object* obj, uint32_t milliseconds);
void DirectionalJumpKick_AnimationOnStart(Object* obj, uint32_t milliseconds);
