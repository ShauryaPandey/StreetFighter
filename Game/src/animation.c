#pragma once
#include "kenanimations.h"
#include <stdlib.h>
#include <assert.h>
#include <Windows.h>
#include <gl/GLU.h>
#include "SOIL.h"
#include "Object.h"

/// <summary>
/// Sprite sheet Init
/// </summary>
/// <param name="component"></param>
/// <param name="direction"></param>
void AnimationComponent_Init(AnimationComponent* component, AnimationDirection direction)
{
	assert(component != NULL);
	component->spriteSheet.characterPage = NULL;
	strcpy(component->ownerName, "Character");
	component->animatedSprite.currentAnimation = NULL;
	component->animatedSprite.animationDirection = direction;
	component->animatedSprite.currentFrame = 0;
	component->animatedSprite.elapsedTime = 0;
	component->animatedSprite.isAnimationChanged = false;

	component->animations = NULL;
	component->numAnimations = KenAnim_Count;
}



/// <summary>
/// The animation itself has direction like going right or left use this function to set that.
/// </summary>
/// <param name="component"></param>
/// <param name="direction"></param>
void SetDirection(AnimationComponent* component, AnimationDirection direction)
{
	assert(component != NULL);
	component->animatedSprite.animationDirection = direction;
}

/// <summary>
/// If user of this function wants every frame to have equal time then use this
/// </summary>
/// <param name="animation"></param>
/// <param name="frameTime"></param>
void Animation_SetUniformFrameTime(Animation* animation, uint32_t frameTime)
{
	assert(animation != NULL);
	for (int i = 0; i < animation->len; ++i)
	{
		animation->frames[i].frameTime = frameTime;
	}	
}

/// <summary>
/// This function gives equal frame time to each of the frames by dividing the total duration by number of frames.
/// </summary>
/// <param name="component"></param>
/// <param name="id"></param>
/// <param name="behaviourDuration"></param>
void Ken_PlayAnimation_Uniform(AnimationComponent* component, Ken_Animations id, uint32_t behaviourDuration)
{
	assert(component != NULL);
	assert(component->animations != NULL);
	assert(id < KenAnim_Count);
	uint32_t perFrameTime = behaviourDuration / component->animations[id].len;
	Animation_SetUniformFrameTime(&component->animations[id], perFrameTime);
	if (component->animatedSprite.currentAnimation == &component->animations[id])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[id];
	}
}
/// <summary>
///  This method expects the caller to have taken care of frame times of the animation. So particularly useful when you want frames of the animation
/// to have different frame times.
/// </summary>
/// <param name="component"></param>
/// <param name="id"></param>
void Ken_PlayAnimation_NonUniform(AnimationComponent* component, Ken_Animations id)
{
	assert(component != NULL);
	assert(component->animations != NULL);
	assert(id < KenAnim_Count);

	if (component->animatedSprite.currentAnimation == &component->animations[id])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[id];
	}
}

/// <summary>
/// Plays hadouken animation on the ball which goes through multiple sprites as it is moving itself.
/// </summary>
/// <param name="component"></param>
/// <param name="id"></param>
/// <param name="animDuration"></param>
void Hadouken_PlayAnimation(AnimationComponent* component, Hadouken_Animations id, uint32_t animDuration)
{
	assert(component->animations != NULL);
	assert(id < HadoukenAnim_Count);
	
	if (component->animatedSprite.currentAnimation == &component->animations[id])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[id];
	}
}
/// <summary>
/// Animation for punching.
/// </summary>
/// <param name="component"></param>
void PlayPunchAnimation(AnimationComponent* component)
{
	assert(component->animations != NULL);


	if (component->animatedSprite.currentAnimation == &component->animations[KenAnim_Punching])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[KenAnim_Punching];
	}
}

/// <summary>
/// Idle animation.
/// </summary>
/// <param name="component"></param>
void PlayIdleAnimation(AnimationComponent* component)
{
	assert(component->animations != NULL);

	if (component->animatedSprite.currentAnimation == &component->animations[KenAnim_Idle])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[KenAnim_Idle];
	}
}
/// <summary>
/// Kick animation
/// </summary>
/// <param name="component"></param>
void PlayKickAnimation(AnimationComponent* component)
{
	assert(component->animations != NULL);

	if (component->animatedSprite.currentAnimation == &component->animations[KenAnim_Kick])
	{
		component->animatedSprite.isAnimationChanged = false;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[KenAnim_Kick];
	}
}
/// <summary>
/// Jump kick animation
/// </summary>
/// <param name="component"></param>
void PlayJumpKickAnimation(AnimationComponent* component)
{
	assert(component->animations != NULL);

	if (component->animatedSprite.currentAnimation == &component->animations[KenAnim_JumpKick])
	{
		component->animatedSprite.isAnimationChanged = true;
	}
	else
	{
		component->animatedSprite.isAnimationChanged = true;
		component->animatedSprite.currentAnimation = &component->animations[KenAnim_JumpKick];
	}

}


static const char CHARACTER_PAGE[] = "asset/ken.png";
static const int jumpKickAnimLen = 11;


/// <summary>
/// Gets called for both character freeing up storage within the animation component by frames data 
/// and animation struct data both were malloc'ed.
/// Also gets called when hadouken object is being deleted.
/// </summary>
/// <param name="animComponent"></param>
void DeInitAnimations(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	assert(animComponent->animations != NULL);
	for (int i = 0; i < animComponent->numAnimations; i++)
	{
		assert(animComponent->animations[i].frames != NULL);
		free(animComponent->animations[i].frames);
	}
	free(animComponent->animations);
}

/// <summary>
/// This function initializes all the anims for Ryu
/// </summary>
/// <param name="animComponent"></param>
void InitRyuAnimations(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//aniamted sprite setup
	animComponent->animatedSprite.currentFrame = 0;
	animComponent->animatedSprite.elapsedTime = 0.0f;
	//animComponent->animatedSprite.animationDirection = Animation_Right;
	animComponent->animations = malloc(animComponent->numAnimations * sizeof(Animation)); // aniamtions on Ken
	assert(animComponent->animations != NULL);

	RyuAnim_InitCrouchKick(animComponent);
	RyuAnim_InitCrouchPunch(animComponent);
	RyuAnim_InitCrouch(animComponent);
	RyuAnim_InitCrouchBlock( animComponent);
	RyuAnim_InitStandBlock(animComponent);
	RyuAnim_InitHurt(animComponent);
	RyuAnim_InitBaseHurt(animComponent);
	RyuAnim_InitCrouchHurt(animComponent);
	RyuAnim_InitKick(animComponent);
	RyuAnim_InitIdle(animComponent);
	RyuAnim_InitPunch(animComponent);
	RyuAnim_InitWalk(animComponent);
	RyuAnim_InitHadouken(animComponent);
	RyuAnim_InitJump(animComponent);
	RyuAnim_InitJumpPunch(animComponent);
	RyuAnim_InitJumpKick(animComponent);
	RyuAnim_InitDirectionalJump(animComponent);
	RyuAnim_InitDirectionalJumpPunch(animComponent);
	RyuAnim_InitDirectionalJumpKick(animComponent);
	RyuAnim_InitDeath(animComponent);
	RyuAnim_InitVictory(animComponent);
}

/// <summary>
/// This function initializes all the anims for Ken
/// </summary>
/// <param name="animComponent"></param>
void InitKenAnimations(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//aniamted sprite setup
	animComponent->animatedSprite.currentFrame = 0;
	animComponent->animatedSprite.elapsedTime = 0.0f;
	//animComponent->animatedSprite.animationDirection = Animation_Right;
	animComponent->animations = malloc(animComponent->numAnimations * sizeof(Animation)); // aniamtions on Ken
	assert(animComponent->animations != NULL);
	//walking setup
	KenAnim_InitWalk(animComponent);
	KenAnim_InitIdle(animComponent);
	KenAnim_InitPunch(animComponent);
	KenAnim_InitKick(animComponent);
	KenAnim_InitHurt(animComponent);
	KenAnim_InitBasicHurt(animComponent);
	KenAnim_InitCrouchHurt(animComponent);
	KenAnim_InitStandBlock(animComponent);
	KenAnim_InitCrouchBlock(animComponent);
	KenAnim_InitCrouch(animComponent);
	KenAnim_InitCrouchPunch(animComponent);
	KenAnim_InitCrouchKick(animComponent);
	KenAnim_InitHadouken(animComponent);
	KenAnim_InitJump(animComponent);
	KenAnim_InitJumpPunch(animComponent);
	KenAnim_InitJumpKick(animComponent);
	KenAnim_InitDirectionalJump(animComponent);
	KenAnim_InitDirectionalJumpPunch(animComponent);
	KenAnim_InitDirectionalJumpKick(animComponent);
	KenAnim_InitDeath(animComponent);
	KenAnim_InitVictory(animComponent);
}

void HadoukenAnim_InitMoving(AnimationComponent* animComponent);


/// <summary>
/// Initializes hadouken's animations.
/// </summary>
/// <param name="animComponent"></param>
void InitHadoukenAnimations(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
    animComponent->animations = malloc(animComponent->numAnimations * sizeof(Animation)); // aniamtions on Hadouken
	assert(animComponent->animations != NULL);
	animComponent->numAnimations = 1;
	HadoukenAnim_InitMoving(animComponent);
}