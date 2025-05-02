#include "baseTypes.h"
#include "object.h"
#include <assert.h>

static ObjRegistrationFunc _registerFunc = NULL;
static ObjRegistrationFunc _deregisterFunc = NULL;

/// @brief Enable callback to a registrar on ObjInit/Deinit
/// @param registerFunc 
/// @param deregisterFunc 
void objEnableRegistration(ObjRegistrationFunc registerFunc, ObjRegistrationFunc deregisterFunc)
{
    _registerFunc = registerFunc;
    _deregisterFunc = deregisterFunc;
}

/// @brief Disable registration during ObjInit/Deinit
void objDisableRegistration()
{
    _registerFunc = _deregisterFunc = NULL;
}

/// @brief Initialize an object. Intended to be called from subclass constructors
/// @param obj 
/// @param vtable 
/// @param pos 
/// @param vel 
void objInit(Object* obj, ObjVtable* vtable, Collider* collider, Bounds2D levelDef, Coord2D pos, Coord2D vel)
{
    obj->vtable = vtable;
    obj->position = pos;
    obj->velocity = vel;
    obj->collider = collider;
    obj->levelDef = levelDef;
    if (_registerFunc != NULL)
    {
        _registerFunc(obj);
    }
}

/// @brief Deinitialize an object
/// @param obj 
void objDeinit(Object* obj)
{
    if (_deregisterFunc != NULL)
    {
        _deregisterFunc(obj);
    }
}

/// @brief Draw this object, using it's vtable
/// @param obj 
void objDraw(Object* obj)
{
    if (obj->vtable != NULL && obj->vtable->draw != NULL) 
    {
        obj->vtable->draw(obj);
    }
}

/// @brief Update this object, using it's vtable
/// @param obj 
/// @param milliseconds 
void objUpdate(Object* obj, uint32_t milliseconds)
{
    if (obj->vtable != NULL && obj->vtable->update != NULL) 
    {
        obj->vtable->update(obj, milliseconds);
        return;
    }

    objDefaultUpdate(obj, milliseconds);
}

void objDefaultUpdate(Object* obj, uint32_t milliseconds)
{
    obj->position.x += obj->velocity.x;
    obj->position.y += obj->velocity.y;
}

void ObjChangeSizeRatio(Object* obj,float xratio, float yratio)
{
    assert(xratio <= 1);
    assert(yratio <= 1);
    //Calculating the delta by which to move the object down after resizing
    float newYSize = obj->size.y * yratio;
    float delta = obj->size.y - newYSize;
    delta /= 2;

    obj->size.x *= xratio;
    obj->size.y *= yratio;
    obj->position.y += delta;

//Move the obj down
}

void ObjChangeSizeAbs(Object* obj, float newX, float newY)
{

	float delta = obj->size.y - newY;
	delta /= 2;
	obj->position.y += delta;
    obj->size.x = newX;
    obj->size.y = newY;
}

//Set transform
void SetObjectTransform(Object* obj, float newX, float newY)
{
	assert(obj != NULL);
	/*Character* character = (Character*)obj;*/
	//x
	if (newX + (obj->size.x/2.0f) > obj->levelDef.botRight.x)
	{
		//Clamp it
		obj->position.x = obj->levelDef.botRight.x - (obj->size.x / 2.0f);
	}
	else if (newX - (obj->size.x/2.0f) < obj->levelDef.topLeft.x)
	{
		//Clamp it
		obj->position.x = obj->levelDef.topLeft.x + (obj->size.x / 2.0f);
	}
	else
	{
		obj->position.x = newX;
	}
	//y
	if (newY + (obj->size.y / 2.0f) > obj->levelDef.botRight.y) //if y is deeper than the mx
	{
		//Clamp it
		obj->position.y = obj->levelDef.botRight.y - (obj->size.y / 2.0f);
	}
	else if (newY - (obj->size.y/2.0f) < obj->levelDef.topLeft.y) //shallower than the top
	{
		//Clamp it
		obj->position.y = obj->levelDef.topLeft.y + (obj->size.y / 2.0f);
	}
	else
	{
		obj->position.y = newY;
	}
}

//Move transform
void MoveObjectTransform(Object* obj, float deltaX, float deltaY)
{
	float newX = 0, newY = 0;
	newX = obj->position.x + deltaX;
	newY = obj->position.y + deltaY;
	SetObjectTransform(obj, newX, newY);
}
extern float g_groundLevel;
void MoveObjectToGround(Object* obj)
{
	float ySize = obj->size.y;
	obj->position.y = g_groundLevel - ySize / 2.0f;
}

bool IsObjectOnGround(Object* obj)
{
	if (obj->position.y + (obj->size.y / 2.0f) <= g_groundLevel)
	{
		if (g_groundLevel - (obj->position.y + (obj->size.y / 2.0f)) < 0.5f)
		{
			return true;
		}
	}
	else 
	{
		if ((obj->position.y + (obj->size.y / 2.0f)) - g_groundLevel < 0.5f)
		{
			return true;
		}
	}
	return false;
}