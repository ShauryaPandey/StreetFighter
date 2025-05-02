#pragma once
#include "baseTypes.h"
#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

void objMgrInit(uint32_t maxObjects);
void objMgrShutdown();
void objMgrAdd(Object* obj);
void objMgrRemove(Object* obj);

void objMgrDraw();
void objMgrUpdate(uint32_t milliseconds);

#ifdef __cplusplus
}
#endif