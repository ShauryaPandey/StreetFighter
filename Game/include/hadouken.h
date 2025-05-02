#pragma once
#include "animation.h"
#include "gameplaycomponent.h"
#include "gameplaystatemanager.h"
#include "Object.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct hadouken_t Hadouken;
	typedef void (*hadoukenNotifyOwnerFunc)(Hadouken* hadouken);
	Hadouken* hadoukenNew(Bounds2D box, Coord2D* position, hadoukenNotifyOwnerFunc notifyOwnerFunc, float velocityX);
	void _hadoukenUpdate(Object* obj, uint32_t milliseconds);
	void _hadoukenDraw(Object* obj);
	void hadoukenDelete(Hadouken* hadouken);
	void _hadoukenProcessImpactInfo(Object* obj);
	void _hadoukenOnCollisionEnter(Object* thisObj, Object* otherObj);
	void hadoukenInitTextures();
	AnimationComponent* GetHadoukenAnimationComponent(Hadouken* hadouken);

	void HadoukenAnim_InitMoving(AnimationComponent* animComponent);
#ifdef __cplusplus
}
#endif