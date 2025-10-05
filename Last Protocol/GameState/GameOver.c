#include "GameOver.h"

void LoadGameOverTitle(void);
void LoadGameOverButton(void);

void CollisionMouseButtonGameOver(void);
void UpdateGameOverButtonPressed(sfRenderWindow* _window);
void ControllerGestionGameOver(float _dt,sfRenderWindow* _window);
void MouseGestionGameOver(sfRenderWindow* _window);

GameOverData gameOver;

void LoadGameOverTitle(void)
{
	//char textBuffer[10];

	//sprintf_s(textBuffer, 10, "GAME OVER");
	//gameOver.titleText = CreateText(gameOver.font, SCREEN_WIDTH / 2, SCREEN_HEIGHT/3, 25 * SCALE, sfWhite, sfTrue, textBuffer);
}

void LoadGameOverButton(void)
{
	char textBuffer[GAME_OVER_NB_BUTTON][10];

	sprintf_s(textBuffer[GAME_OVER_RETRY], 6, "Retry");
	sprintf_s(textBuffer[GAME_OVER_MENU], 5, "Menu");
	sprintf_s(textBuffer[GAME_OVER_QUIT], 5, "Quit");

	for (unsigned i = 0; i < GAME_OVER_NB_BUTTON; i++)
	{
		gameOver.button[i].text = CreateText(gameOver.font, SCREEN_WIDTH / (GAME_OVER_NB_BUTTON + 1) * ((float)i + 1), SCREEN_HEIGHT - 50, 20 * SCALE, sfWhite, sfTrue, textBuffer[i]);
		gameOver.button[i].name = i;
	}
	gameOver.buttonPressed = GAME_OVER_NB_BUTTON;

	gameOver.textureCursor = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Home/Cursor.png", NULL);
	gameOver.spriteCursor = CreateSimpleSprite(gameOver.textureCursor, SCALE);

	fileCollected = 0;
	fileSave = 0;
	crono = 0;
	lifeLost = 0;
	gameOver.positionCursor = (sfVector2f){0,0};
}

void LoadGameOver(void)
{
	StopMusic();

	gameOver.texture = sfTexture_createFromFile("Assets/Textures/Game Over/Game Over.png", NULL);
	gameOver.sprite = CreateSimpleSprite(gameOver.texture, SCALE);

	gameOver.font = sfFont_createFromFile("Assets/Fonts/GlobalFont.ttf");
	LoadGameOverTitle();
	LoadGameOverButton();
}

void MouseMovedGameOver(sfMouseMoveEvent _mouseMove)
{
	//gameOver.mousePosition = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };
	//sfSprite_setPosition(gameOver.spriteCursor, gameOver.mousePosition);
	gameOver.positionCursor = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };

}

void KeypressedGameOver(sfRenderWindow* _window, sfKeyEvent _key)
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

void UpdateGameOver(sfRenderWindow* _window, float _dt)
{

	
	sfJoystick_update();
	if (sfJoystick_isConnected(0))
	{
		//joystick
		ControllerGestionGameOver(_dt, _window);
		
		
	}
	
	
	sfSprite_setPosition(gameOver.spriteCursor, gameOver.positionCursor);

	sfRenderWindow_setMouseCursorVisible(_window, sfFalse);
	SetDefaultCamera(_window);

	CollisionMouseButtonGameOver();
	UpdateGameOverButtonPressed(_window);
}

void DrawGameOverButton(sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, gameOver.sprite, NULL);
	for (unsigned i = 0; i < GAME_OVER_NB_BUTTON; i++)
	{
		sfRenderWindow_drawText(_window, gameOver.button[i].text, NULL);
	}
}

void DrawGameOver(sfRenderWindow* _window)
{
	//sfRenderWindow_drawText(_window, gameOver.titleText, NULL);
	DrawGameOverButton(_window);
	sfRenderWindow_drawSprite(_window, gameOver.spriteCursor, NULL);

}

void CleanupGameOverButton(void)
{
	for (unsigned i = 0; i < GAME_OVER_NB_BUTTON; i++)
	{
		sfText_destroy(gameOver.button[i].text);
		gameOver.button[i].text = NULL;
	}
}

void CleanupGameOver(void)
{
	//sfText_destroy(gameOver.titleText);
	//gameOver.titleText = NULL;

	CleanupGameOverButton();

	gameOver.buttonPressed = GAME_OVER_NB_BUTTON;

}

void CollisionMouseButtonGameOver(void)
{
	for (unsigned i = 0; i < GAME_OVER_NB_BUTTON; i++)
	{
		sfFloatRect textHitbox = sfText_getGlobalBounds(gameOver.button[i].text);
		if (IsCollidingPointRectangle(sfSprite_getPosition(gameOver.spriteCursor), textHitbox))
		{
			//sfText_setColor(.button[i].text, sfColor_fromRGB(155, 0, 155));
			if (!gameOver.button[i].mouseIsAbove)
			{
				sfText_setColor(gameOver.button[i].text, sfColor_fromRGB(200, 30, 85));
				gameOver.button[i].mouseIsAbove = sfTrue;
			}
			if (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click))
			{
				switch (gameOver.button[i].name)
				{
				case GAME_OVER_RETRY:
					if (GetTransitionState(READY))
					{
						//StartTransitionFade(sfBlack, 5);
						StartTransitionPicture(sfWhite, 2, 10);
						gameOver.buttonPressed = GAME_OVER_RETRY;
					}
					break;
				case GAME_OVER_MENU:
					if (GetTransitionState(READY))
					{
						//StartTransitionFade(sfBlack, 5);
						StartTransitionPicture(sfWhite, 1,10);
						gameOver.buttonPressed = GAME_OVER_MENU;
					}
					break;

				case GAME_OVER_QUIT:
					if (GetTransitionState(READY))
					{
						StartTransitionIris(gameOver.mousePosition, sfBlack, 30);
						gameOver.buttonPressed = GAME_OVER_QUIT;
					}
					break;
				}
			}
		}
		else
		{
			sfText_setColor(gameOver.button[i].text, sfWhite);
			gameOver.button[i].mouseIsAbove = sfFalse;
		}
	}
}

void UpdateGameOverButtonPressed(sfRenderWindow* _window)
{
	switch (gameOver.buttonPressed)
	{
	case GAME_OVER_RETRY:
		if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(GAME);

		}
		break;

	case GAME_OVER_MENU:
		if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(MENU);
		}
		break;

	case GAME_OVER_QUIT:
		if (GetTransitionState(COVER_SCREEN))
		{
			sfRenderWindow_close(_window);
		}
		break;
	}
}

void MouseGestionGameOver(sfRenderWindow* _window)
{
	gameOver.positionCursor.x = (float)sfMouse_getPositionRenderWindow(_window).x;
	gameOver.positionCursor.y = (float)sfMouse_getPositionRenderWindow(_window).y;
}

void ControllerGestionGameOver(float _dt, sfRenderWindow* _window)
{

	if (CheckMapping(GetMappingMenu()->left) && gameOver.positionCursor.x > 0)
	{
		gameOver.positionCursor.x -= 300 * _dt;
	}
	if (CheckMapping(GetMappingMenu()->right) && gameOver.positionCursor.x < SCREEN_WIDTH)
	{
		gameOver.positionCursor.x += 300 * _dt;
	}
	if (CheckMapping(GetMappingMenu()->up) && gameOver.positionCursor.y > 0)
	{
		gameOver.positionCursor.y -= 300 * _dt;
	}
	if (CheckMapping(GetMappingMenu()->down) && gameOver.positionCursor.y < SCREEN_HEIGHT)
	{
		gameOver.positionCursor.y += 300 * _dt;
	}
}