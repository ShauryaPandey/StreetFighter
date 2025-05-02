#include "collider.h"
#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"
#include "random.h"
#include "field.h"
#include "Object.h"
//Possible subtypes for cases should the subtype of collider determine nature of collsision. Not in use right now.
typedef struct boxCollider_t
{
	Collider super;
	Bounds2D box;

}BoxCollider;

typedef struct circleCollider_t
{
	Collider super;
	uint32_t raidus;

}CircleCollider;

void Collider_OnCollisionEnterDefault(Object* thisObj/*A*/, Object* otherObj/*B*/);
/// <summary>
/// When 2 objects collide, this function runs and calls onCOllisionEnter overriden logic in the objects.
/// </summary>
/// <param name="thisObj"></param>
/// <param name="otherObj"></param>
void Collider_OnCollisionEnter(Object* thisObj, Object* otherObj)
{
	Collider_OnCollisionEnterDefault(thisObj, otherObj);
	if (thisObj != NULL && thisObj->collider != NULL) //Checking if A has collider
	{
		if (otherObj != NULL && otherObj->collider != NULL) //CHecking if B has collider
		{
			//Both have colliders so we call the overriden functions in both A and B
			if (thisObj->collider->vtable != NULL && thisObj->collider->vtable->onCollisionEnterFunc != NULL)
			{
				thisObj->collider->vtable->onCollisionEnterFunc(thisObj, otherObj);
			}
			if (otherObj->collider->vtable != NULL && otherObj->collider->vtable->onCollisionEnterFunc != NULL)
			{
				otherObj->collider->vtable->onCollisionEnterFunc(otherObj, thisObj);// other and this are reversed
			}
		}
	}
}
/// <summary> 
/// DO we even need this? Maybe not.
/// I don't think there is a default reaction to collision in my application 
/// but this should be there in most general applications.
/// </summary>
/// <param name="thisObj"></param>
/// <param name="other"></param>
static void Collider_OnCollisionEnterDefault(Object* thisObj, Object* other)
{
	//Back-up on both object

}

/// <summary>
/// Impact is processed in this function. This function is called in a way that it is deferred after the OnCollisionEnter.
/// </summary>
/// <param name="obj"></param>
void Collider_ProcessImpact(Object* obj)
{
	if (obj != NULL && obj->collider != NULL)
	{
		if (obj->collider->vtable && obj->collider->vtable->processImpact != NULL)
		{
			obj->collider->vtable->processImpact(obj);
		}
	}
}