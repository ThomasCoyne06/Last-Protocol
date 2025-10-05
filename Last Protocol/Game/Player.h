#ifndef PLAYER_H
#define PLAYER_H

#include "../Utility/Common.h"

#include "Bullet.h"
#include "Pickup.h"
#include "Level.h"
#include "Weapon.h"
#include "Laser.h"
#include "Bumper.h"
#include "../Utility/Mapping.h"
#include "HUD.h"
#include "Background.h"
#include "Sign.h"
#include "Door.h"
#include "Miscellenous.h"
#include "../Stats.h"
#include "../Utility/Transition.h"

#include "../Utility/Camera.h"

#define PLAYER_WIDTH 240
#define PLAYER_HEIGHT 127
#define ORIGIN_Y 107.3f
#define PLAYER_HITBOX_OFFSET 0

#define PLAYER_HITBOX_WIDTH 19
#define PLAYER_HITBOX_HEIGHT 39
#define HITBOX_JUMP_HEIGHT 30

#define MAX_HEALTH_POINTS 6
#define INVINCIBLE_TIMER 2

#define NB_JUMP 1
#define NB_DASH 1
#define FRAME_RATE 10
#define PLAYER_SPEED_MIN 275
#define PLAYER_SPEED_MAX 600
#define PLAYER_SCALE 8
#define PLAYER_SHOOT_COOLDOWN 0.7f

#define ACCELERATION_TIME 0.4f
#define SLOW 0.2f //0.00002 //

#define PLAYER_IMPULSE -700
////////////////////////////////////////////////////////////////////////////////////
//  jump size for bumper
#define PLAYER_IMPULSE_BUMPER -1400

#define COOLDOWN_JUMP 0.3f
#define COOLDOWN_DASH 1

// Not used?
//#define START_FALLING_SPEED -300
//#define MAX_FALLING_SPEED 500
#define JUMP_DIRECTION_CHANGE 1.f//0.6f

// the increase of size lenght when you maintain the jump button 
#define GRAVITY_REDUCE 1.6f

// not used, was supposed to be use to be able to jump after falling from a platform
#define FLOATINGROUND 0.2f

#define TRAVELLED_DISTANCE 800
#define DASH_SPEED 2500
#define  TIME_BETWWEN_DASH 0.3f

#define SPEED_SLIDE 1.6f
#define SLOW_SLIDE 1

#define ON_GROUND_TIMER_MAX 0.1f
#define LANDING_SOUND 100
//#define BULLET_TIME_DURATION 5 
#define BULLET_TIME_SLOW 2.5f


#define ENERGY_MAX 100.f
#define ENERGY_WIN_BY_SECOND 20
#define ENERGY_LOST_1ST_SIZE (ENERGY_MAX/19)
#define ENERGY_LOST_2ND_SIZE (ENERGY_MAX/(20/3))
#define ENERGY_LOST_BULLET_TIME 10
// the energy we get by picking file is in Pickup.h


#pragma region Pierre_me_les_casse

#define JUMP_ANIM  0

#define IDLE_ANIM_START  0
#define RUN_ANIM_START 148 
#define JUMP_ANIM_START 267 // + JUMP_ANIM
#define DEAD_ANIM_START 2164 -107

#define SLIDING_ANIM_START 555  
#define DASH_ANIM_START 417

#define STATIC_ATTACK_ANIM_START 689 
#define MOVING_ATTACK_ANIM_START 816
#define JUMPING_ATTACK_ANIM_START 950  + JUMP_ANIM

#define STATIC_ATTACK_MEDIUM 1519
#define MOVING_ATTACK_MEDIUM 1690 
#define JUMPING_ATTACK_MEDIUM 1858 + JUMP_ANIM

#define STATIC_ATTACK_BIG 1093 
#define MOVING_ATTACK_BIG 1227  
#define JUMPING_ATTACK_BIG 1382 + JUMP_ANIM

#define JUMP_MOMENTUM -400
#define KNOCKBACK_POWER 1000

#pragma endregion Pierre_me_les_casse

typedef enum
{
	PLAYER_LEFT,
	PLAYER_RIGHT,
}PlayerDirection;

typedef enum
{
	IDLE,
	RUN,
	JUMP,
	SLIDING,
	FALL,
	
	DAMAGE,
	DEAD,

	DASHING,
	CRAWLING,
	KNOCKBACK,

	STATIC_ATTACK,
	MOVING_ATTACK,
	JUMPING_ATTACK,

	
	
	NB_STATE
}PlayerState;

typedef struct
{
	sfBool isAlive;
	sfBool cheatMod;
	sfBool isPressingCheatMod;

	sfBool isActif;
	sfTexture* texture;
	sfSprite* sprite;
	sfRectangleShape* hitbox;
	//sfFloatRect hitbox;
	sfBool isSwapingWeapon;

	AnimationData animation[NB_STATE - 3];

	AnimationData animationAttack[3 * 3];
	AnimationData* currentAnimation;

	int HealthPoint;
	float timerInvinsible;

	float energy;


	//float cooldown;

	PlayerState state;
	PlayerDirection direction;


	float lastSpeedReached;
	float slow;
	float acceleration;
	sfVector2f velocity;

	PlayerDirection dirB4Jump;

	float cooldownJump;
	int nbJump;
	sfBool isJumping;
	sfBool canJump;

	sfBool IsBumped;


	int nbDash;

	float cooldownDash;
	float distanceDash;

	sfBool canDash;
	sfBool IsDashing;



	sfBool onGround;
	float onGroundTimer;

	//float stillOnTheGround;
	//sfBool canShoot;

	float timerSlide;
	sfBool canStand;
	sfBool canSlide;


	sfBool IsBulletTime;
	float  timeBulletTime;
	sfBool IsPressingBulletTime;

	sfBool enableGravity;

	sfBool IsAttackingPressed;

	//sfVector2f mousePosition;

	sfBool collisionHoriz;

	sfSprite* ThunderSprite;
	AnimationData ThunderAnimation;

}PlayerData;

void LoadPlayer(void);
void UpdatePlayer(sfRenderWindow* _window, float _dt);
void UpdatePlayerState(void);
void DrawPlayer(sfRenderWindow* _window);
void DrawPlayerHitbox(sfRenderWindow* _window);
void DrawPlayerHitboxOrigin(sfRenderWindow* _window);
void CleanupPlayer(void);
void KeypressedPlayer(sfKeyEvent _key);

void EditPlayerHP(int _editHP);

void SetPlayerState(PlayerState _state);
sfBool GetPlayerBulletTime();


const PlayerData* GetPlayer(void);
PlayerData* SetPlayer(void);

void SetPlayerActivity(sfBool _activity);
sfBool IsPlayerAlive();

float GetEnergy();
void IncreaseEnergy(float _energy);

void PlayerKnockback(float _knockbackPower, float _angle);

#endif

















































