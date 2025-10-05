#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "../Utility/Common.h"
#include "../Game/Game.h"
#include "../Utility/Mapping.h"
#include "Menu.h"
#include "../Utility/Transition.h"


typedef enum
{
	GAME_OVER_RETRY,
	GAME_OVER_MENU,
	GAME_OVER_QUIT,
	GAME_OVER_NB_BUTTON
}GameOverButtonName;

typedef struct
{
	sfText* text;
	GameOverButtonName name;
	sfBool mouseIsAbove;

}GameOverButton;

typedef struct
{
	sfTexture* textureCursor;
	sfSprite* spriteCursor;
	sfVector2f positionCursor;

	sfFont* font;
	//sfText* titleText;
	sfTexture* texture;
	sfSprite* sprite;
	GameOverButton button[GAME_OVER_NB_BUTTON];
	GameOverButtonName buttonPressed;
	sfVector2f mousePosition;

}GameOverData;

void LoadGameOver(void);
void MouseMovedGameOver(sfMouseMoveEvent _mouseMove);
void KeypressedGameOver(sfRenderWindow* _window, sfKeyEvent _key);
void UpdateGameOver(sfRenderWindow* _window, float _dt);
void DrawGameOver(sfRenderWindow* _window);
void CleanupGameOver(void);

#endif
