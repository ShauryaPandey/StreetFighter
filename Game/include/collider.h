#pragma once
#include "baseTypes.h"
#include <stdlib.h>

typedef struct object_t Object;;
typedef void (*OnCollisionEnter)(Object* thisObj, Object* otherObj);
typedef void (*ProcessImpact)(Object* thisObject);
/// <summary>
/// The information about the other object which has collided with this collider.
/// </summary>
typedef struct collisionInfo_t
{
	Object* other;

}CollisionInfo;

/// <summary>
/// The collider vtable.
/// </summary>
typedef struct collider_vtable_t
{
	OnCollisionEnter onCollisionEnterFunc; //Function the derived class must provide implementation for. This is what you want your object to do on collision. Dont make changes to your object's state. Put that info in impactInfo
	ProcessImpact processImpact; //This function must also be provided by the derived class, any lasting changes to the object's state must be done here.
} ColliderVtable;
/// <summary>
/// An object that wants to participate in collisions must own a collider. 
/// It holds information about the collision if it happens and a vtable for supporting polymorphisms with box and circle colliders.
/// </summary>
typedef struct collider_t
{
	CollisionInfo info;
	ColliderVtable* vtable;
}Collider;

void Collider_OnCollisionEnter(Object* thisObj, Object* otherObj);
void Collider_ProcessImpact(Object* obj);