#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "standhurtbehaviour.h"

#include "keycodes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

//void StandBlock_
PlayBigDamageSoundCB g_BigDamageSoundCB;
bool StandHurt_CanStart(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;
    CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
    assert(currentStatePtr != NULL);
    if (currentStatePtr->baseState == Hurt)
    {
        if (currentStatePtr->subState == Jump)
        {
            //Does it have a payload? only then start
            GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
            assert(gameplayComponentPtr != NULL);
            Payload* payload = gameplayComponentPtr->actions[Ken_StandHurt].payload;
            if (payload != NULL)
            {
                return true;
            }
        }
    }
    return false;
}

void StandHurt_OnStart(Object* obj, uint32_t milliseconds)
{
    //Find where attack came from and update direction
    assert(obj != NULL);
    Character* character = (Character*)obj;
    GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
    assert(gameplayComponentPtr != NULL);
    Payload* payload = gameplayComponentPtr->actions[Ken_StandHurt].payload;
    assert(payload != NULL);
    StandHurtPayload* subPayload = (StandHurtPayload*)payload;
    Coord2D otherPosition = subPayload->otherPosition;
//Get other position 
//if other is on my right my direction is right else left
    FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
    assert(characterDirectionPtr != NULL);
    if (otherPosition.x - obj->position.x > 0)//other is on my right
    {
        *characterDirectionPtr = Facing_Right; //This actually means that the character will face right and fall to his left
    }
    else
    {
        *characterDirectionPtr = Facing_Left;
    }
    ModifyCharacterHealth(character, StandHurtValue);
    //Restore original direction in cleanup
    //Payload holds original position, direction

    //Play sound 
    assert(g_BigDamageSoundCB != NULL);
    g_BigDamageSoundCB();
}

void StandHurt_GameplayController(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;
    GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
    assert(gameplayComponentPtr != NULL);
    uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
    float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
    float timePassedNormalized = 1.0f - remainingTimeNormalized;

    AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
    assert(animComponentPtr->animations[KenAnim_StandHurt].len == 2);
    Frame f1 = animComponentPtr->animations[KenAnim_StandHurt].frames[Frame_0];
    Frame f2 = animComponentPtr->animations[KenAnim_StandHurt].frames[Frame_1];
    //Frame f3 = animComponentPtr->animations[KenAnim_StandHurt].frames[Frame_2];
    Frame standFrame = animComponentPtr->animations[KenAnim_Idle].frames[Frame_0];
    float originalLength = standFrame.BottomRight.x - standFrame.topLeft.x;
    float originalHeight = standFrame.BottomRight.y - standFrame.topLeft.y;


    //time less than 0.2
    //Do nothing
    //time between 0.2 and 0.8
    if (timePassedNormalized < 0.8f)
    {
        //Set character size in this frame
        float modifiedHeight = f1.BottomRight.y - f1.topLeft.y;
        float modifiedLength = f1.BottomRight.x - f1.topLeft.x;
        ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);

        //Character goes flying up
        if (timePassedNormalized < 0.5f)
        {
            MoveObjectTransform(obj, 0, -1.0f * obj->velocity.y * milliseconds);
        }
        else //come flying down
        {
            //check distance from ground
            float distance = g_groundLevel - obj->position.y;
            uint32_t duration = gameplayComponentPtr->currentAction.action->duration;
            uint32_t timetoCover = (uint32_t)(duration * 0.3f);
            assert(timetoCover > 0);
            float velocity = distance / (float)timetoCover;
            assert(velocity > 0);
            MoveObjectTransform(obj, 0, velocity * milliseconds);
        }
        FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
        assert(characterDirectionPtr != NULL);
        if (*characterDirectionPtr == Facing_Right)//Character facing right goes left
        {
            MoveObjectTransform(obj, -1.0f * obj->velocity.x, 0.0f);
        }
        else
        {
            MoveObjectTransform(obj, obj->velocity.x, 0.0f);
        }
    }

    //Time between 0.8 and 1
    if (timePassedNormalized > 0.8f)
    {
        float modifiedHeight = f2.BottomRight.y - f2.topLeft.y;
        float modifiedLength = f2.BottomRight.x - f2.topLeft.x;
        ObjChangeSizeAbs(obj, (modifiedLength / originalLength) * CharacterStandWidth, (modifiedHeight / originalHeight) * CharacterStandHeight);
    }

    //Do nothing
}

void StandHurt_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;
    GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
    assert(gameplayComponentPtr != NULL);
    Payload* payload = gameplayComponentPtr->currentAction.action->payload;
    assert(payload != NULL);
    StandHurtPayload* subPayload = (StandHurtPayload*)payload;
    FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
    assert(characterDirectionPtr != NULL);
    *characterDirectionPtr = subPayload->Originaldirection;
    //Free payload
    DeInitActionPayload(gameplayComponentPtr, Ken_StandHurt);
}

void StandHurt_AnimationController(Object* obj, uint32_t milliseconds)
{
   
}

void StandHurt_AnimOnStart(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;
    GameplayComponent* component = GetCharacterGameplayComponent(character);
    AnimationComponent* animComponent = GetCharacterAnimationComponent(character);
    assert(component != NULL);
    assert(animComponent != NULL);
    uint32_t actionDuration = component->currentAction.action->duration;
    animComponent->animations[KenAnim_StandHurt].frames[Frame_0].frameTime = (uint32_t)(0.8f * (float)actionDuration);
    animComponent->animations[KenAnim_StandHurt].frames[Frame_1].frameTime = (uint32_t)(0.2f * (float)actionDuration);
    Ken_PlayAnimation_NonUniform(animComponent, KenAnim_StandHurt);
}