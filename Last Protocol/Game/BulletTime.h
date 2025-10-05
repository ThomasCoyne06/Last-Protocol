#ifndef BULLETTIME_H
#define BULLETTIME_H

#include "../Utility/Common.h"
#include "Player.h"

#define FRAME_RATE_BULLETTIME 45

typedef struct
{
	sfSprite* spriteCircle;
	sfTexture* textureCircle;

	sfSprite* spriteSquare;
	

	AnimationData animation;
	AnimationData animationSquare;

}BulletTimeData;

void LoadBulletTime();
void UpdateBulletTimeAnimation(float _dt, sfRenderWindow* _window);
void DrawBulletTimeAnimation(sfRenderWindow* _window);
void CleanUpBulletTimeAnimation();
#endif