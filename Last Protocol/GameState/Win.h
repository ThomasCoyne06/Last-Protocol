#ifndef WIN_H
#define WIN_H

#include "../Utility/Common.h"
#include "../Game/Game.h"
#include "../Utility/Transition.h"
#include "../Utility/Mapping.h"
#include "Menu.h"

#define WIN_WIDTH 640
#define WIN_HEIGHT 360

#define WIN_LIGHT_NB_FRAME 2
#define WIN_LIGHT_FRAMERATE 1

#define WIN_FIREWORK_NB_FRAME 18
#define WIN_FIREWORK_FRAMERATE 10

#define WIN_COW_WIDTH 75
#define WIN_COW_HEIGHT 49

#define WIN_COW_NB_FRAME 13
#define WIN_COW_FRAMERATE 8

#define WIN_COW_1_X 130
#define WIN_COW_2_X 1770
#define WIN_COW_Y 150


typedef enum
{
	WIN_RETRY,
	WIN_MENU,
	WIN_QUIT,
	WIN_NB_BUTTON
}WinButtonName;

typedef enum
{
	WIN_BOTTOM,
	WIN_LIGHT,
	WIN_FIREWORK,
	WIN_FRONT,
	WIN_CONGRAT,

	WIN_NB_LAYER
}WinLayer;

typedef struct
{
	sfText* text;
	WinButtonName name;
	sfBool mouseIsAbove;

}WinButton;

typedef struct
{
	sfTexture* textureList[WIN_NB_LAYER];
	sfSprite* spriteList[WIN_NB_LAYER];

	sfTexture* textureCursor;
	sfSprite* spriteCursor;
	sfVector2f positionCursorWin;

	AnimationData animLight;
	AnimationData animFirework;

	sfFont* font;
	sfText* textFileCollected;
	sfText* textBestFileCollected;

	sfText* textTime;
	sfText* textBestTime;

	sfText* textLifeLost;
	sfText* textBestLifeLost;

	WinButton button[WIN_NB_BUTTON];
	WinButtonName buttonPressed;

	sfVector2f mousePosition;

	//COW
	sfTexture* textureCow;
	sfSprite* spriteCow1;
	sfSprite* spriteCow2;
	AnimationData animCow1;
	AnimationData animCow2;

	sfMusic* endingMusic;

}WinData;

void LoadWin(void);
void MouseMovedWin(sfMouseMoveEvent _mouseMove);
void KeypressedWin(sfRenderWindow* _window, sfKeyEvent _key);
void UpdateWin(sfRenderWindow* _window, float _dt);
void DrawWin(sfRenderWindow* _window);
void CleanupWin(void);

#endif
