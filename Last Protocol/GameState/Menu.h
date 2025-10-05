#ifndef MENU_H
#define MENU_H

#include "../Utility/Common.h"
#include "../Game/Game.h"
#include "../Utility/Transition.h"

typedef enum
{
	MENU_PLAY,
	MENU_QUIT,

	MENU_NB_BUTTON

}MenuButtonName;

typedef struct
{
	sfText* text;
	MenuButtonName name;
	sfBool mouseIsAbove;

}MenuButton;

typedef struct
{
	MenuButton button[MENU_NB_BUTTON];
	MenuButtonName buttonPressed;
	sfVector2f mousePosition;
}MenuData;

void LoadMenu(void);
void MouseMovedMenu(sfMouseMoveEvent _mouseMove);
void UpdateMenu(sfRenderWindow* _window, float);
void DrawMenu(sfRenderWindow*);
void CleanupMenu(void);
void KeypressedMenu(sfRenderWindow* _window, sfKeyEvent _key);

#endif
