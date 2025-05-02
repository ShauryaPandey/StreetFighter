#pragma once
#include "Object.h"
bool Retract_CanStart(Object* obj, uint32_t milliseconds);
void Retract_GameplayOnStart(Object* obj, uint32_t milliseconds);
void Retract_GameplayController(Object* obj, uint32_t milliseconds);
void RetractAction_GameplayCleanup(Object* obj, uint32_t milliseconds);