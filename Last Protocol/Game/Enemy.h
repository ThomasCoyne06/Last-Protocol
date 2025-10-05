#ifndef ENEMY_H
#define ENEMY_H

#include "../Utility/Common.h"
#include "../Utility/Animation.h"
#include "../Utility/LinkedList.h"
#include "../Utility/FileReader.h"
#include "Player.h"
#include "Bullet.h"
#include "Level.h"

#define OFFSET_SHOOT_X 20.f
#define OFFSET_SHOOT_Y 140.f

#define BLINK_SPEED 0.1f
#define BLINK_ALPHA_MIN 100
#define BLINK_ALPHA_FULL 255

#define JUMP_FORCE 1200
#define JUMP_DETECT_RANGE 3
#define JUMP_DETECT_VELOCITY 100
#define JUMP_BONUS_SPEED 400
//#define KNOCKBACK_ENEMY_POWER 2000
#define KNOCKBACK_ENEMY_DURATION 0.1f


typedef enum
{
	ENEMY_TURN_LEFT = -1,
	ENEMY_TURN_RIGHT = 1,
}EnemyTurn;

typedef enum
{
	ENEMY_BASIC,
	ENEMY_TV,
	ENEMY_EYE,
	ENEMY_JUMPER,
	ENEMY_MINI,

	NB_ENEMY_TYPE
}EnemyType;

typedef enum
{
	ENEMY_IDLE,
	ENEMY_MOVE,
	ENEMY_DEAD,
	ENEMY_SHOOT,
	ENEMY_LANDING,
	ENEMY_JUMP,
	//ENEMY_KNOCKBACK,

	NB_ENEMY_STATE
}EnemyState;

typedef struct
{
	sfRectangleShape* hitbox;
	AnimationData currentAnimation;

	int life;
	sfVector2f velocity;
	sfVector2i colDir;
	sfVector2f jumpTarget;

	EnemyState state;
	EnemyType type;
	EnemyTurn turn;

	sfBool onGround;
	sfBool isDead;
	sfBool isInvincible;
	sfBool enableGravity;
	sfBool isJump;
	sfBool isKnockback;

	float explosionCountdown;
	float altitudeTimer;
	float shootTimer;
	float invulnerabilityTimer;
	float blinkTimer;
	float speed;
	float knockbackDuration;

	int alpha;

}EnemyObj;

typedef struct
{
	int maxLife;
	int myCrashDamage;
	int herCrashDamage;
	int shootDamage;
	float shootCooldown;
	float invulnerabilityCooldown;
	int explosionDamage;
	float explosionCountdownMax;
	float speed;
	float speedTakeoff;
	float altitudeMax;
	float detectRange;
	float agressiveRange;
	float fallVulnerability;
	sfVector2f hitboxSize;
	EnemyState startState;
	EnemyTurn startTurn;

}EnemyStatistic;

typedef struct
{
	sfBool detectPlayer;
	sfBool unDetectPlayer;
	sfBool followPlayerHoriz;
	sfBool followPlayerVerti;
	sfBool gravityAffect;
	sfBool roam;
	sfBool comeDown;
	sfBool shoot;
	sfBool jump;
	sfBool fly;
	sfBool fallDamage;
	sfBool explode;
	sfBool landing;

}EnemyBehavior;

typedef struct
{
	sfTexture* texture;
	List* list;

	sfRectangleShape* hitboxList[NB_ENEMY_TYPE];

	EnemyStatistic statsList[NB_ENEMY_TYPE];
	EnemyBehavior behaviorList[NB_ENEMY_TYPE];
	AnimationData animationList[NB_ENEMY_TYPE][NB_ENEMY_STATE];

}EnemyData;

void AddEnemy(EnemyType _type, sfVector2f _position);

void LoadEnemy(void);

void UpdateEnemy(float _dt);

void DrawEnemy(sfRenderWindow* _window);

void DrawEnemyHitbox(sfRenderWindow* _windows);

void DrawEnemyHitboxOrigin(sfRenderWindow* _window);

void CleanupEnemy(void);

void DestroyEnemy(EnemyObj* _enemy, unsigned _index);

void EditEnemyHP(EnemyObj* _enemy, int _editHP);



const EnemyData* GetEnemy(void);

EnemyData* SetEnemy(void);

void SetEnemyState(EnemyObj* _enemy, EnemyState _state);

void PlayEnemyDeathSound(EnemyType _enemyType);
void PlayEnemyHitSound(EnemyType _enemyType);
void EnemyKnockback(EnemyObj* _enemy, float _knockbackPower, float _angle);


#endif
