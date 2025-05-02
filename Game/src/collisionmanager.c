#include "collider.h"
#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"
#include "random.h"
#include "field.h"
#include "Object.h"
#include <assert.h>


/// <summary>
/// Checks collision between 2 objects.
/// </summary>
/// <param name="thisObj"></param>
/// <param name="otherObj"></param>
/// <returns></returns>
static bool CheckCollision(Object* thisObj, Object* otherObj)
{
	assert(thisObj != NULL);
	assert(otherObj != NULL);
	if (thisObj->collider != NULL && otherObj->collider!=NULL) // If both objects have colliders
	{
		float thisCenterX = thisObj->position.x;
		float thisCenterY = thisObj->position.y;
		float otherCenterX = otherObj->position.x;
		float otherCenterY = otherObj->position.y;

		//ARight < BLeft : If A is much to the left of B
		if (thisCenterX + (thisObj->size.x/2.0f) < otherCenterX - (otherObj->size.x/2.0f))
		{
			return false;
		}

		//ATop > BBottom : If A is much below B
		if (thisCenterY-(thisObj->size.y/2.0f) > otherCenterY+(otherObj->size.y/2.0f))
		{
			return false;
		}
		//ABottom < BTop : If A is much above B
		if (thisCenterY+(thisObj->size.y/2.0f) < otherCenterY-(otherObj->size.y/2.0f))
		{
			return false;
		}
		//ALeft>BRight : If A is much to the right of B
		if (thisCenterX-(thisObj->size.x/2.0f)> otherCenterX+(otherObj->size.x/2.0f))
		{
			return false;
		}

		return true;
	}
	return false;
}

//To-DO: Pass the data of which side and how much into on collision enter - not done

/// <summary>
///This global function is called in the objectMgr and iterates on all objects checking collisions on them. 
/// </summary>
/// <param name="list"></param>
/// <param name="max"></param>
void CheckAllCollisions(Object** list, int max)
{
	//Check collision between every pair
	for (int i = 0; i < max; i++)
	{
		Object* thisObj = list[i];
		if (thisObj != NULL)
		{
			for (int j = i + 1; j < max; ++j)
			{
				Object* otherObj = list[j];
				if (otherObj != NULL)
				{
					if (CheckCollision(thisObj, otherObj))
					{
						thisObj->collider->info.other = otherObj;
						Collider_OnCollisionEnter(thisObj,otherObj); //Internally calls the onCollissionEnter on both the collider objects
						Collider_ProcessImpact(thisObj);//Any state changes that occured during collission are processed here
						Collider_ProcessImpact(otherObj);
					}
				}
			}
		}
	}
}