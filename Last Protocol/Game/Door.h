#ifndef DOOR_H
#define DOOR_H

#include "../Utility/Common.h"
#include "../Utility/LinkedList.h"
#include "Level.h"

#define DOOR_IDLE_FRAMES 6
#define DOOR_FRAMERATE 6
#define DOOR_SPRITE_WIDTH 546/6
#define DOOR_SPRITE_HEIGHT 60

#define HITBOX_SIZE_X 212
#define HITBOX_SIZE_Y 180

#define CROSSED_SIZE_X 10

typedef struct
{
	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData* currentAnimation;
	sfRectangleShape* hitbox;
	sfBool isOpen;
	sfFloatRect crossedHitbox;
}Door;

typedef struct
{
	sfTexture* texture;
	List* doorList;
}DoorData;

void LoadDoor(void);
void UpdateDoor(float _dt);
void DrawDoor(sfRenderWindow* _window);
void CleanupDoor(void);

void AddDoor(sfVector2f _position);
unsigned GetDoorNb(void);
sfFloatRect GetDoorHitbox(unsigned _arrayPos);
sfFloatRect GetDoorCrossedHitbox(unsigned _arrayPos);
void DoorIsOpen(void);

#endif