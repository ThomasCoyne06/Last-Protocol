 #ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../Utility/Common.h"
#include "../Utility/Camera.h"

#define PARALAX_OFFSET_Y  0//1860
#define PARALAX_WIDTH 3841
#define PARALAX_HEIGHT 360

#define PARALAX_NB_FRAME 11
#define PARALAX_FRAME_RATE 10

#define PARALAX_1_SPEED 44
#define PARALAX_2_SPEED 46
#define PARALAX_3_SPEED 48

#define LV2_PARALAX_WIDTH 640
#define LV2_PARALAX_HEIGHT 1440

#define LV2_BOTTOM_SPEED -46
#define LV2_PARALAX_1_SPEED -40
#define LV2_PARALAX_2_SPEED -42
#define LV2_PARALAX_3_SPEED -44

#define TREE_WIDTH 227
#define TREE_HEIGHT 309
#define TREE_IDLE_NB_FRAME 11
#define TREE_IDLE_FRAMERATE 10

#define TREE_LOL_NB_FRAME 11
#define TREE_LOL_FRAMERATE 10

#define RECTANGLE_RED_SPEED 200
#define RECTANGLE_RED_HEIGHT 50

typedef enum
{
	PARALAX_3,
	PARALAX_2,
	PARALAX_1,

	NB_PARALAX
}BackgroundType;

typedef enum
{
	IDLE_TREE,
	BLINK_TREE,

	NB_TREE_STATE
}TreeState;

typedef struct
{
	sfTexture* texture;
	sfSprite* sprite;
	sfSprite* copySprite;
	AnimationData animation;
	AnimationData copyAnimation;

	sfVector2f speed;

}ParalaxData;

typedef struct
{
	sfTexture* textureBottom;
	sfTexture* textureDegrade;
	sfTexture* textureLight1;
	sfTexture* textureLight2;
	sfTexture* textureTree;
	ParalaxData paralax[NB_PARALAX];
	sfSprite* spriteBottom;
	sfSprite* spriteDegrade;
	sfSprite* spriteLight1;
	sfSprite* spriteLight2;
	sfSprite* spriteTree;
	LevelNumber levelNumber;
	float bottomSpeed;
	AnimationData animTree[NB_TREE_STATE];
	AnimationData* currentAnimTree;
	sfRectangleShape* rectangleRed;
	float rectRedSpeed;

}BackgroundData;

void LoadBackground(LevelNumber _levelNumber);

void UpdateBackground(sfRenderWindow* _window, float _dt);

void DrawBackground(sfRenderWindow* _windows);

void CleanupBackground();

#endif
