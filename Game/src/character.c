#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SOIL.h"

#include "input.h"
#include "baseTypes.h"
#include "character.h"
#include "gameplaycomponent.h"
#include "gameplaystatemanager.h"
#include "hadouken.h"
#include "Object.h"
#include "collider.h"
#include "impactinfo.h"
#include "random.h"
#include "healthbar.h"

static const int32_t CHARACTER_COUNT = 8;
static const int _maxHealth = 100;
/// <summary>
/// The characters in our game that the user can control and do moves with. 
/// </summary>
typedef struct character_t
{
	Object      obj;
	char name[16];
	Coord2D     levelBounds;
	int health;
	uint32_t    character;
	GameSpawnHadoukenFunc spawnHadoukenFunc;
	GameCanSpawnHadoukenFunc canSpawnHadoukenFunc;

	AnimationComponent animComponent; //Resources allocated inside animcomponent should be cleared by anim component
	GameplayComponent gameplayComponent;//Resources allocated inside gameplay component should be cleared by gameplay component
	FacingDirection direction; //where is the character looking
	CharacterGameplayState gameplayState; 
	ImpactInfo impactInfo; //All changes to be made after a collision are stored here and post processed collision
	DefenseComponent defenseComponent; //Details when block/defending that can be used to determine if a block/defense is succesful
	AttackComponent attackComponent; //Details when attacking which can be used if the attack is landing on the opponent
	HealthBar* healthBar; //Reference to this which we update everytime health is changed so that there is a visual update
	bool isDead;
	bool isWon;
} Character;

void UpdateCharacterSize(Character* character, CharacterGameplaySubState subState)
{
	if (subState == Stand)
	{
		ObjChangeSizeAbs(&character->obj, CharacterStandWidth/*character->obj.size.x*/, CharacterStandHeight);
	}
	else if (subState == Crouch)
	{
		ObjChangeSizeAbs(&character->obj, CharacterStandWidth/*character->obj.size.x*/, CharacterCrouchHeight);
	}
}
AnimationComponent* GetCharacterAnimationComponent(Character* character)
{
	return &character->animComponent;
}

char* GetCharacterName(Character* character)
{
	assert(character != NULL);
	return character->name;
}
// How to make movement happen and restrain it within the level bounds
CharacterGameplayState* GetCharacterGameplayState(Character* character)
{
	assert(character != NULL);
	return &character->gameplayState;
}
GameplayComponent* GetCharacterGameplayComponent(Character* character)
{
	return &character->gameplayComponent;
}

FacingDirection* GetCharacterFacingDirection(Character* character)
{
	return &character->direction;
}
AttackComponent* GetCharacterAttackComponent(Character* character)
{
	return &character->attackComponent;
}

DefenseComponent* GetCharacterDefenseComponent(Character* character)
{
	return &character->defenseComponent;
}

uint8_t GetCharacterHealth(Character* character)
{
	return character->health;
}
//Checking if the character can spawn another hadouken. THis is possible if there is no active hadouken on screen
bool CanSpawnHadouken(Character* character)
{
	if (character->canSpawnHadoukenFunc != NULL)
	{
		return character->canSpawnHadoukenFunc(character);
	}
	return false;
}
void ModifyCharacterHealth(Character* character, uint8_t delta)
{
	character->health -= delta;
	if (character->health <= 0)
	{
		character->health = 0;
		character->isDead = true;
	}
}

bool IsCharacterDead(Character* character)
{
	return character->isDead;
}

bool isCharacterWon(Character* character)
{
	return character->isWon;
}
//Declare the character has won
void CharacterWon(Character* character)
{
	character->isWon = true;
}
//Declare the character dead
void CharacterDead(Character* character)
{
	character->isDead = true;
}
//If possible spawn a hadouken
void TrySpawnHadouken(Character* character, Coord2D position, float velocityX)
{
	assert(character->spawnHadoukenFunc != NULL);
	if (character->spawnHadoukenFunc != NULL)
	{
		character->spawnHadoukenFunc(character, position, velocityX);
	}
}

static Animation* walkAnimation;
static const char CHARACTER_PAGE[] = "asset/ken3.png";
static const char CHARACTER2_PAGE[] = "asset/ryu.png";
static bool isAnimStateChanged = false;
static GLuint _characterTexture = 0;
static GLuint _ryuTexture = 0;

// the object vtable for all characters
static void _characterUpdate(Object* obj, uint32_t milliseconds);
static void _characterDraw(Object* obj);
static ObjVtable _characterVtable = {
	_characterDraw,
	_characterUpdate
};
static ColliderVtable _colliderVtable = {
	_characterOnCollisionEnter,
	_processImpactInfo
};

static uint32_t _getUpdateTime();

//void characterInitAnimations(Character* character);
/// @brief one time initialization of textures
void characterInitTextures()
{
	if (_characterTexture == 0)
	{
		_characterTexture = SOIL_load_OGL_texture(CHARACTER_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		assert(_characterTexture != 0);
		//characterInitAnimations();
		//characterInitAnimations();
	}
	if (_ryuTexture == 0)
	{
		_ryuTexture = SOIL_load_OGL_texture(CHARACTER2_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		assert(_ryuTexture != 0);
	}
}

typedef struct healthBar_t HealthBar;
/// @brief Allocates & initializes a character object
/// Initializing a character object
/// @param box 
/// @return 
Character* characterNew(Bounds2D box, Coord2D* position, FacingDirection direction, const char* nameStr, GameSpawnHadoukenFunc spawnHadoukenFunc, GameCanSpawnHadoukenFunc canSpawnHadoukenFunc, HealthBar* bar)
{
	Character* character = malloc(sizeof(Character));
	assert(bar != NULL);
	if (character != NULL)
	{
		character->obj.type = Type_Character;
		//Collider for the character
		Collider* collider = malloc(sizeof(Collider)); //this collider must be freed by object de init
		assert(collider != NULL);
		collider->vtable = &_colliderVtable;

		if (position == NULL)
		{
			Coord2D center = boundsGetCenter(&box);
			Coord2D vel = { 1.0f, 0.1f };
			objInit(&character->obj, &_characterVtable, collider, box, center, vel);
		}
		else
		{
			Coord2D vel = { 1.0f, 0.1f };
			objInit(&character->obj, &_characterVtable, collider, box, *position, vel);
		}
		character->spawnHadoukenFunc = spawnHadoukenFunc;//callback when using hadouken behaviour
		character->canSpawnHadoukenFunc = canSpawnHadoukenFunc; //calback to check if hadouken can be spawned
		strcpy(character->name, nameStr); //storing the name in the character
		// extract the dimensions from the bounding box for rendering
		Coord2D size = { 100,200 };
		character->obj.size = size;
		//character->size = size;// boundsGetDimensions(&box);
		Coord2D levelBounds = boundsGetDimensions(&box);
		character->levelBounds = levelBounds;

		character->direction = direction;
		character->gameplayState.baseState = None;
		character->gameplayState.subState = Stand;
	
		ImpactInfoInit(&character->impactInfo);
		if (direction == Facing_Right)
		{
			AnimationComponent_Init(&character->animComponent,Animation_Right);
		}
		else
		{
			AnimationComponent_Init(&character->animComponent,Animation_Left);
		}

		GameplayComponent_Init(&character->gameplayComponent);
		character->healthBar = bar; //holding reference to the health bar which is a gui representation of the characters health
		character->isDead = false;
		character->health = _maxHealth;
		character->isWon = false;
	}
	return character;
}
//On round change we need to reset some states of the character. It is not needed that we delete the character and reinint it. 
void characterReset(Character* character, float xPos, FacingDirection direction)
{
	assert(character != NULL);
	character->health = _maxHealth;
	character->isWon = false;
	character->isDead = false;
	character->direction = direction;
	AnimationComponent* animComponentPtr = GetCharacterAnimationComponent(character);

	if(direction == Facing_Right)
		animComponentPtr->animatedSprite.animationDirection = Facing_Right;
	else
		animComponentPtr->animatedSprite.animationDirection = Facing_Left;

	character->gameplayState.baseState = None;
	character->gameplayState.subState = Stand;
	Coord2D size = { CharacterStandWidth,CharacterStandHeight };
	character->obj.size = size;
	character->obj.position.x = xPos;
}

/// @brief Frees up resources from a character object
/// @param character 
void characterDelete(Character* character)
{
	free(character->obj.collider);
	objDeinit(&character->obj);
	//GameplayComponent_DeInit(&character->gameplayComponent);
	ImpactInfoDeInit(&character->impactInfo);
	free(character);
}

/// @brief Object draw handler
/// @param obj 
static void _characterDraw(Object* obj)
{
	assert(obj->type == Type_Character);
	Character* character = (Character*)obj;
	assert(character != NULL);
	AnimatedSprite* animatedSprite = &character->animComponent.animatedSprite;
	assert(animatedSprite != NULL);
	if (animatedSprite->currentAnimation == NULL)
	{
		//no animation to play which is odd maybe this should be asserted

		return;
	}

	assert(animatedSprite->currentAnimation != NULL);
	const Frame* currentFrame = NULL;
	//Get the texture coordinates from the frame struct

	assert(animatedSprite->currentFrame < animatedSprite->currentAnimation->len);

	currentFrame = &animatedSprite->currentAnimation->frames[animatedSprite->currentFrame]; //Now draw this frame
	uint32_t spriteSheetWidth = 0;
	uint32_t spriteSheetHeight = 0;
	assert(currentFrame != NULL);
	glEnable(GL_TEXTURE_2D);
	glDisable(0x809D); //Const value for GL_MULTISAMPLE
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);

	if (strcmp(character->name, "Ryu") == 0)
	{
		glBindTexture(GL_TEXTURE_2D, _ryuTexture);
		spriteSheetWidth = RyuSpriteSheetWidth;
		spriteSheetHeight = RyuSpriteSheetHeight;
	}
	else if (strcmp(character->name, "Ken") == 0)
	{
		//glBindTexture(GL_TEXTURE_2D, _ryuTexture);
		glBindTexture(GL_TEXTURE_2D, _characterTexture);
		spriteSheetWidth = KenSpriteSheetWidth;
		spriteSheetHeight = KenSpriteSheetHeight;
	}
	else
	{
		assert(false);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, _characterTexture);
	glBegin(GL_TRIANGLE_STRIP);
	{
		// calculate the bounding box of the character
		GLfloat xPositionLeft = (obj->position.x - character->obj.size.x / 2);
		GLfloat xPositionRight = (obj->position.x + character->obj.size.x / 2);
		GLfloat yPositionTop = (obj->position.y - character->obj.size.y / 2);
		GLfloat yPositionBottom = (obj->position.y + character->obj.size.y / 2);

		float startingU = 0;
		float endingU = 0;
		float startingV = 0;
		float endingV = 0;
		if (animatedSprite->animationDirection == Animation_Right)
		{
			startingU = currentFrame->topLeft.x / spriteSheetWidth; //u1
			endingU = currentFrame->BottomRight.x / spriteSheetWidth; //u2
			startingV = 1 - ((float)currentFrame->BottomRight.y / (float)spriteSheetHeight); //v1
			endingV = 1 - ((float)currentFrame->topLeft.y / (float)spriteSheetHeight); //v2

		}
		else //anim is left so reverese the U values
		{
			endingU = currentFrame->topLeft.x / spriteSheetWidth; //u1
			startingU = currentFrame->BottomRight.x / spriteSheetWidth; //u2
			startingV = 1 - ((float)currentFrame->BottomRight.y / (float)spriteSheetHeight); //v1
			endingV = 1 - ((float)currentFrame->topLeft.y / (float)spriteSheetHeight); //v2
		}

		// calculate the starting uv... remember v of 0 is the bottom of the texture

		const float BG_DEPTH = -0.99f; 

		// draw the textured quad as a tristrip
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

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
//Checks if the impactPoint lies somewhere in the bounds of the character. If it is then the character is getting hit otherwise not.
bool IsImpactPointHitting(Object* obj, Coord2D impactPoint)
{
	assert(obj != NULL);
	if (impactPoint.x < obj->position.x + obj->size.x / 2.0f && impactPoint.x > obj->position.x - obj->size.x / 2.0f)
	{// ip btw x1 and x2
		if (impactPoint.y > obj->position.y - obj->size.y / 2.0f && impactPoint.y < obj->position.y + obj->position.y / 2.0f)
		{//measure from the top so is it deeper than top and above the bottom?
			return true;
		}
	}
	return false;
}
//Checking if the impact point is in the defense range of the character. Useful once established that the impact point is hitting.
bool IsImpactPointInDefenseRange(Object* obj, Object* other, Coord2D impactPoint)
{
	Character* character = (Character*)obj;
	DefenseComponent* defenseComponent = GetCharacterDefenseComponent(character);
	// #To-Do : Check if defense is in direction of attack, it not no need to check switch case
	//if other is on my left and my direction is right then i get attacked
	//if other is on my right and my direction is left then I get attacked
	if (obj->position.x - other->position.x > 0) //other is on my left
	{
		if (character->direction == Facing_Right)
		{
			//I hit
			return false;
		}
	}
	else
	{
		if (character->direction == Facing_Left)
		{
			//I hit
			return false;
		}
	}

	switch (defenseComponent->range)
	{
	case Defense_Upper:
	{
		if (impactPoint.y < obj->position.y) //if impact point is above the mid point then it is defended
		{
			return true;
		}
		return false;
	}
	case Defense_Full:
	{
		return true;
	}

	}
	return false;
}

//Ever collider object must override this function. This is what it does upon collision. COntains logic for what happens next.
// However, since we dont want to interfere with the collission processing of other colliders we refrain from changing states. Rather
// Put that information in the impactInfo struct that gets processed by a different system
//Dont modify any states here except impact Info
static void _characterOnCollisionEnter(Object* thisObj, Object* otherObj)
{
	assert(thisObj != NULL);
	assert(otherObj != NULL);
	Character* thisChar = (Character*)thisObj;
	//if other char is hadouken
	//what to do if hit by hadouken 
	if (otherObj->type == Type_Hadouken) //High priority attack
	{
		Hadouken* otherHadouken = (Hadouken*)otherObj;
		if (thisChar->gameplayState.baseState == Block)
		{
			assert(IsImpactInfoDirty(&thisChar->impactInfo) == false);
			if (IsImpactPointInDefenseRange(thisObj, otherObj, otherObj->position))
			{
				ImpactInfoSet(&thisChar->impactInfo, BlockHurt, thisChar->gameplayState.subState, otherObj);
			}
			else
			{
				ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
			}
		}
		else //anything else and this charactr should get hurt
		{
			assert(IsImpactInfoDirty(&thisChar->impactInfo) == false);
			ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
		}
		return;
	}
	//Stop Movement Action
	//Calculate distance along x and y to retract and use that action

	//If other is character
	Character* otherChar = (Character*)otherObj;
	//If other is not attacking but only stopped our movement
	if ((otherChar->gameplayState.baseState == None || otherChar->gameplayState.baseState == Block)
		&& (thisChar->gameplayState.baseState == None || thisChar->gameplayState.baseState == Block))
	{
		if (thisChar->gameplayState.subState == Jump || otherChar->gameplayState.subState == Jump)
		{
			return; //dont retract when either is jumping
		}
		assert(IsImpactInfoDirty(&thisChar->impactInfo) == false); //Impactinfo should not have existing impact information
		ImpactInfoSet(&thisChar->impactInfo, Retract, thisChar->gameplayState.subState, otherObj);
	}

	if (otherChar->gameplayState.baseState == Attack)
	{
		if (!otherChar->attackComponent.isActive)
		{
			//Doesn't have an impact point so no damage
			return;
		}
		if (!IsImpactPointHitting(thisObj, otherChar->attackComponent.impactPoint))
		{
			//Not hitting so escape
			return;
		}
		switch (thisChar->gameplayState.baseState)
		{
		case None:
		{
			//Get hurt
			//Does character have existing impact Info? That is bad 
			assert(IsImpactInfoDirty(&thisChar->impactInfo) == false); //Impactinfo should not have existing impact information
			//If attack priority is heavy then substate of hurt is jump elese whatever was earlier
			ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
			// how to distribvguish btw basic hurt, crouch hurt, big hurt
			break; //Case None
		}
		case Block:
		{
			assert(IsImpactInfoDirty(&thisChar->impactInfo) == false);
			if (IsImpactPointInDefenseRange(thisObj, otherObj, otherChar->attackComponent.impactPoint))
			{
				ImpactInfoSet(&thisChar->impactInfo, BlockHurt, thisChar->gameplayState.subState, otherObj);
			}
			else
			{
				ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
			}
			break; //Case Block
		}
		case Attack:
		{
			assert(IsImpactInfoDirty(&thisChar->impactInfo) == false);
			//Is it in impact point? already checked above
			//Yes
			// Okay is my attack in impact point 
			if (IsImpactPointHitting(otherObj, thisChar->attackComponent.impactPoint))
			{
				//Yes- who has more priority?
				AttackPriority myAttackPriority = thisChar->attackComponent.priority;
				AttackPriority hisAttackPriority = otherChar->attackComponent.priority;
				if (myAttackPriority > hisAttackPriority)
				{
					return;
				}
				else
				{
					ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
				}
			}
			else
			{
				//No? Okay hurt
				ImpactInfoSet(&thisChar->impactInfo, Hurt, thisChar->gameplayState.subState, otherObj);
			}
			break;
		}
		}
	}
}
// This function checks current time and determines whether the frame in animation must be updated
static void _characterAnimUpdate(Object* obj, uint32_t milliseconds)
{
	Character* character = (Character*)obj;
	assert(character != NULL);
	AnimatedSprite* animatedSprite = &character->animComponent.animatedSprite;
	if (animatedSprite == NULL)
	{
		return;
	}
	if (animatedSprite->currentAnimation == NULL)
	{
		return;
	}
	assert(animatedSprite->currentAnimation != NULL);
	if (animatedSprite->isAnimationChanged == true) //if animation is changed, we basically need to change elapsedtime and current frame number
	{
		animatedSprite->elapsedTime = 0;
		animatedSprite->currentFrame = 0;
		animatedSprite->isAnimationChanged = false;
	}
	animatedSprite->elapsedTime += milliseconds;

	if (animatedSprite->elapsedTime > animatedSprite->currentAnimation->frames[animatedSprite->currentFrame].frameTime) //if elapsed time is more than frame time we move to next frame
	{
		animatedSprite->currentFrame += 1;
		if (animatedSprite->currentFrame >= animatedSprite->currentAnimation->len)
		{
			animatedSprite->currentFrame = 0; //basically we are looping animation
		}

		animatedSprite->elapsedTime = 0.0f; //elapsedTime back to 0
	}
	else
	{
		//animated sprite on the correct frame
	}
}

static void _characterUpdate(Object* obj, uint32_t milliseconds)
{
	//objDefaultUpdate(obj, milliseconds);

	Character* character = (Character*)obj;
	assert(character != NULL);
	AnimationComponent* animComponent = &character->animComponent;
	assert(animComponent != NULL);
	//AnimationDirection direction;

	//Is ImpactInfo Dirty?
	//Call State transition

	GameplayComponent* gameplayComponent = &character->gameplayComponent;
	assert(gameplayComponent != NULL);
	GameplayController_Update(gameplayComponent, obj, milliseconds); //Calling the update on the gameplay component which will execute the current behaviour/action and evaluate if a new one can begin

	_characterAnimUpdate(obj, milliseconds);
	setHealthBar(character->healthBar, character->health); //a better way is to check if health has changed but this works too where we update the health bar every frame
}

PlayHadoukenSound _hadoukenSoundCB = NULL; //reference for function that will play the sound for hadouken
/// <summary>
/// Deffering the changes to the character attributes made during collision by placing the logic in this implementation which only runs
/// when onCollisionEnter is executed on the 2 colliding entities
/// </summary>
/// <param name="obj"></param>
static void _processImpactInfo(Object* obj)
{
	assert(obj != NULL);
	Character* character = (Character*)obj;
	if (IsImpactInfoDirty(&character->impactInfo))
	{
		//Make changes
		ImpactInfo* impactInfo = &character->impactInfo;
		assert(impactInfo->newState != NULL);
		assert(impactInfo->other != NULL);
		TryChangeGameplayState(character, *impactInfo->newState);

		switch (impactInfo->newState->baseState)
		{
		case Retract:
		{
			//To-Do: Check if the action already has a payload this may be if the action never could execute.
			TryDeInitActionPayload(&character->gameplayComponent, Ken_Retract);
			//Create new Retract payload
			RetractPayload* payload = malloc(sizeof(RetractPayload));
			assert(payload != NULL);
			payload->other = impactInfo->other;
			InitActionPayload(&character->gameplayComponent, Ken_Retract, (Payload*)payload);
			break;
		}
		case Hurt:
		{
			switch (impactInfo->newState->subState)
			{
			case Stand:
			{
				//Stand & crouch have the same behaviour so the next case should execute for stand

			}
			case Crouch:
			{
				TryDeInitActionPayload(&character->gameplayComponent, Ken_BasicHurt);
				assert(character->gameplayComponent.actions[Ken_BasicHurt].payload == NULL);

				HurtPayload* payload = malloc(sizeof(HurtPayload));
				assert(payload != NULL);
				payload->Originaldirection = character->direction;
				payload->otherPosition = impactInfo->other->position;
				InitActionPayload(&character->gameplayComponent, Ken_BasicHurt, (Payload*)payload);
				break;
			}
			case Jump:
			{
				TryDeInitActionPayload(&character->gameplayComponent, Ken_StandHurt);
				assert(character->gameplayComponent.actions[Ken_StandHurt].payload == NULL);

				//Create payload for stand hurt
				StandHurtPayload* payload = malloc(sizeof(StandHurtPayload));
				assert(payload != NULL);
				payload->Originaldirection = character->direction;
				payload->otherPosition = impactInfo->other->position;
				payload->y = obj->position.y;
				InitActionPayload(&character->gameplayComponent, Ken_StandHurt, (Payload*)payload);
				break;
			}
			}
			break; // from case baseState Hurt
		}
		case BlockHurt:
		{
			TryDeInitActionPayload(&character->gameplayComponent, Ken_BlockHurt);
			assert(character->gameplayComponent.actions[Ken_BlockHurt].payload == NULL);
			//Character state change is sufficient dont think any payload is required
			Payload* payload = malloc(sizeof(Payload));
			assert(payload != NULL);
			payload->position = obj->position;
			payload->size = obj->size;
			InitActionPayload(&character->gameplayComponent, Ken_BlockHurt, payload);
			break;
		}

		default:
		{
			assert(false);
		}

		}
		ImpactInfoDeInit(&character->impactInfo);
	}
}

//static uint32_t _getUpdateTime()
//{
//	const uint32_t MIN_TIME = 500;
//	const uint32_t MAX_TIME = 2000;
//
//	return randGetInt(MIN_TIME, MAX_TIME);
//}
