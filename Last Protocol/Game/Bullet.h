#ifndef SHOT_H
#define SHOT_H

#include "../Utility/Common.h"
#include "../Resources/Sounds.h"
#include "Miscellenous.h"

#define BULLET_SPEED 800
#define BULLET_MAX_TIMER 10

#define COLLISION_OFFSET_X 45
#define COLLISION_OFFSET_Y 65


typedef enum
{
	BULLET_BASIC,
	BULLET_BOSS,

	NB_BULLET_TYPE
}BulletType;

typedef struct
{
	sfSprite* sprite;
	sfVector2f velocity;
	AnimationData animationList[NB_BULLET_TYPE];
	BulletType type;
	float timer;
	sfBool isCreated;

	sfRectangleShape* hitbox;
	sfVector2f hitboxSize;

	sfCircleShape* circle;

	sfBool isExploded;

}BulletObj;

typedef struct
{
	sfTexture* texture;
	List* list;

}BulletData;

void AddBullet(BulletType _type, sfVector2f _startPosition, sfVector2f _targetPosition);

void LoadBullet(void);

void UpdateBullet(float _dt);

void DrawBullet(sfRenderWindow* _window);

void CleanupBullet(void);

void DestroyBullet(List* _list, unsigned _index);

const BulletData* GetBullet(void);

BulletData* SetBullet(void);

void DrawBulletHitbox(sfRenderWindow* _windows);


#endif