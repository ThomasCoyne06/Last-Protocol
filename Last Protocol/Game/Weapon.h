#ifndef WEAPON_H
#define WEAPON_H

#include "../Utility/Common.h"
#include "../Utility/Animation.h"
#include "../Utility/Mapping.h"
#include "Player.h"
#include "Enemy.h"

#define SWORD_POS_X 20
#define SWORD_POS_Y 0
#define SWORD_ATTACK_X 0

#define SWORD_MIROR_X 1.5
#define SWORD_MEDIUM_X 3

#define SWORD_NORMAL_SIZE_X 120
#define SWORD_NORMAL_SIZE_Y 140

#define SWORD_HORIZONTAL_SIZE_X 190
#define SWORD_HORIZONTAL_SIZE_Y 220
#define SWORD_HORIZONTAL_POS_X -50
#define SWORD_HORIZONTAL_POS_Y 10
#define SWORD_MEDIUM_POS_X -120
#define SWORD_MEDIUM_POS_Y -90

#define SWORD_VERTICAL_SIZE_X 270
#define SWORD_VERTICAL_SIZE_Y 300


//#define SWORD_IDLE_NORMAL_X 9
//#define SWORD_IDLE_NORMAL_Y 22
//#define SWORD_IDLE_NORMAL_POS 47
//
//#define SWORD_IDLE_MEDIUM_X 9
//#define SWORD_IDLE_MEDIUM_Y 42
//#define SWORD_IDLE_MEDIUM_POS 0

#define SWORD_WIDTH 14
#define SWORD_HEIGHT 73

#define SWORD_IDLE_FRAME_NB 8
#define SWORD_IDLE_FRAMERATE 10

#define SWORD_SMALL_DAMAGE 1
#define SWORD_MEDIUM_DAMAGE 2
#define SWORD_BIG_DAMAGE 3

#define KNOCKBACK_SMALL 2000
#define KNOCKBACK_MEDIUM 3000
#define KNOCKBACK_BIG 4000


#define LOAD_SIZE_X 72
#define LOAD_SIZE_Y 72

#define LOAD_ORIGIN_X 61
#define LOAD_ORIGIN_Y 71

#define LOAD_SCALE_SMALL 0.5f
#define LOAD_SCALE_MEDIUM 3
#define LOAD_SCALE_BIG 7

#define LOAD_HITBOX_SIZE 20

#define LOAD_RADIUS 60

#define LOAD_SPEED_SMALL 20
#define LOAD_SPEED_MEDIUM 6
#define LOAD_SPEED_BIG 3

#define LOAD_HITBOX_NB 5
#define LOAD_HITBOX_ANGLE_DIFFERENCE 20

typedef enum
{
	STRETCH_SMALL,
	STRETCH_MEDIUM,
	STRETCH_BIG,
}WeaponScale;

typedef enum
{
	SWORD,
	LOAD,
	WEAPON_NB
}WeaponType;

typedef enum
{
	SWORD_IDLE_SMALL,
	SWORD_IDLE_MEDIUM,
	SWORD_IDLE_BIG,
	SWORD_NORMAL_SLASH,
	SWORD_VERTICAL_SLASH,
	SWORD_HORIZONTAL_SLASH,

	LOAD_SMALL,
	LOAD_MEDIUM,
	LOAD_BIG,

	WEAPON_ANIMATION_NB
}WeaponAnimation;

typedef struct
{
	sfTexture* textureArray[WEAPON_NB];
	sfSprite* spriteArray[WEAPON_NB];
	AnimationData animationArray[WEAPON_ANIMATION_NB];
	AnimationData* currentAnimation;

	WeaponType weaponType;
	WeaponScale weaponScale;

	sfBool isLeft;
	sfBool isAttacking;
	sfBool isSwapingWeapon;

	sfRectangleShape* hitbox[LOAD_HITBOX_NB];
}WeaponData;

void LoadWeapon(void);
void UpdateWeapon(float _dt);
void DrawWeapon(sfRenderWindow* _window);
void CleanupWeapon(void);

//Get the number of hitboxes of the weapon attack. If the player is not attacking return NULL
int GetWeaponHitboxNumber(void);
//Return the hitbox of the attack. To get all hitbox use GetWeaponHitboxNumber first
sfRectangleShape* GetWeaponHitbox(int _arrayPos);

void IncreaseSizeWeapon(void);
WeaponScale GetSizeWeapon(void);

void SetAttackWeapon(void);
void ResetWeapon(void);

void SetSizeWeapon(int _size);
const WeaponData* GetWeapon(void);
sfBool IsWeaponAttacking();

#endif


