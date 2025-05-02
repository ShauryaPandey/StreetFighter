#pragma once
#include "baseTypes.h"
//#include "collider.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum objectType_t
    {
        Type_Character,
        Type_Hadouken,
        Type_Count,
        Type_Invalid = -1
    }ObjectType;

// object "virtual" functions
typedef struct object_t Object;
typedef struct collider_t Collider;
typedef void (*ObjDrawFunc)(Object*);
typedef void (*ObjUpdateFunc)(Object*, uint32_t);

typedef struct object_vtable_t {
    ObjDrawFunc     draw;
    ObjUpdateFunc   update;
} ObjVtable;

typedef struct object_t {
    ObjVtable*      vtable;
    Collider*       collider;
    ObjectType      type;
    Bounds2D        levelDef;
    Coord2D         position;
    Coord2D         velocity;
    //Collider*       collider;
    Coord2D         size;

} Object;

typedef void (*ObjRegistrationFunc)(Object*);

// class-wide registration methods
void objEnableRegistration(ObjRegistrationFunc registerFunc, ObjRegistrationFunc deregisterFunc);
void objDisableRegistration();

// object API
void objInit(Object* obj, ObjVtable* vtable,Collider* collider, Bounds2D levelDef, Coord2D pos, Coord2D vel);
void objDeinit(Object* obj);
void objDraw(Object* obj);
void objUpdate(Object* obj, uint32_t milliseconds);

void ObjChangeSizeRatio(Object* obj, float xratio, float yratio);
void ObjChangeSizeAbs(Object* obj, float newX, float newY);
void SetObjectTransform(Object* obj, float newX, float newY);
void MoveObjectTransform(Object* obj, float deltaX, float deltaY);

extern float g_groundLevel;
void MoveObjectToGround(Object* obj);

bool IsObjectOnGround(Object* obj);

// default update implementation that just moves at the current velocity
void objDefaultUpdate(Object* obj, uint32_t milliseconds);
void CheckAllCollisions(Object** list, int max);
#ifdef __cplusplus
}
#endif