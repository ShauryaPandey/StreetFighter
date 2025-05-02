#pragma once
#include "baseTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Frame_0 0
#define Frame_1 1
#define Frame_2 2
#define Frame_3 3
#define Frame_4 4

	typedef enum
	{
		AnimType_Idle,
		AnimType_Walking,
		AnimType_Punching
	}AnimationType; //not being used now this was early approach
	typedef enum
	{
		Animation_Right,
		Animation_Left
	}AnimationDirection;
	/// <summary>
	/// The name of the sheet from which animations frames are sourced.
	/// </summary>
	typedef struct spriteSheet_t
	{
		char* characterPage;

	}SpriteSheet;
	/// <summary>
	/// A frame of animation holds information that identify it in a sprite sheet.
	/// </summary>
	typedef struct frame_t
	{
		Coord2D topLeft;
		Coord2D BottomRight;
		uint32_t frameTime;
	} Frame;
	/// <summary>
	/// Animation struct holds the array of frames that define that animation.
	/// </summary>
	typedef struct animation_t
	{
		Frame* frames;
		int len; //number of frames
	}Animation;

	/// <summary>
	/// The sprite of the character containing other information - the facing direction, current animation, current frame and time passed.
	/// </summary>
	typedef struct animatedSprite_t
	{
		Animation* currentAnimation;
		AnimationDirection animationDirection;
		int currentFrame;
		float elapsedTime;
		bool isAnimationChanged;
	}AnimatedSprite;

	typedef enum ken_Animations_t
	{
		KenAnim_Walking = 0,
		KenAnim_Punching = 1,
		KenAnim_Idle = 2,
		KenAnim_Kick = 3,
		KenAnim_JumpKick = 4,
		KenAnim_StandHurt = 5,
		KenAnim_BasicHurt = 6,
		KenAnim_CrouchHurt = 7,
		KenAnim_StandBlock = 8,
		KenAnim_Crouch = 9,
		KenAnim_CrouchBlock = 10,
		KenAnim_CrouchPunch = 11,
		KenAnim_CrouchKick = 12,
		KenAnim_Hadouken = 13,
		KenAnim_StraightJump = 14,
		KenAnim_DirectionalJump=15,
		KenAnim_JumpPunch = 16,
		KenAnim_DirectionalJumpPunch = 17,
		KenAnim_DirectionalJumpKick = 18,
		KenAnim_Death = 19,
		KenAnim_Victory = 20,
		//KenAnim_JumpKick = 17,
		//KenAnim_CrouchHurt = 6,
		//KenAnim_JumpHurt = 7,
		KenAnim_Count,
		KenAnim_Invalid = -1
	}Ken_Animations;

	typedef enum ryu_Animations_t
	{
		RyuAnim_Walking = 0,
		RyuAnim_Punching = 1,
		RyuAnim_Idle = 2,
		RyuAnim_Kick = 3,
		RyuAnim_JumpKick = 4,
		RyuAnim_StandHurt = 5,
		RyuAnim_BasicHurt = 6,
		RyuAnim_CrouchHurt = 7,
		RyuAnim_StandBlock = 8,
		RyuAnim_Crouch = 9,
		RyuAnim_CrouchBlock = 10,
		RyuAnim_CrouchPunch = 11,
		RyuAnim_CrouchKick = 12,
		RyuAnim_Hadouken = 13,
		RyuAnim_StraightJump = 14,
		RyuAnim_DirectionalJump = 15,
		RyuAnim_JumpPunch = 16,
		RyuAnim_DirectionalJumpPunch = 17,
		RyuAnim_DirectionalJumpKick = 18,
		RyuAnim_Death = 19,
		RyuAnim_Victory = 20,
		//KenAnim_JumpKick = 17,
		//KenAnim_CrouchHurt = 6,
		//KenAnim_JumpHurt = 7,
		RyuAnim_Count,
		RyuAnim_Invalid = -1
	}Ryu_Animations;
	//above 2 enums have same names
	typedef enum hadouken_Animations_t
	{
		HadoukenAnim_Moving = 0,
		HadoukenAnim_Destroyed = 1,
		HadoukenAnim_Count,
		HadoukenAnim_Invalid = -1
	}Hadouken_Animations;

	typedef struct animComponent_t
	{
		char ownerName[20];
		SpriteSheet spriteSheet;
		Animation* animations;
		int numAnimations;
		AnimatedSprite animatedSprite;
	}AnimationComponent;

	void AnimationComponent_Init(AnimationComponent* component, AnimationDirection direction);
	void Ken_PlayAnimation_Uniform(AnimationComponent* component, Ken_Animations id, uint32_t behaviourDuration);
	void Ken_PlayAnimation_NonUniform(AnimationComponent* component, Ken_Animations id);
	void Hadouken_PlayAnimation(AnimationComponent* component, Hadouken_Animations id, uint32_t animDuration);

	void SetDirection(AnimationComponent* component, AnimationDirection direction);
	void InitKenAnimations(AnimationComponent* animComponent);
	void InitRyuAnimations(AnimationComponent* animComponent);
	void DeInitAnimations(AnimationComponent* animComponent);
	void InitHadoukenAnimations(AnimationComponent* animComponent);
#ifdef __cplusplus
}
#endif