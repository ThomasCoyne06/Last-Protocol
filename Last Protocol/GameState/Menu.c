#include "Menu.h"

void LoadMenuButton(void);
void DrawMenuButton(sfRenderWindow* _window);
void CleanupMenuButton(void);

void CollisionMouseButtonMenu(sfRenderWindow* _window);
void UpdateButtonPressed(sfRenderWindow* _window);

MenuData menu;

void LoadMenuButton(void)
{
	sfFont* font = sfFont_createFromFile("Assets/Fonts/GlobalFont.ttf");

	char textBuffer[MENU_NB_BUTTON][15];

	sprintf_s(textBuffer[MENU_PLAY], 5, "Play");
	sprintf_s(textBuffer[MENU_QUIT], 5, "Quit");

	sfVector2f position = { 0 };

	for (unsigned i = 0; i < MENU_NB_BUTTON; i++)
	{
		menu.button[i].text = CreateText(font, SCREEN_WIDTH / (MENU_NB_BUTTON + 1) * ((float)i + 1), SCREEN_HEIGHT / 2, 20 * SCALE, sfWhite, sfTrue, textBuffer[i]);
		menu.button[i].name = i;
	}
	menu.buttonPressed = MENU_NB_BUTTON;
}

void LoadMenu(void)
{
	LoadMenuButton();
}

void MouseMovedMenu(sfMouseMoveEvent _mouseMove)
{
	//menu.mousePosition = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };
	
}

void KeypressedMenu(sfRenderWindow* _window, sfKeyEvent _key)
{
	if (_key.code == sfKeyEscape)
	{
		sfRenderWindow_close(_window);
	}
	else if (_key.code == sfKeySpace)
	{
		SetGameState(GAME);
	}
}

void UpdateMenu(sfRenderWindow* _window, float _dt)
{
	CollisionMouseButtonMenu(_window);
	UpdateButtonPressed(_window);
}

void DrawMenuButton(sfRenderWindow* _window)
{
	SetDefaultCamera(_window);

	for (unsigned i = 0; i < MENU_NB_BUTTON; i++)
	{
		sfRenderWindow_drawText(_window, menu.button[i].text, NULL);
	}
}

void DrawMenu(sfRenderWindow* _window)
{
	DrawMenuButton(_window);
}

void CleanupMenuButton(void)
{
	for (unsigned i = 0; i < MENU_NB_BUTTON; i++)
	{
		sfText_destroy(menu.button[i].text);
		menu.button[i].text = NULL;

		menu.button[i].name = 0;
	}

}

void CleanupMenu(void)
{
	CleanupMenuButton();

	menu = (MenuData){ 0 };
	menu.buttonPressed = MENU_NB_BUTTON;
}

void CollisionMouseButtonMenu(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < MENU_NB_BUTTON; i++)
	{
		sfFloatRect textHitbox = sfText_getGlobalBounds(menu.button[i].text);
		if (IsCollidingPointRectangle(menu.mousePosition, textHitbox))
		{
			if (!menu.button[i].mouseIsAbove)
			{
				sfText_setColor(menu.button[i].text, sfColor_fromRGB(200, 30, 85));
				menu.button[i].mouseIsAbove = sfTrue;
			}
			if (sfMouse_isButtonPressed(sfMouseLeft))
			{
				switch (menu.button[i].name)
				{
				case MENU_PLAY:
					if (GetTransitionState(READY))
					{
						StartTransitionPicture(sfWhite, 2, 10);

						//StartTransitionFade(sfBlack, 5);
						menu.buttonPressed = MENU_PLAY;
					}
					break;
				case MENU_QUIT:
					if (GetTransitionState(READY))
					{
						StartTransitionIris(menu.mousePosition, sfBlack, 30);
						menu.buttonPressed = MENU_QUIT;
					}
					break;
				}
			}
		}
		else
		{
			sfText_setColor(menu.button[i].text, sfWhite);
			menu.button[i].mouseIsAbove = sfFalse;
		}
	}
}

void UpdateButtonPressed(sfRenderWindow* _window)
{
	switch (menu.buttonPressed)
	{
	case MENU_PLAY:
		if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(GAME);
		}
		break;


	case MENU_QUIT:
		if (GetTransitionState(COVER_SCREEN))
		{
			sfRenderWindow_close(_window);
		}
		break;
	}
}