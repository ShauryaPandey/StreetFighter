#pragma once
#include "Object.h"
bool BlockHurt_CanStart(Object* obj, uint32_t milliseconds);
void BlockHurt_GameplayOnStart(Object* obj, uint32_t milliseconds);
void BlockHurt_GameplayController(Object* obj, uint32_t milliseconds);
void BlockHurt_GameplayCleanup(Object* obj, uint32_t milliseconds);
