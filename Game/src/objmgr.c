#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include "objmgr.h"
#include "baseTypes.h"

static struct objmgr_t {
    Object** list;
    uint32_t max;
    uint32_t count;
} _objMgr = { NULL, 0, 0 };

/// @brief Initialize the object manager
/// @param maxObjects 
void objMgrInit(uint32_t maxObjects)
{
    // allocate the required space
    _objMgr.list = malloc(maxObjects * sizeof(Object*));
    if (_objMgr.list != NULL) {
        // initialize as empty
        ZeroMemory(_objMgr.list, maxObjects * sizeof(Object*));
        _objMgr.max = maxObjects;
        _objMgr.count = 0;
    }

    // setup registration, so all initialized objects are logged w/ the manager
    objEnableRegistration(objMgrAdd, objMgrRemove);
}

/// @brief Shutdown the object manager
void objMgrShutdown()
{
    // disable registration, since the object manager is shutting down
    objDisableRegistration();

    // this isn't strictly required, but want to enforce proper cleanup
    assert(_objMgr.count == 0);

    // objMgr doesn't own the objects, so just clean up self
    free(_objMgr.list);
    _objMgr.list = NULL;
    _objMgr.max = _objMgr.count = 0;
}

/// @brief Add an object to be tracked by the manager
/// @param obj 
void objMgrAdd(Object* obj)
{
    for (uint32_t i = 0; i < _objMgr.max; ++i)
    {
        if (_objMgr.list[i] == NULL)
        {
            _objMgr.list[i] = obj;
            ++_objMgr.count;
            return;
        }
    }

    // out of space to add object!
    assert(false);
}

/// @brief Remove an object from the manager's tracking
/// @param obj 
void objMgrRemove(Object* obj)
{
    for (uint32_t i = 0; i < _objMgr.max; ++i)
    {
        if (obj == _objMgr.list[i])
        {
            // no need to free memory, so just clear the reference
            _objMgr.list[i] = NULL;
            --_objMgr.count;
            return;
        }
    }

    // could not find object to remove!
    assert(false);
}

/// @brief Draws all registered objects
void objMgrDraw() 
{
    for (uint32_t i = 0; i < _objMgr.max; ++i)
    {
        Object* obj = _objMgr.list[i];
        if (obj != NULL)
        {
            // TODO - consider draw order?
            objDraw(obj);
        }
    }
}

/// @brief Updates all registered objects
/// @param milliseconds 
void objMgrUpdate(uint32_t milliseconds)
{
    for (uint32_t i = 0; i < _objMgr.max; ++i)
    {
        Object* obj = _objMgr.list[i];
        if (obj != NULL)
        {
            objUpdate(obj, milliseconds);
        }
    }
    CheckAllCollisions(_objMgr.list, _objMgr.max);
}

