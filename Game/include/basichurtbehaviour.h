#pragma once
#include "object.h"
bool BasicHurt_CanStart(Object* obj, uint32_t milliseconds);
void BasicHurt_OnStart(Object* obj, uint32_t milliseconds);
void BasicHurt_AnimOnStart(Object* obj, uint32_t milliseconds);
void BasicHurt_GameplayCleanup(Object* obj, uint32_t milliseconds);

typedef void (*PlayPunchSoundCB)();
