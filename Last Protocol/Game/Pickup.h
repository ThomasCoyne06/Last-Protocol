#ifndef PICKUP_H
#define PICKUP_H

#include "../Utility/Common.h"
#include "../Utility/LinkedList.h"
#include "Level.h"
#include "Player.h"
#include "HUD.h"
#include "../Stats.h"


#define FILE_SPRITE_SIZE 20

#define FILE_IDLE_FRAMES 4
#define FILE_IDLE_FRAMERATE 5

#define FILE_DESTROY_FRAME 7
#define FILE_DESTROY_FRAMERATE 13

#define LIFE_SPRITE_SIZE_X 23
#define LIFE_SPRITE_SIZE_Y 36

#define LIFE_IDLE_FRAMES 5
#define LIFE_IDLE_FRAMERATE 7

#define LIFE_DESTROY_FRAME 5
#define LIFE_DESTROY_FRAMERATE 13

#define ENERGY_FILE 3

#define PICKUP_MOVE_SPEED 1500


typedef enum
{
	P_FILE,
	P_LIFE,
	BONUS_NB
}PickupType;

typedef struct
{
	PickupType type;
	sfTexture* texture;
	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData destroyAnimation;
	AnimationData* currentAnimation;
	sfBool isExploding;
}Pickup;

typedef struct
{
	List* pickupList;
}PickupData;

void LoadPickup(void);
void UpdatePickup(float _dt);
void DrawPickup(sfRenderWindow* _window);
void DrawObtainedPickup(sfRenderWindow* _window);
void CleanupPickup(void);

void AddPickup(PickupType _type, sfVector2f _position);
unsigned GetPickupNb(void);
sfFloatRect GetPickup(unsigned _arrayPos);
int GetPickupType(unsigned _arrayPos);
void ObtainPickup(unsigned _arrayPos);

#endif