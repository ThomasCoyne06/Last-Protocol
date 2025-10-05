#include "PauseMenu.h"
#include "Utility/Camera.h"
#include "Resources/Music.h"
#include "Resources/Sounds.h"

PauseMenuStruct menuPauseStruct;

void CollisionBetweenMouseIcon(float _dt);
void SetPositionAllSprite(sfRenderWindow* _window);
void LinkToCameraAllSprite(sfRenderWindow* _window);
void QuitMenuPause(void);

void MouseGestion(sfRenderWindow* _window);
void ControllerGestion(float _dt, sfRenderWindow* _window);

void LoadPauseMenu(void)
{

	char* fileCharName[OBJECT_PAUSE_NMB] = { "MenuTab", "Settings1", "Resume", "Quit1", "Cross1" };

	char filename[50];

	//pop up window base files creation
	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB; i++)
	{
		sprintf_s(filename, sizeof(filename), "Assets/Textures/Menu/PauseMenu/%s.png", fileCharName[i]);
		menuPauseStruct.smellsLikeTeenSprite[i] = sfSprite_create();
		menuPauseStruct.texture[i] = sfTexture_createFromFile(filename, NULL);
		sfSprite_setTexture(menuPauseStruct.smellsLikeTeenSprite[i], menuPauseStruct.texture[i], sfFalse);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(menuPauseStruct.smellsLikeTeenSprite[i]);
		sfSprite_setOrigin(menuPauseStruct.smellsLikeTeenSprite[i], (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		sfSprite_setScale(menuPauseStruct.smellsLikeTeenSprite[i], (sfVector2f) { 3.0f, 3.0f });

	}
	//sprintf_s(filename, sizeof(filename), "Assets/Textures/Menu/PauseMenu/%s.png", fileCharName[0]);
	//menuPauseStruct.texture[0] = sfTexture_createFromFile(filename, NULL);

	menuPauseStruct.smellsLikeTeenSprite[0] = CreateSpriteFromSpriteSheet(menuPauseStruct.texture[0], (sfIntRect) { 0, 0, 261, 139 }, sfTrue, SCALE);

	//hitbox square creation and position (static position)

	sfFloatRect hitbox[OBJECT_PAUSE_NMB - 1] = { {0, 0, 110, 30}, {0, 0, 110, 30}, {0, 0, 70, 30}, {0, 0, 25, 20} };
	for (unsigned int i = 0; i < 4; i++)
	{
		menuPauseStruct.hitbox[i] = CreateRectangle(hitbox[i], sfTrue, sfRed, 2.0f, sfYellow);
	}

	//cursor creation
	menuPauseStruct.cursor = sfSprite_create();
	sfTexture* texture = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Home/Cursor.png", NULL);
	sfSprite_setTexture(menuPauseStruct.cursor, texture, sfFalse);
	sfSprite_setScale(menuPauseStruct.cursor, (sfVector2f) { 3.0f, 3.0f });


	char* fileCharNameSett[SETTINGS_OBJECT_NMB] = { "SettingsPopUp", "Button1" };

	char filenameSett[70];
	//settings pop up
	for (unsigned int i = 0; i < SETTINGS_OBJECT_NMB; i++)
	{
		sprintf_s(filenameSett, sizeof(filenameSett), "Assets/Textures/Menu/SettingsTab/%s.png", fileCharNameSett[i]);
		menuPauseStruct.settingsSprite[i] = sfSprite_create();
		menuPauseStruct.settingsTexture[i] = sfTexture_createFromFile(filenameSett, NULL);
		sfSprite_setTexture(menuPauseStruct.settingsSprite[i], menuPauseStruct.settingsTexture[i], sfFalse);
		sfSprite_scale(menuPauseStruct.settingsSprite[i], (sfVector2f) { 3.0f, 3.0f });
	}

	sfFloatRect hitboxSettings[ALL_NAME_NMB] = { {0,0,15,15}, {0,0,30,20}, {0,0,30,20}, {0,0,30,20}, {0,0,30,20} };
	for (unsigned int i = 0; i < 5; i++)
	{
		menuPauseStruct.hitboxSettings[i] = CreateRectangle(hitboxSettings[i], sfTrue, sfRed, 2.0f, sfYellow);
	}

	//level sound text init

	sfFont* font = sfFont_createFromFile("Assets/Fonts/Pixeldu.ttf");

	for (unsigned int i = 0; i < 2; i++)
	{
		menuPauseStruct.textLevelSound[i] = sfText_create();

		sfText_setFont(menuPauseStruct.textLevelSound[i], font);


		sfText_setCharacterSize(menuPauseStruct.textLevelSound[i], SETTINGS_TEXT_SIZE);

		sfText_setColor(menuPauseStruct.textLevelSound[i], sfBlack);

		sfText_setString(menuPauseStruct.textLevelSound[i], "text");

		sfFloatRect rect = sfText_getGlobalBounds(menuPauseStruct.textLevelSound[i]);
		sfText_setOrigin(menuPauseStruct.textLevelSound[i], (sfVector2f) { rect.width / 2, rect.height / 2 });

	}

	//sfText_setString(menuPauseStruct.textLevelSound[0], (char)GetMusicVolume());
	//sfText_setString(menuPauseStruct.textLevelSound[1], (char)GetMusicVolume());


	sfIntRect firstFrame = { 0, 0, 261, 139 };
	menuPauseStruct.backAnim = CreateAnimation(menuPauseStruct.smellsLikeTeenSprite[0], 14, 10, firstFrame, sfTrue);
	menuPauseStruct.buttonClicked = -1;

	menuPauseStruct.positionCursor = (sfVector2f){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
}

void UpdatePauseMenu(float _dt, sfRenderWindow* _window)
{
	//controller and mouse gestion of cursor

	sfRenderWindow_setMouseCursorVisible(_window, sfFalse);




	//set text value
	char text[10];
	sprintf_s(text, sizeof(text), "%.0f", GetMusicVolume());
	sfText_setString(menuPauseStruct.textLevelSound[1], text);
	sprintf_s(text, sizeof(text), "%.0f", GetSoundsVolume());
	sfText_setString(menuPauseStruct.textLevelSound[0], text);

	sfJoystick_update();
	if (sfJoystick_isConnected(0))
	{
		//joystick
		ControllerGestion(_dt, _window);

	}
	//mouse
	

	sfSprite_setPosition(menuPauseStruct.cursor, menuPauseStruct.positionCursor);
	//cursor link to camera
	if (GetGameState() == MENU)
	{
		SetPositionAllSprite(_window);
	}
	else
	{
		LinkToCameraAllSprite(_window);
	}


	//find collision between mouse and button
	//get and convert mouse location
	CollisionBetweenMouseIcon(_dt);

	UpdateAnimation(&menuPauseStruct.backAnim, _dt);

}

void DrawPauseMenu(sfRenderWindow* _window)
{
	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB; i++)
	{
		sfRenderWindow_drawSprite(_window, menuPauseStruct.smellsLikeTeenSprite[i], NULL);
	}
}

void DrawSettingsMenu(sfRenderWindow* _window)
{
	//draw settings windows
	if (menuPauseStruct.settingsOpen)
	{
		for (unsigned int i = 0; i < SETTINGS_OBJECT_NMB; i++)
		{
			sfRenderWindow_drawSprite(_window, menuPauseStruct.settingsSprite[i], NULL);
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			sfRenderWindow_drawText(_window, menuPauseStruct.textLevelSound[i], NULL);
		}
	}

	sfRenderWindow_drawSprite(_window, menuPauseStruct.cursor, NULL);

}

void CleanupPauseMenu(void)
{
}

PauseMenuObjectEnum GetButtonClicked(void)
{
	return menuPauseStruct.buttonClicked;
}

void ResetClickedButton(void)
{
	menuPauseStruct.buttonClicked = -1;
}

void SetSettingsVisibility(sfBool _isvisi)
{
	menuPauseStruct.settingsOpen = _isvisi;
}

sfVector2f GetCursorPositionOnPause(void)
{
	return sfSprite_getPosition(menuPauseStruct.cursor);
}

void CollisionBetweenMouseIcon(float _dt)
{
	sfVector2f mouse2f = sfSprite_getPosition(menuPauseStruct.cursor);

	if (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click))
	{
		//menu pause
		for (unsigned int i = 0; i < OBJECT_PAUSE_NMB - 1; i++)
		{
			sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(menuPauseStruct.hitbox[i]);

			if (sfFloatRect_contains(&hitbox, mouse2f.x, mouse2f.y))
			{

				//adapt the index for the switch case
				int value = i + 1;

				//if colliding, then switch on type of button
				switch (value)
				{
				case SETTINGS_BUTTON_PAUSE:
					menuPauseStruct.buttonClicked = SETTINGS_BUTTON_PAUSE;
					menuPauseStruct.settingsOpen = sfTrue;
					break;
				case CONTINUE_BUTTON_PAUSE:
					menuPauseStruct.buttonClicked = CONTINUE_BUTTON_PAUSE;
					break;
				case QUIT_PAUSE:
					QuitMenuPause();
					break;
				case CROSS_PAUSE:
					menuPauseStruct.buttonClicked = CROSS_PAUSE;
					break;
				default:
					menuPauseStruct.buttonClicked = -1;
					break;
				}
			}
		}

		//settings interaction
		for (unsigned int i = 0; i < ALL_NAME_NMB; i++)
		{
			sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(menuPauseStruct.hitboxSettings[i]);

			if (sfFloatRect_contains(&hitbox, mouse2f.x, mouse2f.y))
			{
				switch (i)
				{
				case CROSS_PAUSE_SETTINGS:
					menuPauseStruct.settingsOpen = sfFalse;
					break;
				case INCREASE_FX_VOLUME:
					AddToSoundsVolume(SLIDER_SPEED);
					UpdateSound(_dt);
					break;
				case INCREASE_MUSIC_VOLUME:
					AddToMusicVolume(SLIDER_SPEED);
					//UpdateMusic(_dt);
					break;
				case DECREASE_FX_VOLUME:
					AddToSoundsVolume(-SLIDER_SPEED);
					UpdateSound(_dt);
					break;
				case DECREASE_MUSIC_VOLUME:
					AddToMusicVolume(-SLIDER_SPEED);
					UpdateMusic(_dt);
					break;
				default:
					break;
				}
			}
		}
	}
}

void QuitMenuPause(void)
{
	menuPauseStruct.buttonClicked = QUIT_PAUSE;
}



void SetPositionAllSprite(sfRenderWindow* _window)
{
	sfVector2f hitbox[OBJECT_PAUSE_NMB - 1] = { SETTINGS_BUTTON_PAUSE_POSITION, CONTINUE_BUTTON_PAUSE_POSITION,
												QUIT_PAUSE_POSITION, CROSS_PAUSE_POSITION };
	sfVector2f positionSettings[ALL_NAME_NMB] = { CROSS_PAUSE_SETTINGS_POSITION, DECREASE_FX_VOLUME_POSITION,
	INCREASE_FX_VOLUME_POSITION, DECREASE_MUSIC_VOLUME_POSITION, INCREASE_MUSIC_VOLUME_POSITION };



	for (unsigned int i = 0; i < 2; i++)
	{
		sfText_setPosition(menuPauseStruct.textLevelSound[i], (sfVector2f) { 425 + 245 * i, 720 });
	}

	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB; i++)
	{
		sfSprite_setPosition(menuPauseStruct.smellsLikeTeenSprite[i], (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	}

	//link to camera all sprite
	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB - 1; i++)
	{
		sfRectangleShape_setPosition(menuPauseStruct.hitbox[i], hitbox[i]);
	}
	//link settings to the camera
	for (unsigned int i = 0; i < SETTINGS_OBJECT_NMB; i++)
	{
		sfSprite_setPosition(menuPauseStruct.settingsSprite[i], SETTINGS_POPUP_POSITION);
	}


	for (unsigned int i = 0; i < ALL_NAME_NMB; i++)
	{
		sfRectangleShape_setPosition(menuPauseStruct.hitboxSettings[i], positionSettings[i]);
	}
}

void LinkToCameraAllSprite(sfRenderWindow* _window)
{

	sfVector2f hitbox[OBJECT_PAUSE_NMB - 1] = { SETTINGS_BUTTON_PAUSE_POSITION, CONTINUE_BUTTON_PAUSE_POSITION,
												QUIT_PAUSE_POSITION, CROSS_PAUSE_POSITION };
	sfVector2f positionSettings[ALL_NAME_NMB] = { CROSS_PAUSE_SETTINGS_POSITION, DECREASE_FX_VOLUME_POSITION,
	INCREASE_FX_VOLUME_POSITION, DECREASE_MUSIC_VOLUME_POSITION, INCREASE_MUSIC_VOLUME_POSITION };
	LinkSpriteToCamera(menuPauseStruct.cursor, _window, menuPauseStruct.positionCursor);


	for (unsigned int i = 0; i < 2; i++)
	{
		LinkSpriteToCamera(menuPauseStruct.textLevelSound[i], _window, (sfVector2f) { 425 + 245 * i, 720 });
	}

	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB; i++)
	{
		LinkSpriteToCamera(menuPauseStruct.smellsLikeTeenSprite[i], _window, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	}

	//link to camera all sprite
	for (unsigned int i = 0; i < OBJECT_PAUSE_NMB - 1; i++)
	{
		LinkSpriteToCamera(menuPauseStruct.hitbox[i], _window, hitbox[i]);
	}
	//link settings to the camera
	for (unsigned int i = 0; i < SETTINGS_OBJECT_NMB; i++)
	{
		LinkSpriteToCamera(menuPauseStruct.settingsSprite[i], _window, SETTINGS_POPUP_POSITION);
	}


	for (unsigned int i = 0; i < ALL_NAME_NMB; i++)
	{
		LinkSpriteToCamera(menuPauseStruct.hitboxSettings[i], _window, positionSettings[i]);
	}
}

void MouseGestion(sfRenderWindow* _window)
{
	menuPauseStruct.positionCursor.x = (float)sfMouse_getPositionRenderWindow(_window).x;
	menuPauseStruct.positionCursor.y = (float)sfMouse_getPositionRenderWindow(_window).y;
}

void ControllerGestion(float _dt, sfRenderWindow* _window)
{
	if (CheckMapping(GetMappingMenu()->left) && menuPauseStruct.positionCursor.x > BORDER_POSITION_LEFT)
	{
		menuPauseStruct.positionCursor.x -= SENSI_HORIZONTAL_CONTROLLER * _dt;
	}
	if (CheckMapping(GetMappingMenu()->right) && menuPauseStruct.positionCursor.x < BORDER_POSITION_RIGHT)
	{
		menuPauseStruct.positionCursor.x += SENSI_HORIZONTAL_CONTROLLER * _dt;
	}
	if (CheckMapping(GetMappingMenu()->up) && menuPauseStruct.positionCursor.y > BORDER_POSITION_TOP)
	{
		menuPauseStruct.positionCursor.y -= SENSI_VERTICAL_CONTROLLER * _dt;
	}
	if (CheckMapping(GetMappingMenu()->down) && menuPauseStruct.positionCursor.y < BORDER_POSITION_BOT)
	{
		menuPauseStruct.positionCursor.y += SENSI_VERTICAL_CONTROLLER * _dt;
	}
}

MouseMovedCursor(sfMouseMoveEvent _mouseMove)
{
	menuPauseStruct.positionCursor = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y } ;
}
