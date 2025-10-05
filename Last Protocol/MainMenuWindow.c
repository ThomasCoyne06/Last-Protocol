#include "MainMenuWindow.h"
#include "Utility/Mapping.h"

WindowBaseStruct windowStruct;

void closeWindow(void);

void ShowProg(void);
void ShowGraph(void);


void LoadWindow(void)
{

	char* windowBase[WINDOW_BASE_NB] = { "Exit1", "Smaller1" };
	char* windowOption[OPTION_SPRITE_NB] = { "Background" };
	char* windowCredits[CREDITS_SPRITE_NB] = { "CreditTab","CreditProg", "CreditGraph" };

	windowStruct.basePosition = (sfVector2f){ 250, 250 };
	windowStruct.content.menuActual = CREDITS;
	windowStruct.content.showGraph = sfFalse;

	for (unsigned int i = 0; i < WINDOW_BASE_NB; i++)
	{
		//change size and position for each button
		sfFloatRect size = { 0,0,0,0 };
		if (i == EXIT)
		{
			sfFloatRect sizeTemp = EXIT_FLOATRECT_VALUE;
			size = sizeTemp;
		}
		else if (i == REDUCE)
		{
			sfFloatRect sizeTemp = REDUCE_FLOATRECT_VALUE;
			size = sizeTemp;
		}
		windowStruct.hitbox[i] = CreateRectangle(size, sfFalse, sfRed, 2.0f, sfYellow);
	}

	char* fileName[50] = { 0 };
	//create sprite for the base of the window
	for (unsigned int i = 0; i < WINDOW_BASE_NB; i++)
	{
		windowStruct.sprite[i] = sfSprite_create();
		sprintf_s(fileName, sizeof(fileName), "Assets/Textures/Menu/MainMenu/%s.png", windowBase[i]);
		windowStruct.texture[i] = sfTexture_createFromFile(fileName, NULL);
		sfSprite_setTexture(windowStruct.sprite[i], windowStruct.texture[i], sfFalse);
		sfSprite_scale(windowStruct.sprite[i], (sfVector2f) { 3, 3 });
		sfSprite_setPosition(windowStruct.sprite[i], windowStruct.basePosition);
	}

	//create sprite for the option Menu
	for (unsigned int i = 0; i < OPTION_SPRITE_NB; i++)
	{
		windowStruct.content.optionSprite[i] = sfSprite_create();
		sprintf_s(fileName, sizeof(fileName), "Assets/Textures/Menu/MainMenu/Options/%s.png", windowOption[i]);
		windowStruct.content.optionTexture[i] = sfTexture_createFromFile(fileName, NULL);
		sfSprite_setTexture(windowStruct.content.optionSprite[i], windowStruct.content.optionTexture[i], sfFalse);
		sfSprite_scale(windowStruct.content.optionSprite[i], (sfVector2f) { 3, 3 });
		sfSprite_setPosition(windowStruct.content.optionSprite[i], windowStruct.basePosition);
	}

	//create sprite for the credits Menu
	for (unsigned int i = 0; i < CREDITS_SPRITE_NB; i++)
	{
		windowStruct.content.creditsSprite[i] = sfSprite_create();
		sprintf_s(fileName, sizeof(fileName), "Assets/Textures/Menu/MainMenu/Credits/%s.png", windowCredits[i]);
		windowStruct.content.creditsTexture[i] = sfTexture_createFromFile(fileName, NULL);
		sfSprite_setTexture(windowStruct.content.creditsSprite[i], windowStruct.content.creditsTexture[i], sfFalse);
		sfSprite_scale(windowStruct.content.creditsSprite[i], (sfVector2f) { 3, 3 });
		sfSprite_setPosition(windowStruct.content.creditsSprite[i], windowStruct.basePosition);
	}

	//creation of credits hitbox (clickable)
	for (unsigned int i = 0; i < TEAM; i++)
	{
		sfFloatRect pos = { 300.f + i * 15, 370.f + i * 25, 60.f + i * 15, 20.f };
		windowStruct.creditsHitbox[i] = CreateRectangle(pos, sfTrue, sfRed, 1.0f, sfYellow);
	}

	windowStruct.cursor = sfSprite_create();
	windowStruct.cursorTexture = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Home/Cursor.png", NULL);
	sfSprite_setTexture(windowStruct.cursor, windowStruct.cursorTexture, sfFalse);
	sfSprite_setScale(windowStruct.cursor, (sfVector2f) { 3.0f, 3.0f });

	windowStruct.border = sfSprite_create();
	windowStruct.borderTexture = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/ScreenBorder.png", NULL);
	sfSprite_setTexture(windowStruct.border, windowStruct.borderTexture, sfTrue);
	sfSprite_setScale(windowStruct.border, (sfVector2f) { 3.0f, 3.0f });

	closeWindow();
}

void UpdateWindow(float _dt, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window)
{

	sfVector2i position = sfMouse_getPositionRenderWindow(_window);
	//sfVector2f position2f = (sfVector2f){ position.x, position.y };

	for (unsigned int i = 0; i < WINDOW_BASE_NB; i++)
	{
		//if player click on exit or reduce, the window will be close
		if (IsCollidingPointRectangle(GetCursorPosition(), sfRectangleShape_getGlobalBounds(windowStruct.hitbox[i]))
			&& (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click)))
		{
			closeWindow();
		}
	}
	//collision mouse with button to change between graph and prog
	for (unsigned int i = 0; i < TEAM; i++)
	{
		if (IsCollidingPointRectangle(GetCursorPosition(), sfRectangleShape_getGlobalBounds(windowStruct.creditsHitbox[i]))
			&& (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click)))
		{
			if (i == 0)
			{
				ShowProg();
			}
			else
			{
				ShowGraph();
			}
		}
	}
	//sfSprite_setPosition(windowStruct.cursor, (sfVector2f) { _mouseMove.x, _mouseMove.y });
	//sfSprite_setPosition(windowStruct.cursor, (sfVector2f) { (float)position.x, (float)position.y });
}

void DrawWindow(sfRenderWindow* _window)
{
	if (windowStruct.content.windowClosed == sfFalse)
	{
		//draw a specific window
		switch (windowStruct.content.menuActual)
		{

			//show option window
		case OPTION:
			for (unsigned int i = 0; i < OPTION_SPRITE_NB; i++)
			{
				sfRenderWindow_drawSprite(_window, windowStruct.content.optionSprite[i], NULL);
			}
			break;

			//show credits window
		case CREDITS:
			for (unsigned int i = 0; i < CREDITS_SPRITE_NB; i++)
			{
				sfRenderWindow_drawSprite(_window, windowStruct.content.creditsSprite[BORDER], NULL);
				switch (windowStruct.content.showGraph)
				{
				case sfTrue:
					sfRenderWindow_drawSprite(_window, windowStruct.content.creditsSprite[GRAPH], NULL);
					break;
				case sfFalse:
					sfRenderWindow_drawSprite(_window, windowStruct.content.creditsSprite[PROG], NULL);
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		//Common to all windows
		for (unsigned int i = 0; i < WINDOW_BASE_NB; i++)
		{
			sfRenderWindow_drawSprite(_window, windowStruct.sprite[i], NULL);
		}

	}


	sfRenderWindow_drawSprite(_window, windowStruct.cursor, NULL);

}

void CleanupWindow(void)
{
	//cleanup base window and hitbox
	for (unsigned int i = 0; i < WINDOW_BASE_NB; i++)
	{
		sfRectangleShape_destroy(windowStruct.hitbox[i]);
		windowStruct.hitbox[i] = NULL;
		sfSprite_destroy(windowStruct.sprite[i]);
		windowStruct.sprite[i] = NULL;
	}

	for (unsigned int i = 0; i < TEAM; i++)
	{
		sfRectangleShape_destroy(windowStruct.creditsHitbox[i]);
		windowStruct.creditsHitbox[i] = NULL;
	}

	sfSprite_destroy(windowStruct.cursor);
	windowStruct.cursor = NULL;
	sfTexture_destroy(windowStruct.cursorTexture);
	windowStruct.cursorTexture = NULL;


	//cleanup Option window
	for (unsigned int i = 0; i < OPTION_SPRITE_NB; i++)
	{
		sfSprite_destroy(windowStruct.content.optionSprite[i]);
		windowStruct.content.optionSprite[i] = NULL;
		sfTexture_destroy(windowStruct.content.optionTexture[i]);
		windowStruct.content.optionTexture[i] = NULL;
	}

	//cleanup credits window
	for (unsigned int i = 0; i < CREDITS_SPRITE_NB; i++)
	{
		sfSprite_destroy(windowStruct.content.creditsSprite[i]);
		windowStruct.content.creditsSprite[i] = NULL;
		sfTexture_destroy(windowStruct.content.creditsTexture[i]);
		windowStruct.content.creditsTexture[i] = NULL;
	}
}

//change between each menu
void GenerateOptionMenu(void)
{
	windowStruct.content.menuActual = OPTION;
	windowStruct.content.windowClosed = sfFalse;

}

void GenerateCreditsMenu(void)
{
	windowStruct.content.menuActual = CREDITS;
	windowStruct.content.windowClosed = sfFalse;
}

sfVector2f GetCursorPosition(void)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(windowStruct.cursor);
	return (sfVector2f) { hitbox.left, hitbox.top };
}

void SetCursorPosition(sfVector2f _position)
{
	sfSprite_setPosition(windowStruct.cursor, _position);
}

void DrawBorderScreen(sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, windowStruct.border, NULL);

}

void closeWindow(void)
{	//base window
	windowStruct.content.windowClosed = sfTrue;
}

void ShowProg(void)
{
	windowStruct.content.showGraph = sfFalse;
}

void ShowGraph(void)
{
	windowStruct.content.showGraph = sfTrue;
}
