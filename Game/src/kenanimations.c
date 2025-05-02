#include "kenanimations.h"
#include <stdlib.h>
#include <assert.h>
#include <Windows.h>

#include "Object.h"
/*
* Allocating frame coords for the frames in the animations we need in the game. The values here are not put in a defines file or static const
* since there are a lot of such values and they are not going to be repeated a lot.
*/

static const int walkAnimLen = 3;
static const int punchAnimLen = 2/*1*/;
static const int idleAnimLen = 4;
static const int kickAnimLen = 2;
//static const int jumpKickAnimLen = 11;
static const int standHurtAnimLen = 2/*3*//*4*/;
static const int basicHurtAnimLen = 1;
static const int standBlockAnimLen = 1;
static const int crouchAnimLen = /*2*/1;
static const int crouchBlockAnimLen = 1;
static const int crouchPunchAnimLen = 2;
static const int crouchKickAnimLen = 2;
static const int hadoukenLen = 3;
static const int straightJumpAnimLen = 3;
static const int directionalJumpAnimLen = 4;
static const int jumpPunchAnimLen = 4;
static const int jumpKickAnimLen = 4;
static const int directionalJumpPunchAnimLen = 5;
static const int directionalJumpKickAnimLen = 5;
static const int deathAnimLen = 1;
static const int victoryAnimLen = 1;

// #To-Do : wrap assert in a different function

void KenAnim_InitDeath(AnimationComponent* animComponent)
{
	animComponent->animations[KenAnim_Death].frames = malloc(deathAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Death].len = deathAnimLen;
	//animComponent->animations[KenAnim_CrouchKick].perFrameTime = 1000.0f;

	Frame* currentFrame = animComponent->animations[KenAnim_Death].frames;
	assert(currentFrame != NULL);
	Coord2D temp;

	temp.x = 377;
	temp.y = 912;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 272;
	temp.y = 880;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitDeath(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_Death].frames = malloc(deathAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Death].len = deathAnimLen;
	//animComponent->animations[KenAnim_CrouchKick].perFrameTime = 1000.0f;

	Frame* currentFrame = animComponent->animations[RyuAnim_Death].frames;
	assert(currentFrame != NULL);
	Coord2D temp;

	temp.x = 295;
	temp.y = 1024;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 191;
	temp.y = 994;
	currentFrame[Frame_0].topLeft = temp;
}

void KenAnim_InitVictory(AnimationComponent* animComponent)
{
	animComponent->animations[KenAnim_Victory].frames = malloc(victoryAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Victory].len = victoryAnimLen;
	//animComponent->animations[KenAnim_CrouchKick].perFrameTime = 1000.0f;

	Frame* currentFrame = animComponent->animations[KenAnim_Victory].frames;
	assert(currentFrame != NULL);
	Coord2D temp;
	temp.x = 119;
	temp.y = 1031;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 74;
	temp.y = 923;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitVictory(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_Victory].frames = malloc(victoryAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Victory].len = victoryAnimLen;
	//animComponent->animations[KenAnim_CrouchKick].perFrameTime = 1000.0f;

	Frame* currentFrame = animComponent->animations[RyuAnim_Victory].frames;
	assert(currentFrame != NULL);
	Coord2D temp;
	temp.x = 110;
	temp.y = 1024;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 911;
	currentFrame[Frame_0].topLeft = temp;
}
void KenAnim_InitCrouchKick(AnimationComponent* animComponent)
{
	//Crouch Kick
	animComponent->animations[KenAnim_CrouchKick].frames = malloc(crouchKickAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_CrouchKick].len = crouchKickAnimLen;
	//animComponent->animations[KenAnim_CrouchKick].perFrameTime = 1000.0f;

	Frame* currentFrame = animComponent->animations[KenAnim_CrouchKick].frames;
	assert(currentFrame != NULL);
	Coord2D temp;
	temp.x = 293;
	temp.y = 576;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 207;
	temp.y = 514;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 111;
	temp.y = 295;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[Frame_1].topLeft = temp;
}

void RyuAnim_InitCrouchKick(AnimationComponent* animComponent)
{
	//Crouch Kick
	animComponent->animations[KenAnim_CrouchKick].frames = malloc(crouchKickAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_CrouchKick].len = crouchKickAnimLen;

	Frame* currentFrame = animComponent->animations[KenAnim_CrouchKick].frames;
	assert(currentFrame != NULL);
	Coord2D temp;
	//Crouch kick
	temp.x = 93;
	temp.y = 576;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 514;
	currentFrame[Frame_0].topLeft = temp;
	//Crouch sitting
	temp.x = 111;
	temp.y = 296;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[Frame_1].topLeft = temp;
}

void KenAnim_InitCrouchPunch(AnimationComponent* animComponent)
{
	//Crouch Punch
	animComponent->animations[KenAnim_CrouchPunch].frames = malloc(crouchPunchAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_CrouchPunch].len = crouchPunchAnimLen;
	//animComponent->animations[KenAnim_CrouchPunch].perFrameTime = 1000.0f;

	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_CrouchPunch].frames;
	assert(currentFrame != NULL);
	temp.x = 487;
	temp.y = 296;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 407;
	temp.y = 236;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 111;
	temp.y = 295;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[Frame_1].topLeft = temp;

}

void RyuAnim_InitCrouchPunch(AnimationComponent* animComponent)
{
	//Crouch Punch
	animComponent->animations[RyuAnim_CrouchPunch].frames = malloc(crouchPunchAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_CrouchPunch].len = crouchPunchAnimLen;
	//animComponent->animations[KenAnim_CrouchPunch].perFrameTime = 1000.0f;

	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_CrouchPunch].frames;
	assert(currentFrame != NULL);

	temp.x = 488;
	temp.y = 296;
	currentFrame[Frame_0].BottomRight = temp;

	temp.x = 407;
	temp.y = 236;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 111;
	temp.y = 296;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[Frame_1].topLeft = temp;
}

void KenAnim_InitCrouch(AnimationComponent* animComponent)
{
	//Crouch
	assert(animComponent != NULL);
	animComponent->animations[KenAnim_Crouch].frames = malloc(crouchAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Crouch].len = crouchAnimLen;
	//animComponent->animations[KenAnim_Crouch].perFrameTime = 500.0f;

	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Crouch].frames;
	assert(currentFrame != NULL);
	/*temp.x = 50;
	temp.y = 295;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 215;
	currentFrame[Frame_0].topLeft = temp;*/

	temp.x = 111;
	temp.y = 295;
	currentFrame[/*Frame_1*/Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[/*Frame_1*/Frame_0].topLeft = temp;

}

void RyuAnim_InitCrouch(AnimationComponent* animComponent)
{
	//Crouch
	assert(animComponent != NULL);
	animComponent->animations[RyuAnim_Crouch].frames = malloc(crouchAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Crouch].len = crouchAnimLen;
	//animComponent->animations[KenAnim_Crouch].perFrameTime = 500.0f;

	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Crouch].frames;
	assert(currentFrame != NULL);
	temp.x = 111;
	temp.y = 296;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 232;
	currentFrame[Frame_0].topLeft = temp;
}

void KenAnim_InitCrouchBlock(AnimationComponent* animComponent)
{
	//Crouch Block
	assert(animComponent != NULL);
	animComponent->animations[KenAnim_CrouchBlock].frames = malloc(crouchBlockAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_CrouchBlock].len = crouchBlockAnimLen;
	//animComponent->animations[KenAnim_CrouchBlock].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_CrouchBlock].frames;
	assert(currentFrame != NULL);
	temp.x = 166;
	temp.y = 296;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 119;
	temp.y = 233;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitCrouchBlock(AnimationComponent* animComponent)
{
	//Crouch Block
	assert(animComponent != NULL);
	animComponent->animations[RyuAnim_CrouchBlock].frames = malloc(crouchBlockAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_CrouchBlock].len = crouchBlockAnimLen;
	//animComponent->animations[KenAnim_CrouchBlock].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_CrouchBlock].frames;
	assert(currentFrame != NULL);

	temp.x = 168;
	temp.y = 296;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 119;
	temp.y = 232;
	currentFrame[Frame_0].topLeft = temp;
}

void KenAnim_InitStandBlock(AnimationComponent* animComponent)
{
	// Stand Block
	assert(animComponent != NULL);
	animComponent->animations[KenAnim_StandBlock].frames = malloc(standBlockAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_StandBlock].len = standBlockAnimLen;
	//animComponent->animations[KenAnim_StandBlock].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_StandBlock].frames;
	assert(currentFrame != NULL);
	temp.x = 449;
	temp.y = 400;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 393;
	temp.y = 306;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitStandBlock(AnimationComponent* animComponent)
{
	// Stand Block
	assert(animComponent != NULL);
	animComponent->animations[RyuAnim_StandBlock].frames = malloc(standBlockAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_StandBlock].len = standBlockAnimLen;
	//animComponent->animations[KenAnim_StandBlock].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_StandBlock].frames;
	assert(currentFrame != NULL);

	temp.x = 448;
	temp.y = 400;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 392;
	temp.y = 307;
	currentFrame[Frame_0].topLeft = temp;
}

void KenAnim_InitHurt(AnimationComponent* animComponent)
{
	//Hurt
	assert(animComponent != NULL);
	animComponent->animations[KenAnim_StandHurt].frames = malloc(standHurtAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_StandHurt].len = standHurtAnimLen;
	//animComponent->animations[KenAnim_StandHurt].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_StandHurt].frames;
	assert(currentFrame != NULL);
	
	//i == 2
	temp.x = 264;
	temp.y = 912;
	currentFrame[Frame_0/*Frame_2*/].BottomRight = temp;
	temp.x = 198;
	temp.y = 848;
	currentFrame[Frame_0/*Frame_2*/].topLeft = temp;
	//i == 3
	temp.x = 377;
	temp.y = 912;
	currentFrame[Frame_1/*Frame_3*/].BottomRight = temp;
	temp.x = 272;
	temp.y = 880;
	currentFrame[Frame_1/*Frame_3*/].topLeft = temp;
}

void RyuAnim_InitHurt(AnimationComponent* animComponent)
{
	//Hurt
	assert(animComponent != NULL);
	animComponent->animations[RyuAnim_StandHurt].frames = malloc(standHurtAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_StandHurt].len = standHurtAnimLen;
	//animComponent->animations[KenAnim_StandHurt].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_StandHurt].frames;
	assert(currentFrame != NULL);

	//Falling
	temp.x = 183;
	temp.y = 1024;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 119;
	temp.y = 960;
	currentFrame[Frame_0].topLeft = temp;
	//Fallen
	temp.x = 295;
	temp.y = 1024;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 191;
	temp.y = 994;
	currentFrame[Frame_1].topLeft = temp;
}

void KenAnim_InitBasicHurt(AnimationComponent* animComponent)
{
	animComponent->animations[KenAnim_BasicHurt].frames = malloc(basicHurtAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_BasicHurt].len = basicHurtAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_BasicHurt].frames;
	assert(currentFrame != NULL);

	temp.x = 71;
	temp.y = 912;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 820;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitBaseHurt(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_BasicHurt].frames = malloc(basicHurtAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_BasicHurt].len = basicHurtAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_BasicHurt].frames;
	assert(currentFrame != NULL);

	//Stand hurt
	temp.x = 263;
	temp.y = 800;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 207;
	temp.y = 711;
	currentFrame[Frame_0].topLeft = temp;
}

void KenAnim_InitCrouchHurt(AnimationComponent* animComponent)
{
	animComponent->animations[KenAnim_CrouchHurt].frames = malloc(basicHurtAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_CrouchHurt].len = basicHurtAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_CrouchHurt].frames;
	assert(currentFrame != NULL);

	temp.x = 192;
	temp.y = 912;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 143;
	temp.y = 849;
	currentFrame[Frame_0].topLeft = temp;
}

void RyuAnim_InitCrouchHurt(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_CrouchHurt].frames = malloc(basicHurtAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_CrouchHurt].len = basicHurtAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_CrouchHurt].frames;
	assert(currentFrame != NULL);

	temp.x = 320;
	temp.y = 800;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 271;
	temp.y = 737;
	currentFrame[Frame_0].topLeft = temp;
}
void KenAnim_InitKick(AnimationComponent* animComponent)
{
	animComponent->animations[KenAnim_Kick].frames = malloc(kickAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Kick].len = kickAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Kick].frames;
	assert(currentFrame != NULL);
	//x = 0, 100; y :420,505
	//Kick
	temp.x = 100;
	temp.y = 505;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 0;
	temp.y = 420;
	currentFrame[Frame_0].topLeft = temp;
	//Idle 3
	temp.x = 526;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 463;
	temp.y = 13;
	currentFrame[Frame_1].topLeft = temp;
}

void RyuAnim_InitKick(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_Kick].frames = malloc(kickAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Kick].len = kickAnimLen;
	//animComponent->animations[KenAnim_Kick].perFrameTime = 2000.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Kick].frames;
	assert(currentFrame != NULL);
	//Kick
	temp.x = 97;
	temp.y = 504;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 419;
	currentFrame[Frame_0].topLeft = temp;

	//Idle 3
	temp.x = 447;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 383;
	temp.y = 13;
	currentFrame[Frame_1].topLeft = temp;
}

void KenAnim_InitIdle(AnimationComponent* animComponent)
{
	// 4 frames
	animComponent->animations[KenAnim_Idle].frames = malloc(idleAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Idle].len = idleAnimLen;
	//animComponent->animations[KenAnim_Idle].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Idle].frames;
	assert(currentFrame != NULL);
	//i==0
	temp.x = 374;
	temp.y = 104;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 328;
	temp.y = 11;
	currentFrame[Frame_0].topLeft = temp;
	//i==1
	temp.x = 451;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 383;
	temp.y = 13;
	currentFrame[Frame_1].topLeft = temp;
	//i==2
	temp.x = 526;
	temp.y = 104;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 463;
	temp.y = 13;
	currentFrame[Frame_2].topLeft = temp;
	//i==3
	temp.x = 599;
	temp.y = 104;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 535;
	temp.y = 13;
	currentFrame[Frame_3].topLeft = temp;
}

void RyuAnim_InitIdle(AnimationComponent* animComponent)
{
	// 4 frames
	//Ryu has 3 frames only
	const int ryuIdleAnimLen = 3;
	animComponent->animations[RyuAnim_Idle].frames = malloc(ryuIdleAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Idle].len = ryuIdleAnimLen;
	//animComponent->animations[KenAnim_Idle].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Idle].frames;
	assert(currentFrame != NULL);

	temp.x = 374;
	temp.y = 104;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 327;
	temp.y = 11;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 447;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 383;
	temp.y = 13;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 511;
	temp.y = 104;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 455;
	temp.y = 12;
	currentFrame[Frame_2].topLeft = temp;
}

void KenAnim_InitPunch(AnimationComponent* animComponent)
{
	//2 frame
	animComponent->animations[KenAnim_Punching].frames = malloc(punchAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Punching].len = punchAnimLen;
	//animComponent->animations[KenAnim_Punching].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Punching].frames;
	assert(currentFrame != NULL);

	temp.x = 154;
	temp.y = 400;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 79;
	temp.y = 307;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 526;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 463;
	temp.y = 13;
	currentFrame[Frame_1].topLeft = temp;
}

void RyuAnim_InitPunch(AnimationComponent* animComponent)
{
	animComponent->animations[RyuAnim_Punching].frames = malloc(punchAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Punching].len = punchAnimLen;
	//animComponent->animations[KenAnim_Punching].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Punching].frames;
	assert(currentFrame != NULL);
	
	temp.x = 263;
	temp.y = 400;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 168;
	temp.y = 310;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 511;
	temp.y = 104;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 455;
	temp.y = 12;
	currentFrame[Frame_1].topLeft = temp;
}
void KenAnim_InitWalk(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_Walking].frames = malloc(walkAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_Walking].len = walkAnimLen;
	//animComponent->animations[KenAnim_Walking].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Walking].frames;
	assert(currentFrame != NULL);

	temp.x = 438;
	temp.y = 911;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 383;
	temp.y = 815;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 499;
	temp.y = 912;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 447;
	temp.y = 815;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 566;
	temp.y = 913;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 511;
	temp.y = 815;
	currentFrame[Frame_2].topLeft = temp;
}

void RyuAnim_InitWalk(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_Walking].frames = malloc(walkAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Walking].len = walkAnimLen;
	//animComponent->animations[KenAnim_Walking].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Walking].frames;
	assert(currentFrame != NULL);
	
	temp.x = 380;
	temp.y = 904;
	currentFrame[Frame_0].BottomRight = temp;
	
	temp.x = 327;
	temp.y = 808;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 446;
	temp.y = 905;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 391;
	temp.y = 808;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 508;
	temp.y = 904;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 455;
	temp.y = 809;
	currentFrame[Frame_2].topLeft = temp;
}




void KenAnim_InitHadouken(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_Hadouken].frames = malloc(hadoukenLen * sizeof(Frame));
	animComponent->animations[KenAnim_Hadouken].len = hadoukenLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_Hadouken].frames;
	assert(currentFrame != NULL);

	temp.x = 414;
	temp.y = 680;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 351;
	temp.y = 590;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 585;
	temp.y = 680;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 519;
	temp.y = 591;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 510;
	temp.y = 680;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 423;
	temp.y = 591;
	currentFrame[Frame_2].topLeft = temp;
}

void RyuAnim_InitHadouken(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_Hadouken].frames = malloc(hadoukenLen * sizeof(Frame));
	animComponent->animations[RyuAnim_Hadouken].len = hadoukenLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_Hadouken].frames;
	assert(currentFrame != NULL);

	temp.x = 395;
	temp.y = 704;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 336;
	temp.y = 614;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 328;
	temp.y = 704;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 263;
	temp.y = 614;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 253;
	temp.y = 704;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 167;
	temp.y = 614;
	currentFrame[Frame_2].topLeft = temp;
}

void KenAnim_InitJump(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_StraightJump].frames = malloc(straightJumpAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_StraightJump].len = straightJumpAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_StraightJump].frames;
	assert(currentFrame != NULL);

	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 204;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 167;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_2].topLeft = temp;
}

void RyuAnim_InitJump(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_StraightJump].frames = malloc(straightJumpAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_StraightJump].len = straightJumpAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_StraightJump].frames;
	assert(currentFrame != NULL);

	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 140;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 103;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_2].topLeft = temp;
}

void KenAnim_InitJumpPunch(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_JumpPunch].frames = malloc(jumpPunchAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_JumpPunch].len = jumpPunchAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_JumpPunch].frames;
	assert(currentFrame != NULL);

	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 204;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 167;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 253;
	temp.y = 680;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 183;
	temp.y = 618;
	currentFrame[Frame_2].topLeft = temp;
	
	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_3].topLeft = temp;
}

void RyuAnim_InitJumpPunch(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_JumpPunch].frames = malloc(jumpPunchAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_JumpPunch].len = jumpPunchAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_JumpPunch].frames;
	assert(currentFrame != NULL);

	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 140;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 103;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 493;
	temp.y = 576;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 423;
	temp.y = 512;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_3].topLeft = temp;
}
void KenAnim_InitJumpKick(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_JumpKick].frames = malloc(jumpKickAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_JumpKick].len = jumpKickAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_JumpKick].frames;
	assert(currentFrame != NULL);

	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 204;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 167;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 85;
	temp.y = 680;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 7;
	temp.y = 615;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 160;
	temp.y = 208;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 127;
	temp.y = 112;
	currentFrame[Frame_3].topLeft = temp;
}

void RyuAnim_InitJumpKick(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_JumpKick].frames = malloc(jumpKickAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_JumpKick].len = jumpKickAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_JumpKick].frames;
	assert(currentFrame != NULL);
	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 140;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 103;
	temp.y = 143;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 326;
	temp.y = 576;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 247;
	temp.y = 511;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 96;
	temp.y = 208;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 63;
	temp.y = 112;
	currentFrame[Frame_3].topLeft = temp;
}
void KenAnim_InitDirectionalJump(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_DirectionalJump].frames = malloc(directionalJumpAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_DirectionalJump].len = directionalJumpAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_DirectionalJump].frames;
	assert(currentFrame != NULL);

	temp.x = 118;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 71;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 264;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 215;
	temp.y = 162;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 320;
	temp.y = 209;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 271;
	temp.y = 163;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 118;
	temp.y = 208;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 71;
	temp.y = 120;
	currentFrame[Frame_3].topLeft = temp;
}

void RyuAnim_InitDirectionalJump(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_DirectionalJump].frames = malloc(directionalJumpAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_DirectionalJump].len = directionalJumpAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_DirectionalJump].frames;
	assert(currentFrame != NULL);

	temp.x = 53;
	temp.y = 209;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 256;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 207;
	temp.y = 160;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 312;
	temp.y = 208;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 263;
	temp.y = 160;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 53;
	temp.y = 209;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 7;
	temp.y = 120;
	currentFrame[Frame_3].topLeft = temp;
}
void KenAnim_InitDirectionalJumpPunch(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_DirectionalJumpPunch].frames = malloc(directionalJumpPunchAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_DirectionalJumpPunch].len = directionalJumpPunchAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_DirectionalJumpPunch].frames;
	assert(currentFrame != NULL);

	temp.x = 118;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 71;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 264;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 215;
	temp.y = 162;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 320;
	temp.y = 209;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 271;
	temp.y = 163;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 253;
	temp.y = 680;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 183;
	temp.y = 618;
	currentFrame[Frame_3].topLeft = temp;

	temp.x = 422;
	temp.y = 208;
	currentFrame[Frame_4].BottomRight = temp;
	temp.x = 383;
	temp.y = 143;
	currentFrame[Frame_4].topLeft = temp;
}

void RyuAnim_InitDirectionalJumpPunch(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_DirectionalJumpPunch].frames = malloc(directionalJumpPunchAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_DirectionalJumpPunch].len = directionalJumpPunchAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_DirectionalJumpPunch].frames;
	assert(currentFrame != NULL);

	temp.x = 53;
	temp.y = 209;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 256;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 207;
	temp.y = 160;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 312;
	temp.y = 208;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 263;
	temp.y = 160;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 493;
	temp.y = 576;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 423;
	temp.y = 512;
	currentFrame[Frame_3].topLeft = temp;

	temp.x = 358;
	temp.y = 208;
	currentFrame[Frame_4].BottomRight = temp;
	temp.x = 319;
	temp.y = 143;
	currentFrame[Frame_4].topLeft = temp;
}
void KenAnim_InitDirectionalJumpKick(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[KenAnim_DirectionalJumpKick].frames = malloc(directionalJumpKickAnimLen * sizeof(Frame));
	animComponent->animations[KenAnim_DirectionalJumpKick].len = directionalJumpKickAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[KenAnim_DirectionalJumpKick].frames;
	assert(currentFrame != NULL);

	temp.x = 118;
	temp.y = 208;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 71;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 264;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 215;
	temp.y = 162;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 320;
	temp.y = 209;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 271;
	temp.y = 163;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 85;
	temp.y = 680;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 7;
	temp.y = 615;
	currentFrame[Frame_3].topLeft = temp;

	temp.x = 422;
	temp.y = 208;
	currentFrame[Frame_4].BottomRight = temp;
	temp.x = 383;
	temp.y = 143;
	currentFrame[Frame_4].topLeft = temp;
}

void RyuAnim_InitDirectionalJumpKick(AnimationComponent* animComponent)
{
	assert(animComponent != NULL);
	//3 frames
	animComponent->animations[RyuAnim_DirectionalJumpKick].frames = malloc(directionalJumpKickAnimLen * sizeof(Frame));
	animComponent->animations[RyuAnim_DirectionalJumpKick].len = directionalJumpKickAnimLen;
	//animComponent->animations[KenAnim_Hadouken].perFrameTime = 500.0f;
	Coord2D temp;
	Frame* currentFrame = animComponent->animations[RyuAnim_DirectionalJumpKick].frames;
	assert(currentFrame != NULL);

	temp.x = 53;
	temp.y = 209;
	currentFrame[Frame_0].BottomRight = temp;
	temp.x = 7;
	temp.y = 120;
	currentFrame[Frame_0].topLeft = temp;

	temp.x = 256;
	temp.y = 208;
	currentFrame[Frame_1].BottomRight = temp;
	temp.x = 207;
	temp.y = 160;
	currentFrame[Frame_1].topLeft = temp;

	temp.x = 312;
	temp.y = 208;
	currentFrame[Frame_2].BottomRight = temp;
	temp.x = 263;
	temp.y = 160;
	currentFrame[Frame_2].topLeft = temp;

	temp.x = 326;
	temp.y = 576;
	currentFrame[Frame_3].BottomRight = temp;
	temp.x = 247;
	temp.y = 511;
	currentFrame[Frame_3].topLeft = temp;

	temp.x = 358;
	temp.y = 208;
	currentFrame[Frame_4].BottomRight = temp;
	temp.x = 319;
	temp.y = 143;
	currentFrame[Frame_4].topLeft = temp;
}