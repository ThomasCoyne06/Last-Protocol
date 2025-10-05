#ifndef PROPS_H
#define PROPS_H

#include "../Utility/Common.h"
#include "../Utility/Animation.h"
#include "../Utility/LinkedList.h"
#include "Level.h"

#define STREET_LIGHT_FLICKER_MAX_FRAMERATE 12
#define MAX_RANDOM 2
typedef enum
{
	PROPS_GRASS_1,
	PROPS_GRASS_2,
	PROPS_LIGHT_CEILING,
	PROPS_SLIDE_SIGN,
	PROPS_JUMP_SIGN,
	PROPS_RUN_LEFT_SIGN,
	PROPS_RUN_RIGHT_SIGN,
	PROPS_UP_SIGN,
	PROPS_RIGHT_SIGN,
	PROPS_DOWN_SIGN,
	PROPS_LEFT_SIGN,
	PROPS_DEAD_ROBOT,
	PROPS_STREET_LIGHT,
	PROPS_VINE_1,
	PROPS_VINE_2,
	PROPS_VINE_3,

	NB_PROPS_TYPE
}PropsType;

typedef struct
{
	sfSprite* sprite;

	AnimationData animationList[NB_PROPS_TYPE];
	PropsType type;


	float maxTimer;
	float flickerTimer;

	sfRectangleShape* hitbox;
	sfVector2f hitboxSize;


	sfBool isInteractible;
	sfBool isUpFront;
	sfBool animationPlayed;

}PropsObj;

typedef struct
{
	sfTexture* texture;
	List* list;

}PropsData;

void LoadProps(LevelNumber _levelNumber);
void UpdateProps(float _dt);
void DrawProps(sfRenderWindow* _window);
void CleanupProps(void);
void DestroyProps(List* _list, unsigned _index);
const PropsData* GetProps(void);
PropsData* SetProps(void);

void DrawPropsHitbox(sfRenderWindow* _windows);



#endif // !MISCELLENOUS_H