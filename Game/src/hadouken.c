#include "hadouken.h"

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>
#include "SOIL.h"

#include "animation.h"
#include "shape.h"
#include "baseTypes.h"
#include "collider.h"
#include "impactinfo.h"
#include "Object.h"
#include "random.h"

//THe hadouken can collider with another hadouken or a character so we need to be able to have collision detection on the hadouken
static ColliderVtable _colliderVtable = {
    _hadoukenOnCollisionEnter,
    _hadoukenProcessImpactInfo
};
static ObjVtable _hadoukenVtable = {
    _hadoukenDraw,
    _hadoukenUpdate
};
/// <summary>
/// Hadouken is the fireball that Ryu and Ken can spawn in the hadouken behaviour. This holds information about that fireball.
/// </summary>
typedef struct hadouken_t
{
    Object object;
    AnimationComponent animComponent;
    hadoukenNotifyOwnerFunc notifyOwnerFunc;
    float radius; //If using as a ball instead of a rectange holding the sprite
    ImpactInfo impactInfo;
}Hadouken;

AnimationComponent* GetHadoukenAnimationComponent(Hadouken* hadouken)
{
    assert(hadouken != NULL);
    return &hadouken->animComponent;
}
static GLuint _hadoukenTexture = 0;
static const char HADOUKEN_PAGE[] = "asset/ken2.png";

void hadoukenInitTextures()
{
    if (_hadoukenTexture == 0)
    {
        _hadoukenTexture = SOIL_load_OGL_texture(HADOUKEN_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_hadoukenTexture != 0);

    }
}
//If the hadouken crosses the bounds of the screen we need to de-initialize it
static bool _isHadoukenCrossedBounds(Object* obj)
{
    if (obj->position.x - obj->size.x / 2.0f > obj->levelDef.botRight.x)
    {
        return true;
    }
    if (obj->position.x + obj->size.x / 2.0f < obj->levelDef.topLeft.x)
    {
        return true;
    }
    return false;
}

/// <summary>
///Initialize the animation component of the hadouken when it is initialized. 
/// </summary>
/// <param name="hadouken"></param>
/// <param name="velocityX"></param>
void HadoukenInitAnimationComponent(Hadouken* hadouken, float velocityX)
{
    assert(hadouken != NULL);
    AnimationComponent* component = &hadouken->animComponent;
    assert(component != NULL);
    strcpy(component->ownerName, "Hadouken");
    component->spriteSheet.characterPage = NULL;

    component->animatedSprite.currentAnimation = NULL;
    if (velocityX > 0) //positive velocity means its going right
    {
        component->animatedSprite.animationDirection = Animation_Left;
    }
    else
    {
        component->animatedSprite.animationDirection = Animation_Right;
    }

    component->animatedSprite.currentFrame = 0;
    component->animatedSprite.elapsedTime = 0;
    component->animatedSprite.isAnimationChanged = false;

    component->animations = NULL;
    component->numAnimations = HadoukenAnim_Count;

    //Modify hadouken animation components direction according to velocity val
    if (velocityX > 0)
    {
        hadouken->animComponent.animatedSprite.animationDirection = Animation_Right; //reverse since sprites are reversed by source
    }
    else
    {
        hadouken->animComponent.animatedSprite.animationDirection = Animation_Left; //reverse since sprites are reversed by source
    }
}

//Creates a hadouken object and returns the pointer to that obj
Hadouken* hadoukenNew(Bounds2D box, Coord2D* position, hadoukenNotifyOwnerFunc notifyOwnerFunc, float velocityX)
{
    Hadouken* hadouken = malloc(sizeof(Hadouken));
    assert(hadouken != NULL);
    hadouken->object.type = Type_Hadouken;
    Collider* collider = malloc(sizeof(Collider));
    assert(collider != NULL);
    collider->vtable = &_colliderVtable;
    assert(position != NULL);
    Coord2D vel = { velocityX,0 };
    objInit(&hadouken->object, &_hadoukenVtable, collider, box, *position, vel);
    hadouken->radius = 20.0f;
    hadouken->notifyOwnerFunc = notifyOwnerFunc;
    Coord2D size = { 50,50 };
    hadouken->object.size = size;

    HadoukenInitAnimationComponent(hadouken, velocityX); //Inits the animcomoponent accordingly
    ImpactInfoInit(&hadouken->impactInfo);
    return hadouken;
}

/// <summary>
/// Must make sure to delete the animation and frames within that along with the obj. 
/// </summary>
/// <param name="hadouken"></param>
void hadoukenDelete(Hadouken* hadouken)
{
    assert(hadouken != NULL);
    free(hadouken->object.collider);
    DeInitAnimations(&hadouken->animComponent);
    objDeinit(&hadouken->object);
    ImpactInfoDeInit(&hadouken->impactInfo);
    free(hadouken);
}

/// <summary>
/// For playing animations on the hadouken itself. At the moment the hadouken transitions during movement 
/// but future scope involves an animation on collision.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
static void _hadoukenAnimUpdate(Object* obj, uint32_t milliseconds)
{
    assert(obj != NULL);
    Hadouken* hadouken = (Hadouken*)obj;
    assert(hadouken != NULL);
    AnimatedSprite* animatedSprite = &hadouken->animComponent.animatedSprite;
    if (animatedSprite == NULL)
    {
        return;
    }
    if (animatedSprite->currentAnimation == NULL)
    {
        return;
    }
    assert(animatedSprite->currentAnimation != NULL);
    if (animatedSprite->isAnimationChanged == true)
    {
        animatedSprite->elapsedTime = 0;
        animatedSprite->currentFrame = 0;
    }
    animatedSprite->elapsedTime += milliseconds; //updating elapsed time

    if (animatedSprite->elapsedTime > animatedSprite->currentAnimation->frames[animatedSprite->currentFrame].frameTime) //if new frame needs to be put
    {
        animatedSprite->currentFrame += 1;
        if (animatedSprite->currentFrame >= animatedSprite->currentAnimation->len)
        {
            animatedSprite->currentFrame = animatedSprite->currentAnimation->len - 1; //stay on the last frame
        }

        animatedSprite->elapsedTime = 0.0f; //elapsedTime back to 0
    }
    else
    {
        //animated sprite on the correct frame
    }
}

/// <summary>
///Once spawned the hadouken has a very specific behaviour that needs to run every update frame. 
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
static void _hadoukenUpdate(Object* obj, uint32_t milliseconds)
{
    obj->position.x += obj->velocity.x * milliseconds;
    obj->position.y += obj->velocity.y * milliseconds;
    Hadouken* hadouken = (Hadouken*)obj;

    Hadouken_PlayAnimation(&hadouken->animComponent, HadoukenAnim_Moving, 10000);
    _hadoukenAnimUpdate(obj, milliseconds);
    if (_isHadoukenCrossedBounds(obj))
    {
        //Destroy this and notfiy the owner about it
        Hadouken* hadouken = (Hadouken*)obj;
        hadouken->notifyOwnerFunc(hadouken);
        hadoukenDelete(hadouken);
    }

    //if collision happens destroy the object
}

/// <summary>
/// Drawing the object on screen. 
/// </summary>
/// <param name="obj"></param>
static void _hadoukenDraw(Object* obj)
{
    assert(obj != NULL);
    assert(obj->type == Type_Hadouken);

    Hadouken* hadouken = (Hadouken*)obj;
    assert(hadouken != NULL);
    AnimatedSprite* animatedSprite = &hadouken->animComponent.animatedSprite;
    assert(animatedSprite != NULL);
    if (animatedSprite->currentAnimation == NULL)
    {
        //no animation to play

        return;
    }

    assert(animatedSprite->currentAnimation != NULL);
    const Frame* currentFrame = NULL;
    //Get the texture coordinates from the frame struct

    assert(animatedSprite->currentFrame < animatedSprite->currentAnimation->len);

    currentFrame = &animatedSprite->currentAnimation->frames[animatedSprite->currentFrame]; //Now draw this frame

    assert(currentFrame != NULL);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _hadoukenTexture);
    glBegin(GL_TRIANGLE_STRIP);
    {
        // calculate the bounding box of the character
        GLfloat xPositionLeft = (obj->position.x - hadouken->object.size.x / 2);
        GLfloat xPositionRight = (obj->position.x + hadouken->object.size.x / 2);
        GLfloat yPositionTop = (obj->position.y - hadouken->object.size.y / 2);
        GLfloat yPositionBottom = (obj->position.y + hadouken->object.size.y / 2);

        float startingU = 0;
        float endingU = 0;
        float startingV = 0;
        float endingV = 0;
        //hadouken sprites are reversed in original source
        if (animatedSprite->animationDirection == Animation_Left)
        {
            startingU = currentFrame->topLeft.x / (float)938; //u1
            endingU = currentFrame->BottomRight.x / (float)938; //u2
            startingV = 1 - ((float)currentFrame->BottomRight.y / (float)9683); //v1
            endingV = 1 - ((float)currentFrame->topLeft.y / (float)9683); //v2

        }
        else
        {
            endingU = currentFrame->topLeft.x / (float)938; //u1
            startingU = currentFrame->BottomRight.x / (float)938; //u2
            startingV = 1 - ((float)currentFrame->BottomRight.y / (float)9683); //v1
            endingV = 1 - ((float)currentFrame->topLeft.y / (float)9683); //v2
        }

        const float BG_DEPTH = -0.99f;

        // draw the textured quad as a tristrip
        glColor4ub(0x00, 0xFF, 0xFF, 0xFF);

        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();
}

/// <summary>
/// Dont make any state changes here do that in process impact info. 
/// </summary>
/// <param name="thisObj"></param>
/// <param name="otherObj"></param>
static void _hadoukenOnCollisionEnter(Object* thisObj, Object* otherObj)
{
    assert(thisObj != NULL);
    assert(otherObj != NULL);

    //Irespective of other objs type this must get destoryed after notifyingowner
    Hadouken* hadouken = (Hadouken*)thisObj;
    hadouken->notifyOwnerFunc(hadouken); //Tells the character and the lvel manager actually that it is destroyed so update the active hadoukens array
    //Notify owner and destroy object
}

/// <summary>
/// Where actual collision detection is processed. It is deferred so that all 
/// colliding objects can run their detection without any interference.
/// </summary>
/// <param name="obj"></param>
static void _hadoukenProcessImpactInfo(Object* obj)
{
    Hadouken* hadouken = (Hadouken*)obj;
    hadoukenDelete(hadouken);
}

static const int hadoukenMoveAnimLen = 3;

/// <summary>
/// Initialize animation for moving animations. 
/// </summary>
/// <param name="animComponent"></param>
void HadoukenAnim_InitMoving(AnimationComponent* animComponent)
{
    animComponent->animations[HadoukenAnim_Moving].frames = malloc(hadoukenMoveAnimLen * sizeof(Frame));
    animComponent->animations[HadoukenAnim_Moving].len = hadoukenMoveAnimLen;

    Frame* currentFrame = animComponent->animations[HadoukenAnim_Moving].frames;
    assert(currentFrame != NULL);
    Coord2D temp;
    temp.x = 566;
    temp.y = 3730;
    currentFrame[Frame_0].BottomRight = temp;
    temp.x = 536;
    temp.y = 3699;
    currentFrame[Frame_0].topLeft = temp;
    currentFrame[Frame_0].frameTime = 100;
    temp.x = 468;
    temp.y = 3730;
    currentFrame[Frame_1].BottomRight = temp;
    temp.x = 421;
    temp.y = 3699;
    currentFrame[Frame_1].topLeft = temp;
    currentFrame[Frame_1].frameTime = 500;
    temp.x = 533;
    temp.y = 3730;
    currentFrame[Frame_2].BottomRight = temp;
    temp.x = 470;
    temp.y = 3699;
    currentFrame[Frame_2].topLeft = temp;
    currentFrame[Frame_2].frameTime = 10000;
}