#include "Win.h"

void LoadWinSprite(void);
void LoadWinButton(void);
void LoadCowSprite(void);
void MouseGestionWin(sfRenderWindow* _window);
void ControllerGestionWin(float _dt, sfRenderWindow* _window);
void DrawWinButton(sfRenderWindow* _window);
void CleanupWinButton(void);

void CollisionMouseButtonWin(void);
void UpdateWinButtonPressed(sfRenderWindow* _window);

WinData win;

void LoadWinSprite(void)
{
	win.textureList[WIN_BOTTOM] = sfTexture_createFromFile("Assets/Textures/Win/BottomWin.png", NULL);
	win.textureList[WIN_LIGHT] = sfTexture_createFromFile("Assets/Textures/Win/Light.png", NULL);
	win.textureList[WIN_FIREWORK] = sfTexture_createFromFile("Assets/Textures/Win/Firework.png", NULL);
	win.textureList[WIN_FRONT] = sfTexture_createFromFile("Assets/Textures/Win/FrontWin.png", NULL);
	win.textureList[WIN_CONGRAT] = sfTexture_createFromFile("Assets/Textures/Win/Congrat.png", NULL);

	for (unsigned i = 0; i < WIN_NB_LAYER; i++)
	{
		win.spriteList[i] = CreateSimpleSprite(win.textureList[i], SCALE);
	}

	sfIntRect rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	win.animLight = CreateAnimation(win.spriteList[WIN_LIGHT], WIN_LIGHT_NB_FRAME, WIN_LIGHT_FRAMERATE, rect, sfTrue);
	win.animFirework = CreateAnimation(win.spriteList[WIN_FIREWORK], WIN_FIREWORK_NB_FRAME, WIN_FIREWORK_FRAMERATE, rect, sfTrue);

	win.textureCursor = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Home/Cursor.png", NULL);
	win.spriteCursor = CreateSimpleSprite(win.textureCursor, SCALE);
}

void LoadWinButton(void)
{
	char textBuffer[WIN_NB_BUTTON][10];

	sprintf_s(textBuffer[WIN_RETRY], 6, "Retry");
	sprintf_s(textBuffer[WIN_MENU], 5, "Menu");
	sprintf_s(textBuffer[WIN_QUIT], 5, "Quit");

	for (unsigned i = 0; i < WIN_NB_BUTTON; i++)
	{
		win.button[i].text = CreateText(win.font, SCREEN_WIDTH / (WIN_NB_BUTTON + 1) * ((float)i + 1), SCREEN_HEIGHT - 50, 20 * SCALE, sfWhite, sfTrue, textBuffer[i]);
		win.button[i].name = i;
	}
	win.buttonPressed = WIN_NB_BUTTON;

}

void LoadCowSprite(void)
{
	win.textureCow = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Popup/Cow.png", NULL);
	
	win.spriteCow1 = sfSprite_create();
	win.spriteCow2 = sfSprite_create();

	sfSprite_setTexture(win.spriteCow1, win.textureCow, sfTrue);
	sfSprite_setTexture(win.spriteCow2, win.textureCow, sfTrue);

	sfSprite_setScale(win.spriteCow1, (sfVector2f) { SCALE, SCALE });
	sfSprite_setScale(win.spriteCow2, (sfVector2f) { -SCALE, SCALE });

	sfSprite_setPosition(win.spriteCow1, (sfVector2f) { WIN_COW_1_X , WIN_COW_Y});
	sfSprite_setPosition(win.spriteCow2, (sfVector2f) { WIN_COW_2_X, WIN_COW_Y });

	sfIntRect rect = { 0, 0, WIN_COW_WIDTH, WIN_COW_HEIGHT };
	win.animCow1 = CreateAnimation(win.spriteCow1, WIN_COW_NB_FRAME, WIN_COW_FRAMERATE, rect, sfTrue);
	win.animCow2 = CreateAnimation(win.spriteCow2, WIN_COW_NB_FRAME, WIN_COW_FRAMERATE, rect, sfTrue);
}

void LoadWin(void)
{
	win.font = sfFont_createFromFile("Assets/Fonts/GlobalFont.ttf");
	LoadWinSprite();
	LoadWinButton();
	LoadCowSprite();

	win.endingMusic = sfMusic_createFromFile("Assets/Musics/EndMenu.wav");
	StopMusic();
	sfMusic_setLoop(win.endingMusic, sfTrue);
	sfMusic_play(win.endingMusic);
	

	static sfBool boolean;
	if (!boolean)
	{
		bestFileCollected = fileSave;
		bestCrono = crono;
		bestLifeLost = lifeLost;
		boolean = sfTrue;
	}
	else
	{
		bestFileCollected = fileSave > bestFileCollected ? fileSave : bestFileCollected;
		bestCrono = crono < bestCrono ? crono : bestCrono;
		bestLifeLost = lifeLost < bestLifeLost ? lifeLost : bestLifeLost;
	}


	char textBuffer[50];
	sprintf_s(textBuffer, 49, "Files Collected : %d", fileSave);
	win.textFileCollected = CreateText(win.font, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 3, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	sprintf_s(textBuffer, 49, "Best  : %d", bestFileCollected);
	win.textBestFileCollected = CreateText(win.font, SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 3, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	sprintf_s(textBuffer, 49, "Time : %02d:%02d", (int)crono / 60, (int)crono % 60);
	win.textTime = CreateText(win.font, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 2, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	sprintf_s(textBuffer, 49, "Best : %02d:%02d", (int)bestCrono / 60, (int)bestCrono % 60);
	win.textBestTime = CreateText(win.font, SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 2, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	sprintf_s(textBuffer, 49, "Lifes Lost : %d", lifeLost);
	win.textLifeLost = CreateText(win.font, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 1.5f, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	sprintf_s(textBuffer, 49, "Best : %d", bestLifeLost);
	win.textBestLifeLost = CreateText(win.font, SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 1.5f, 25 * SCALE, sfBlack, sfFalse, textBuffer);

	fileCollected = 0;
	fileSave = 0;
	crono = 0;
	lifeLost = 0;
}

void MouseMovedWin(sfMouseMoveEvent _mouseMove)
{
	//win.mousePosition = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };
	//sfSprite_setPosition(win.spriteCursor, win.mousePosition);
	win.positionCursorWin = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };
}

void KeypressedWin(sfRenderWindow* _window, sfKeyEvent _key)
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

void UpdateWin(sfRenderWindow* _window, float _dt)
{
	sfJoystick_update();
	if (sfJoystick_isConnected(0))
	{
		//joystick
		ControllerGestionWin(_dt, _window);
		
	}
	//mouse

	
	sfSprite_setPosition(win.spriteCursor, win.positionCursorWin);
	CollisionMouseButtonWin();
	UpdateWinButtonPressed(_window);

	UpdateAnimation(&win.animLight, _dt);
	UpdateAnimation(&win.animFirework, _dt);
	UpdateAnimation(&win.animCow1, _dt);
	UpdateAnimation(&win.animCow2, _dt);
}

void DrawWinButton(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < WIN_NB_BUTTON; i++)
	{
		sfRenderWindow_drawText(_window, win.button[i].text, NULL);
	}
}

void DrawWin(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < WIN_NB_LAYER; i++)
	{
		sfRenderWindow_drawSprite(_window, win.spriteList[i], NULL);
	}

	sfRenderWindow_drawText(_window, win.textFileCollected, NULL);
	sfRenderWindow_drawText(_window, win.textBestFileCollected, NULL);

	sfRenderWindow_drawText(_window, win.textTime, NULL);
	sfRenderWindow_drawText(_window, win.textBestTime, NULL);

	sfRenderWindow_drawText(_window, win.textLifeLost, NULL);
	sfRenderWindow_drawText(_window, win.textBestLifeLost, NULL);

	sfRenderWindow_drawSprite(_window, win.spriteCow1, NULL);
	sfRenderWindow_drawSprite(_window, win.spriteCow2, NULL);

	DrawWinButton(_window);

	sfRenderWindow_drawSprite(_window, win.spriteCursor, NULL);
}

void CleanupWinButton(void)
{
	for (unsigned i = 0; i < WIN_NB_BUTTON; i++)
	{
		sfText_destroy(win.button[i].text);
		win.button[i].text = NULL;
	}
}

void CleanupWin(void)
{
	for (unsigned i = 0; i < WIN_NB_LAYER; i++)
	{
		sfSprite_destroy(win.spriteList[i]);
	}

	sfText_destroy(win.textFileCollected);
	sfText_destroy(win.textBestFileCollected);
	sfText_destroy(win.textTime);
	sfText_destroy(win.textBestTime);
	sfText_destroy(win.textLifeLost);
	sfText_destroy(win.textBestLifeLost);

	CleanupWinButton();
	
	sfMusic_destroy(win.endingMusic);

	win.buttonPressed = WIN_NB_BUTTON;

	win = (WinData){ 0 };
}

void CollisionMouseButtonWin()
{
	for (unsigned i = 0; i < WIN_NB_BUTTON; i++)
	{
		sfFloatRect textHitbox = sfText_getGlobalBounds(win.button[i].text);
		if (IsCollidingPointRectangle(sfSprite_getPosition(win.spriteCursor), textHitbox))
		{
			if (!win.button[i].mouseIsAbove)
			{
				sfText_setColor(win.button[i].text, sfColor_fromRGB(200, 30, 85));
				win.button[i].mouseIsAbove = sfTrue;
			}
			if (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click))
			{
				switch (win.button[i].name)
				{
				case WIN_RETRY:
					if (GetTransitionState(READY))
					{
						StartTransitionPicture(sfWhite, 2, 10);

						win.buttonPressed = WIN_RETRY;
					}
					break;
				case WIN_MENU:
					if (GetTransitionState(READY))
					{
						StartTransitionPicture(sfWhite, 2, 10);

						win.buttonPressed = WIN_MENU;
					}
					break;

				case WIN_QUIT:
					if (GetTransitionState(READY))
					{
						StartTransitionIris(win.mousePosition, sfBlack, 30);
						win.buttonPressed = WIN_QUIT;
					}
					break;
				}
			}
		}
		else
		{
			sfText_setColor(win.button[i].text, sfWhite);
			win.button[i].mouseIsAbove = sfFalse;

		}
	}
}

void UpdateWinButtonPressed(sfRenderWindow* _window)
{
	switch (win.buttonPressed)
	{
	case WIN_RETRY:
		if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(GAME);
		}
		break;

	case WIN_MENU:
		if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(MENU);
		}
		break;

	case WIN_QUIT:
		if (GetTransitionState(COVER_SCREEN))
		{
			sfRenderWindow_close(_window);
		}
		break;
	}
}

void MouseGestionWin(sfRenderWindow* _window)
{
	win.positionCursorWin.x = (float)sfMouse_getPositionRenderWindow(_window).x;
	win.positionCursorWin.y = (float)sfMouse_getPositionRenderWindow(_window).y;
}

void ControllerGestionWin(float _dt, sfRenderWindow* _window)
{
	
	
	if (CheckMapping(GetMappingMenu()->left) && win.positionCursorWin.x > 0)
	{
		win.positionCursorWin.x -= 300 * _dt;
	}
	if (CheckMapping(GetMappingMenu()->right) && win.positionCursorWin.x < SCREEN_WIDTH)
	{
		win.positionCursorWin.x += 300 * _dt; 
		
	}
	if (CheckMapping(GetMappingMenu()->up) && win.positionCursorWin.y > 0)
	{
		win.positionCursorWin.y -= 300 * _dt;
	}
	if (CheckMapping(GetMappingMenu()->down) && win.positionCursorWin.y < SCREEN_HEIGHT)
	{
		win.positionCursorWin.y += 300 * _dt;
	}
}