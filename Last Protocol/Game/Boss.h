#ifndef BOSS_H
#define BOSS_H

#include "../Utility/Common.h"
#include "Player.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Pickup.h"


#define DISTANCE_POSITION_FROM_CENTER 350
#define HEAD_SPEED 135
#define VERTICAL_SLAP_SPEED 1800
#define SHIFUMI_TIMER 1.0f
#define PREPARE_HAND 1.0f
#define WAITING_TIME 1.0f
#define DAMAGE_AGAIN_TIMER 0.55f

#define OFFSET_BOSS_SMOKE 65
#define GUN_SPEED 50
#define BULLET_MAX_FIRE 40

#define ARM_SPEED_GRAB 500

#define ACCEPTANCE_RADIUS 10

#define OFFSET_GRAB_Y 50
#define OFFSET_GRAB_X 50

#define STOP_TIME 2.0f

#define LEFT_HAND 0
#define RIGHT_HAND 1
#define HEAD_PART 3

#define HP_HAND 35 
#define HP_HEAD 25
#define DISTANCE_HAND_HEAD 250.0f
#define INVISIBILY_TIMER 0.3f

#define SHIFUMI_NBR_FRAME 9
#define EQUALITY_SHIFUMI_VALUE 2

#define DEATH_POSITION -200

#define SLAP_HAND_PREP_OFFSET 100
#define SLAP_HAND_BOTT_OFFSET 210

#define HEAD_HITBOX_OFFSET_X 10
#define HEAD_HITBOX_OFFSET_Y 25

#define DAMAGE_TO_BOSE_SMALL 1
#define DAMAGE_TO_BOSE_MEDIUM 3
#define DAMAGE_TO_BOSE_BIG 5

#define GRABBING_TIMER 3.0f
#define GUN_POSITION_X_DECAL 150
#define GUN_POSITION_Y_DECAL 0
#define BULLET_Y_POSITION - 50

typedef	enum
{
	PHASE_HAND,
	PHASE_HEAD,
	BOSS_DEFEATED,

	PHASE_NBR

}PhaseEnum;

typedef enum
{
	NO_ATTACK,
	VERTICAL_SLAP,
	SLAP_PREPARING,
	GUN,
	GUN_PREPARING,
	GRAB,
	GRAB_PREPARING,
	SHIFUMI_STATE,
	HAND_DEAD,
	SAD_HAND,


	ATTACK_NBR

}AttackHand;

typedef enum
{
	NO_SHIFUMI,
	ROCK,
	PAPER,
	SCISSORS,

	SHIFUMI_NBR
}ShifumiEnum;

typedef struct
{
	sfSprite* sprite;

	int hp;

	float canDamage;

	AnimationData beginRelease;
	AnimationData endRelease;
	AnimationData death;

}HeadStruct;

typedef struct
{
	sfSprite* sprite;

	int hp;

	AttackHand attack;
	ShifumiEnum shifumiValue;

	AnimationData idleAnimation;
	AnimationData clapVerticalAnimation;
	AnimationData shifumiAnim;
	AnimationData gunAnim;
	AnimationData deathAnim;

	AnimationData grabIdle;
	AnimationData grabing;
	AnimationData grab;

	AnimationData rockAnim;
	AnimationData paperAnim;
	AnimationData scissorsAnim;
	AnimationData loseAnim;

	AttackHand state;

	float canDamage;

}HandStruct;

typedef struct
{

	sfRectangleShape* leftHandSlapHitbox;
	sfRectangleShape* rightHandSlapHitbox;

	sfRectangleShape* leftHandDamageHitbox;
	sfRectangleShape* rightHandDamageHitbox;

	sfRectangleShape* leftGrabHitbox;
	sfRectangleShape* rightGrabHitbox;

	sfRectangleShape* headHitbox;

}HitboxStruct;

typedef struct
{
	sfBool isInvincible;
	float blinkTimer;
	int alpha;
	float timer;

}Blink182;

typedef struct
{
	HeadStruct head;

	HandStruct handLeft;
	HandStruct handRight;

	PhaseEnum phase;

	sfTexture* texture;

	sfVector2f position[4];
	sfVector2f movement;

	sfBool isArrivedX;
	sfBool isArrivedY;

	float timeStop;
	float handEndShifumi;
	float prepareHand;
	float waiting;
	float canGrab;

	int bulletAmount;
	int positionGoal;
	int counter;

	sfBool startAction;

	sfBool shifumiOver;
	sfBool handReady;
	sfBool actionOver;


	HitboxStruct hitbox;
	Blink182 blink[3];


}BossStruct;


void LoadBoss(void);
void UpdateBoss(float _dt);
void DrawBoss(sfRenderWindow* _window);
void CleanupBoss(void);

void HeadMovement(float _dt);
void SlapVertical(float _dt, sfSprite* _handSprite);

PhaseEnum GetActualPhase(void);

int GetLifeOfHand(int _hand);
int GetLifeOfHead(void);



#endif // !BOSS_H

