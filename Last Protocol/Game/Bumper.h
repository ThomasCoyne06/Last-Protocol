#ifndef BUMPER_H
#define BUMPER_H

#include "../Utility/Common.h"
#include "../Utility/LinkedList.h"
#include "Level.h"

#define BUMPER_IDLE_FRAMES 5
#define BUMPER_BUMP_FRAMES 5

#define BUMPER_IDLE_FRAMERATE 6
#define BUMPER_BUMP_FRAMERATE 20

#define BUMPER_SPRITE_WIDTH 20
#define BUMPER_SPRITE_HEIGHT 40

#define BUMPER_ADJUSTING_POS 20

typedef struct
{
	sfTexture* texture;
	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData bumpAnimation;
	AnimationData* currentAnimation;
	sfBool isBumping;
}Bumper;

typedef struct
{
	sfTexture* texture;
	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData* currentAnimation;
	List* bumperList;
}BumperData;

void LoadBumper();
void UpdateBumper(float _dt);
void DrawBumper(sfRenderWindow* _window);
void CleanupBumper();

void AddBumper(sfVector2f _bumperPosition);
void ActivateBumper(unsigned _arrayPos);
unsigned GetBumperNb(void);
sfFloatRect GetBumperHitbox(unsigned _arrayPos);
sfBool GetBumperActivity(unsigned _arrayPos);

#endif
