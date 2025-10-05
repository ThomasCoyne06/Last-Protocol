#ifndef LASER_H
#define LASER_H

#include "../Utility/Common.h"
#include "../Utility/LinkedList.h"
#include "../Resources/Sounds.h"
#include "Level.h"

#define LASER_TIMER_ON 2.0f
#define LASER_TIMER_OFF 0.9f

#define LASER_IDLE_FRAMES 7
#define LASER_FRAMERATE 10
#define LASER_VERTICAL_SIZE 20

#define LASER_HORIZONTAL_WIDTH 20
#define LASER_HORIZONTAL_HEIGHT 20

#define LASER_HORIZONTAL_HITBOX_SIZE_X 31
#define LASER_HORIZONTAL_HITBOX_SIZE_Y 60
#define LASER_HORIZONTAL_HITBOX_SHIFT 16

#define LASER_VERTICAL_HITBOX_SIZE_X 60
#define LASER_VERTICAL_HITBOX_SIZE_Y 31
#define LASER_VERTICAL_HITBOX_SHIFT 13

#define LASER_HITBOX_POS_X 8

typedef enum
{
	LASER_TYPE_HORIZONTAL,
	LASER_TYPE_VERTICAL
}LaserType;

typedef struct
{
	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData* currentAnimation;
	sfRectangleShape* hitbox;
}Laser;

typedef struct
{
	sfTexture* textureHorizontal;
	sfTexture* textureVertical;
	List* laserList;
	float timer;
	sfBool isOn;
}LaserData;

void LoadLaser(void);
void UpdateLaser(float _dt);
void DrawLaser(sfRenderWindow* _window);
void CleanupLaser(void);

void AddLaser(LaserType _laserType, sfVector2f _position);
unsigned GetLaserNb(void);
sfFloatRect GetLaser(unsigned _arrayPos);
sfBool GetLaserActivity();

#endif