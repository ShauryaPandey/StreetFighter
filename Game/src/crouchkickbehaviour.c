#include "character.h"
#include "gameplaystatemanager.h"
#include "gameplaycomponent.h"
#include "input.h"
#include "keycodes.h"

#include <assert.h>
#include <Windows.h>
#include <stdlib.h>
// Kicking behaviour while kicking
bool CrouchKick_CanStart(Object* obj, uint32_t milliseconds)
{
    if (inputKeyPressed(VK_DOWN) && inputKeyPressed(VK_Z)) //Key down and Z is pressed
    {
        Character* character = (Character*)obj;
        CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
        if (currentStatePtr->subState == Jump)
        {
            return false;
        }
        return true;
    }
    return false;
}

void CrouchKick_GameplayOnStart(Object* obj, uint32_t milliseconds)
{
    assert(obj != NULL);
    CharacterGameplayState crouchKickState = { Attack,Crouch }; //Setting the gameplay state here
    Character* character = (Character*)obj;
    CharacterGameplayState* currentStatePtr = GetCharacterGameplayState(character);
    assert(currentStatePtr != NULL);
    TryChangeGameplayState(character, crouchKickState); // #To-Do : This also changes size. Move it from this function and do it explicitly here
}
//Gameplay controller runs every frame for the duration of the behaviour
void CrouchKick_GameplayController(Object* obj, uint32_t milliseconds)
{
    assert(obj != NULL);
    Character* character = (Character*)obj;

    GameplayComponent* gameplayComponentPtr = GetCharacterGameplayComponent(character);
    assert(gameplayComponentPtr != NULL);
    // I am finding out the time passed until now as a ratio out of 1 so that I can take decisions based on when something needs to be executed
    uint32_t remainingTime = (uint32_t)gameplayComponentPtr->currentAction.remainingTime;
    float remainingTimeNormalized = (float)remainingTime / (float)gameplayComponentPtr->currentAction.action->duration;
    float timePassedNormalized = 1.0f - remainingTimeNormalized;

    AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
    assert(animComponentPtr->animations[KenAnim_CrouchKick].len == 2); //Making sure that the anim is 2 frames long
    //Getting length information directly from the frames instead of using hard values
    Frame CrouchKickFrame = animComponentPtr->animations[KenAnim_CrouchKick].frames[Frame_0];
    Frame CrouchFrame = animComponentPtr->animations[KenAnim_CrouchKick].frames[Frame_1];
    float increasedWidth = CrouchKickFrame.BottomRight.x - CrouchKickFrame.topLeft.x;
    float normalWidth = CrouchFrame.BottomRight.x - CrouchFrame.topLeft.x;
    assert(increasedWidth > 0 && normalWidth > 0);

    AttackComponent* attackComponent = GetCharacterAttackComponent(character);
    FacingDirection* characterDirectionPtr = GetCharacterFacingDirection(character);
    assert(characterDirectionPtr != NULL);

    attackComponent->impactPoint.y = obj->position.y + obj->size.y / 2.0f; // the impact point y position

    if (timePassedNormalized < 0.5f)//If time is less than 0.5 increase object size
    {
        //first frame
        ObjChangeSizeAbs(obj, (increasedWidth / normalWidth) * CharacterStandWidth, CharacterCrouchHeight);
        attackComponent->priority = mediumPriority;
        attackComponent->isActive = true; // attacking in this frame
        if (*characterDirectionPtr == Facing_Right)
        {
            attackComponent->impactPoint.x = obj->position.x + obj->size.x / 2.0f;
        }
        else
        {
            attackComponent->impactPoint.x = obj->position.x - obj->size.x / 2.0f;
        }
    }
    else//if time is more than 0.5 decrease object size
    {
        ObjChangeSizeAbs(obj, CharacterStandWidth, CharacterCrouchHeight);
        attackComponent->isActive = false;
    }
}

/// <summary>
/// Set animation uniformly of same duration to each frame. You get it by diving total behaviour duration by num animation frames.
/// </summary>
/// <param name="obj"></param>
/// <param name="milliseconds"></param>
void CrouchKick_AnimOnStart(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;
    GameplayComponent* componentPtr = GetCharacterGameplayComponent(character);
    AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);
    assert(componentPtr != NULL);
    assert(animComponentPtr != NULL);

    Ken_PlayAnimation_Uniform(animComponentPtr, KenAnim_CrouchKick, componentPtr->currentAction.action->duration);
}

void CrouchKick_GameplayCleanup(Object* obj, uint32_t milliseconds)
{
    Character* character = (Character*)obj;

    AttackComponent* attackComponent = GetCharacterAttackComponent(character);
    attackComponent->isActive = false;
}


