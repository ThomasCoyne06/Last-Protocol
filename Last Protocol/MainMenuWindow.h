#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

//include
#include "../Last Protocol/Utility/Common.h"
#include "Utility/Collision.h"
#include "Utility/Transition.h"

//define
#define OUT_OF_RENDER -1500
#define EXIT_FLOATRECT_VALUE { 685, 255, 15, 15 }
#define REDUCE_FLOATRECT_VALUE { 635, 255, 15, 15 }

//graph/prog
#define TEAM 2

//enum and struct
typedef enum
{
	OPTION,
	CREDITS,

	NB_MENU
}MenuChooseEnum;

typedef enum
{
	BORDER,
	PROG,
	GRAPH,

	CREDITS_SPRITE_NB
}SpriteCreditsNameEnum;

typedef enum
{
	TEST,

	OPTION_SPRITE_NB
}OptionNameEnum;

typedef	enum
{
	EXIT,
	REDUCE,

	WINDOW_BASE_NB

}WindowBaseComponentEnum;

typedef struct
{
	sfSprite* optionSprite[OPTION_SPRITE_NB];
	sfTexture* optionTexture[OPTION_SPRITE_NB];

	sfSprite* creditsSprite[CREDITS_SPRITE_NB];
	sfTexture* creditsTexture[CREDITS_SPRITE_NB];

	MenuChooseEnum menuActual;
	sfBool windowClosed;
	sfBool showGraph;
}WindowContentStruct;


typedef struct
{
	sfSprite* sprite[WINDOW_BASE_NB];
	sfTexture* texture[WINDOW_BASE_NB];

	sfSprite* border;
	sfTexture* borderTexture;

	sfVector2f basePosition;
	WindowContentStruct content;

	sfRectangleShape* hitbox[WINDOW_BASE_NB];
	sfRectangleShape* creditsHitbox[TEAM];

	sfSprite* cursor;
	sfTexture* cursorTexture;


}WindowBaseStruct;

//prototype function
void LoadWindow(void);
void UpdateWindow(float _dt, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window);
void DrawWindow(sfRenderWindow* _window);
void CleanupWindow(void);

void GenerateOptionMenu(void);
void GenerateCreditsMenu(void);

sfVector2f GetCursorPosition(void);
void SetCursorPosition(sfVector2f _position);
void DrawBorderScreen(sfRenderWindow* _window);


#endif // !MAINMENUWINDOW_H
