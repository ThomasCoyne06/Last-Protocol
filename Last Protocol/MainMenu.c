#include "MainMenu.h"
#include "Resources/Music.h"
#include "Resources/Sounds.h"
#include "Utility/Mapping.h"
#include "PauseMenu.h"
#include <time.h>

MainMenuStruct mainMenuStruct;

//return index of sprite that get collide with mouse
int CollisionWithSpriteMainMenu(sfVector2f _mousePosition);

void CollisionAndActionWithButton(int _index, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window);

//when player get the play button over the bin  icon (quit button)
void DeletePlayButton(sfMouseMoveEvent _mouseMove);

sfBool IconCollideWithAnother(sfFloatRect _positionSprite, int _index);

void CleanUpPopup(void);

void DoubleClick(float _dt);


//COW
void LoadCow(void);
void UpdateCow(float _dt);

void LoadMainMenu(void)
{
	fileCollected = 0;
	fileSave = 0;
	crono = 0;
	lifeLost = 0;

	mainMenuStruct.popUpBuffer = sfSoundBuffer_createFromFile("Assets/Sounds/Menu/PopUp.wav");
	mainMenuStruct.popUpSound = sfSound_create();
	sfSound_setBuffer(mainMenuStruct.popUpSound, mainMenuStruct.popUpBuffer);

	mainMenuStruct.popUpMusic = sfMusic_createFromFile("Assets/Musics/MainMenuPart2.ogg");
	sfMusic_setLoop(mainMenuStruct.popUpMusic, sfTrue);


	char* spriteName[NMB_SPRITE] = { "Wallpaper", "PlayButton", "OptionButton",
		"CreditsButton", "QuitButton", "Background", "ExitButton", "OkButton", "Cow" };

	char fileName[80];
	for (unsigned int i = 0; i < NMB_SPRITE; i++)
	{
		//generate sprite for all
		mainMenuStruct.sprite[i] = sfSprite_create();
		if (i <= QUIT_BUTTON)
		{
			//texture
			sprintf_s(fileName, sizeof(fileName), "Assets/Textures/Menu/MainMenu/Home/%s.png", spriteName[i]);
			mainMenuStruct.texture[i] = sfTexture_createFromFile(fileName, NULL);
			sfSprite_setTexture(mainMenuStruct.sprite[i], mainMenuStruct.texture[i], NULL);

			//origin
			sfFloatRect hitbox = sfSprite_getGlobalBounds(mainMenuStruct.sprite[i]);
			sfSprite_setOrigin(mainMenuStruct.sprite[i], (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
			sfSprite_setScale(mainMenuStruct.sprite[i], (sfVector2f) { 3.0f, 3.0f });
		}
		else
		{
			mainMenuStruct.sprite[i] = sfSprite_create();

			sprintf_s(fileName, sizeof(fileName), "Assets/Textures/Menu/MainMenu/Popup/%s.png", spriteName[i]);
			mainMenuStruct.texture[i] = sfTexture_createFromFile(fileName, NULL);
			sfSprite_setTexture(mainMenuStruct.sprite[i], mainMenuStruct.texture[i], NULL);
			sfSprite_setScale(mainMenuStruct.sprite[i], (sfVector2f) { 3.0f, 3.0f });
		}
		//position
		if (i != WALLPAPER)
		{
			//all sprite
			sfSprite_setPosition(mainMenuStruct.sprite[i], (sfVector2f) { ICON_X_POSITION, ICON_Y_POSITION* i });
		}
		else
		{
			//Wallpaper
			sfSprite_setPosition(mainMenuStruct.sprite[i], (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
		}

	}
	
	LoadPauseMenu();

	//animation
	sfIntRect hitbox = { 0, 0, 75, 49 };
	mainMenuStruct.cowAnim = CreateAnimation(mainMenuStruct.sprite[8], 13, 10, hitbox, sfTrue);
	sfSprite_setColor(mainMenuStruct.sprite[8], sfColor_fromRGBA(0,0,0,0));

	hitbox = (sfIntRect){ 0, 0, 640, 360 };
	mainMenuStruct.wallpaperAnimTexture = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Home/MenuGlitch.png", NULL);
	mainMenuStruct.backgroundAnim = CreateAnimation(mainMenuStruct.sprite[WALLPAPER], 18, 12, hitbox, sfTrue);

	//hitbox pop up virus
	for (unsigned int i = 0; i < 3; i++)
	{
		if (i < 2)
		{
			sfFloatRect zone = { 1300 - i * 35, 700 + i * 210, 35, 25 };
			mainMenuStruct.popUpHitbox[i] = CreateRectangle(zone, sfTrue, sfRed, 3.0f, sfYellow);
		}
		else
		{

			sfFloatRect zone = { 1500, 750, 250, 250 };
			mainMenuStruct.popUpHitbox[i] = CreateRectangle(zone, sfTrue, sfRed, 3.0f, sfYellow);
		}
	}



	//set base value of all member of mainMenuStruct
	mainMenuStruct.popUpTexture = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Popup/Popup.png", NULL);
	mainMenuStruct.isBouncing = sfTrue;
	mainMenuStruct.playPopup = sfFalse;

	mainMenuStruct.location.x = 100;
	mainMenuStruct.location.y = 100;

	mainMenuStruct.timerPopUp = BASE_DELAY_POPUP;
	mainMenuStruct.delaySpawnPopup = BASE_DELAY_POPUP;

	mainMenuStruct.quitHit = sfFalse;
	mainMenuStruct.grabIndex = -1;
	mainMenuStruct.isGrabing = sfFalse;
	mainMenuStruct.enterGame = sfFalse;

	mainMenuStruct.timerToDoubleClick = DOUBLE_CLICK_TIMER;
	mainMenuStruct.hasClickOnce = sfFalse;
	mainMenuStruct.doubleClick = sfFalse;
	mainMenuStruct.hasRelease = sfFalse;
	mainMenuStruct.delayDouble = DELAY_TO_DOUBLE_CLICK;
	mainMenuStruct.positionCursor = (sfVector2f){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	LoadCow();

	PlayMusic(MAIN_MENU1, sfTrue, sfFalse);

	fileCollected = 0;
	fileSave = 0;
	crono = 0;
	lifeLost = 0;
}

void UpdateMainMenu(float _dt, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window)
{
	sfRenderWindow_setMouseCursorVisible(_window, sfFalse);

	SetDefaultCamera(_window);

	sfJoystick_update();

	UpdatePauseMenu(_dt, _window);

	UpdateCow(_dt);

	mainMenuStruct.positionCursor = GetCursorPositionOnPause();
	SetCursorPosition(mainMenuStruct.positionCursor);

	DoubleClick(_dt);

	int index = CollisionWithSpriteMainMenu(mainMenuStruct.positionCursor);

	//DeletePlayButton(_mouseMove);

	//interaction with button and sprite
	CollisionAndActionWithButton(index, _mouseMove, _window);

	//cow animation
	if (mainMenuStruct.clickPlayExe == sfTrue)
	{
		UpdateAnimation(&mainMenuStruct.cowAnim, _dt);
	}


	//wallpaper animation or not : if playpopup is true.
	if (mainMenuStruct.playPopup)
	{
		//animated wallpaper
		sfSprite_setTexture(mainMenuStruct.sprite[WALLPAPER], mainMenuStruct.wallpaperAnimTexture, sfTrue);
		UpdateAnimation(&mainMenuStruct.backgroundAnim, _dt);
	}
	else
	{
		//not animated
		sfSprite_setTexture(mainMenuStruct.sprite[WALLPAPER], mainMenuStruct.texture[WALLPAPER], sfTrue);
	}
	if (mainMenuStruct.positionCursor.x >= BORDER_POSITION_LEFT &&
		mainMenuStruct.positionCursor.x <= BORDER_POSITION_RIGHT &&
		mainMenuStruct.positionCursor.y >= BORDER_POSITION_TOP &&
		mainMenuStruct.positionCursor.y <= BORDER_POSITION_BOT)
	{
		//grab function
		if ((sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click)) && index != -1)
		{
			if (mainMenuStruct.grabIndex == -1)
			{
				GrabFileMainMenu(index, _mouseMove);
			}
			else
			{

				GrabFileMainMenu(mainMenuStruct.grabIndex, _mouseMove);

				//save position
				//
				sfVector2f positionSave = sfSprite_getPosition(mainMenuStruct.sprite[mainMenuStruct.grabIndex]);
				sfFloatRect hitbox = sfSprite_getGlobalBounds(mainMenuStruct.sprite[mainMenuStruct.grabIndex]);


				sfSprite_setPosition(mainMenuStruct.sprite[mainMenuStruct.grabIndex], mainMenuStruct.positionCursor);

				//if collide with another icon, do not move
				//
				if (IconCollideWithAnother(hitbox, mainMenuStruct.grabIndex))
				{
					sfSprite_setPosition(mainMenuStruct.sprite[mainMenuStruct.grabIndex], positionSave);
				}
			}
		}

		else if (!(sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click)) && mainMenuStruct.grabIndex != -1)
		{
			DropFileMainMenu(index, _mouseMove);
		}
	}
	else
	{
		DropFileMainMenu(index, _mouseMove);
	}

	//drop


	//move sprite
	if (mainMenuStruct.grabIndex != -1)
	{
		sfSprite_setPosition(mainMenuStruct.sprite[mainMenuStruct.grabIndex], mainMenuStruct.positionCursor);

	}
	for (unsigned int i = 0; i < NMB_SPRITE; i++)
	{
		if (i > QUIT_BUTTON)
		{
			sfSprite_setPosition(mainMenuStruct.sprite[i], QUIT_BUTTON_POSITION);
		}
	}

	sfSprite_setPosition(mainMenuStruct.sprite[8], (sfVector2f) { 450, 750 });

	SpawnPopUpWindow(_dt);
}

void DrawMainMenu(sfRenderWindow* _window)
{
	for (unsigned int i = 0; i < NMB_SPRITE; i++)
	{
		if (i <= QUIT_BUTTON)
		{
			sfRenderWindow_drawSprite(_window, mainMenuStruct.sprite[i], NULL);
		}

		else
		{
			if (i < BACKGROUND)
			{
				for (unsigned int i = 0; i < NUMBER_POPUP; i++)
				{
					if (mainMenuStruct.popUp[i] != NULL)
					{
						sfRenderWindow_drawSprite(_window, mainMenuStruct.popUp[i], NULL);
					}
				}
			}
			if (mainMenuStruct.clickPlayExe == sfTrue)
			{
				if (i != 2)
				{
					sfRenderWindow_drawSprite(_window, mainMenuStruct.sprite[i], NULL);

				}
				else
				{

					sfRenderWindow_drawSprite(_window, mainMenuStruct.sprite[i], NULL);

				}
			}
		}
	}

	sfRenderWindow_drawSprite(_window, mainMenuStruct.spriteCow, NULL);
}

void CleanupMainMenu(void)
{
	for (unsigned int i = 0; i < NMB_SPRITE; i++)
	{
		sfSprite_destroy(mainMenuStruct.sprite[i]);
		mainMenuStruct.sprite[i] = NULL;

		sfTexture_destroy(mainMenuStruct.texture[i]);
		mainMenuStruct.texture[i] = NULL;
	}

	for (unsigned int i = 0; i < 2; i++)
	{
		sfRectangleShape_destroy(mainMenuStruct.popUpHitbox[i]);
		mainMenuStruct.popUpHitbox[i] = NULL;
	}

	for (unsigned int i = 0; i < NUMBER_POPUP; i++)
	{
		if (mainMenuStruct.popUp[i] != NULL)
		{
			sfSprite_destroy(mainMenuStruct.popUp[i]);
			mainMenuStruct.popUp[i] = NULL;
		}
	}
}

void GrabFileMainMenu(int _index, sfMouseMoveEvent _mouseMove)
{
	if (mainMenuStruct.grabIndex != _index)
	{
		mainMenuStruct.grabIndex = _index;
	}
}

void DropFileMainMenu(int _index, sfMouseMoveEvent _mouseMove)
{
	if (mainMenuStruct.grabIndex == _index)
	{
		mainMenuStruct.grabIndex = -1;
	}
	if (_index != -1)
	{

		if (sfSprite_getPosition(mainMenuStruct.sprite[_index]).x < BORDER_POSITION_LEFT)
		{
			sfSprite_setPosition(mainMenuStruct.sprite[_index], (sfVector2f) { BORDER_POSITION_LEFT, sfSprite_getPosition(mainMenuStruct.sprite[_index]).y });
		}
		if (sfSprite_getPosition(mainMenuStruct.sprite[_index]).x > BORDER_POSITION_RIGHT)
		{
			sfSprite_setPosition(mainMenuStruct.sprite[_index], (sfVector2f) { BORDER_POSITION_RIGHT, sfSprite_getPosition(mainMenuStruct.sprite[_index]).y });
		}
		if (sfSprite_getPosition(mainMenuStruct.sprite[_index]).y < BORDER_POSITION_TOP)
		{
			sfSprite_setPosition(mainMenuStruct.sprite[_index], (sfVector2f) { sfSprite_getPosition(mainMenuStruct.sprite[_index]).x, BORDER_POSITION_TOP });
		}
		if (sfSprite_getPosition(mainMenuStruct.sprite[_index]).y > BORDER_POSITION_BOT)
		{
			sfSprite_setPosition(mainMenuStruct.sprite[_index], (sfVector2f) { sfSprite_getPosition(mainMenuStruct.sprite[_index]).x, BORDER_POSITION_BOT });
		}
	}


}


//create popup each 2.5s of the virus
//simple random location or bouncing
void SpawnPopUpWindow(float _dt)
{
	
	if (mainMenuStruct.playPopup == sfTrue)
	{
		mainMenuStruct.timerPopUp -= _dt;
		if (mainMenuStruct.timerPopUp <= 0.0f)
		{

			mainMenuStruct.timerPopUp = mainMenuStruct.delaySpawnPopup;

			if (mainMenuStruct.delaySpawnPopup >= LIMIT_DELAY_POPUP)
			{
				mainMenuStruct.delaySpawnPopup *= COEF_APPARITION;
			}

			if (mainMenuStruct.isBouncing == sfFalse)
			{
				RandomPopupLocation();
			}
			//you should bounce bounce bounce, maaan
			else
			{
				SetUpPopupInFallingStyle();
			}
		}
	}
}

void CreatePopupSprite(int _index)
{
	sfSound_play(mainMenuStruct.popUpSound);

	mainMenuStruct.popUp[_index] = sfSprite_create();
	sfSprite_setTexture(mainMenuStruct.popUp[_index], mainMenuStruct.popUpTexture, sfTrue);
	sfSprite_setScale(mainMenuStruct.popUp[_index], (sfVector2f) { SCALE_POPUP, SCALE_POPUP });
}

void RandomPopupLocation(void)
{
	//for each popup, create it and set random location
	for (unsigned int i = 0; i < NUMBER_POPUP; i++)
	{
		if (mainMenuStruct.popUp[i] == NULL)
		{
			CreatePopupSprite(i);
			sfSprite_setPosition(mainMenuStruct.popUp[i], (sfVector2f) { (rand() % (SCREEN_WIDTH - 585)) + 100, (rand() % (SCREEN_HEIGHT - 385)) + 100 });
			return;
		}
	}
	//generate new position for all 
	for (unsigned int i = 0; i < NUMBER_RANDOM_LOC_POPUP; i++)
	{
		sfSprite_setPosition(mainMenuStruct.popUp[rand() % (NUMBER_POPUP - 1)], (sfVector2f) { (rand() % (SCREEN_WIDTH - 585)) + 100, (rand() % (SCREEN_HEIGHT - 385)) + 100 });
	}
}

void SetUpPopupInFallingStyle(void)
{

	for (unsigned int i = 0; i < NUMBER_POPUP; i++)
	{
		if (mainMenuStruct.popUp[i] == NULL)
		{
			//generate a random location for the next pop up
			mainMenuStruct.location.x += BASE_RANDOM_POSITION + rand() % RANDOM_POSITION_MODULO;
			mainMenuStruct.location.y += BASE_RANDOM_POSITION + rand() % RANDOM_POSITION_MODULO;
			CreatePopupSprite(i);
			sfSprite_setPosition(mainMenuStruct.popUp[i], mainMenuStruct.location);

			//don't let pop up go out of the window AND set new location to the middle of the random possiblities

			//verification on the X axis
			if (mainMenuStruct.location.x > (SCREEN_WIDTH - 585))
			{
				mainMenuStruct.location.x = BASE_RESPAWN_POPUP_POSITION_X;
				mainMenuStruct.location.y = i * 10 + LOCATIONY_POPUP_TO_ADD;
			}
			//verification on the Y axis
			if (mainMenuStruct.location.y > (SCREEN_HEIGHT - 485))
			{
				mainMenuStruct.location.y = BASE_RESPAWN_POPUP_POSITION_Y;
				mainMenuStruct.location.x = i * 10 + LOCATIONX_POPUP_TO_ADD;
			}

			return;
		}
	}
	mainMenuStruct.isBouncing = sfFalse;
	mainMenuStruct.clickPlayExe = sfTrue;
}



int CollisionWithSpriteMainMenu(sfVector2f _mousePosition)
{
	for (unsigned int i = 0; i < 2; i++)
	{
		if (IsCollidingPointRectangle(GetCursorPosition(), sfRectangleShape_getGlobalBounds(mainMenuStruct.popUpHitbox[i])))
		{
			if (i == 0)
			{
				return CROSS;
			}
			else
			{
				return OK_BUTTON;
			}
		}
	}
	for (unsigned int i = 1; i < NMB_SPRITE; i++)
	{
		//if collision find return i, else return -1
		if (IsCollidingPointRectangle(GetCursorPosition(), sfSprite_getGlobalBounds(mainMenuStruct.sprite[i]))
			&& i < BACKGROUND)
		{
			return i;
		}
	}

	return -1;
}

void CollisionAndActionWithButton(int _index, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window)
{
	//if player hit left mouse button, go find if he touch a button or not (HERE IT S DOUBLE CLICK)
	if (mainMenuStruct.doubleClick)
	{
		switch (_index)
		{
			//mainly icon of windows
		case COW:
			//mainMenuStruct.showCow = sfTrue;
			break;
		case PLAY_BUTTON:
			mainMenuStruct.playPopup = sfTrue;
			StopMusic(GetMusicPlayed());
			
			sfMusic_play(mainMenuStruct.popUpMusic);

			mainMenuStruct.runCow = sfTrue;

			break;
		case QUIT_BUTTON:
			mainMenuStruct.quitHit = sfTrue;
			StartTransitionIris(GetCursorPosition(), sfBlack, QUIT_FADE_SPEED);
			break;
		case OPTION_BUTTON:
			SetSettingsVisibility(sfTrue);
			break;
		case CREDITS_BUTTON:
			GenerateCreditsMenu();
			break;
		default:
			//duality - slipknot
			break;
		}
	}
	else
	{
		//for button that didn't need double click
		if (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click))
		{
			switch (_index)
			{
			case CROSS:
				mainMenuStruct.clickPlayExe = sfFalse;
				mainMenuStruct.playPopup = sfFalse;
				mainMenuStruct.timerPopUp = BASE_DELAY_POPUP;
				mainMenuStruct.delaySpawnPopup = BASE_DELAY_POPUP;
				mainMenuStruct.isBouncing = sfTrue;
				CleanUpPopup();

				StopMusic(GetMusicPlayed());
				PlayMusic(MAIN_MENU1, sfTrue, sfFalse);
				break;
			case OK_BUTTON:
				if (mainMenuStruct.clickPlayExe == sfTrue)
				{
					mainMenuStruct.enterGame = sfTrue;
					//StartTransitionIris(GetCursorPosition(), sfBlack, QUIT_FADE_SPEED);
					StartTransitionPicture(sfWhite, 2, 10);

					StopMusic(GetMusicPlayed());
				}
				break;
			default:
				break;
			}
		}
	}

	//when transition are over, active all needed code

	//quit game	
	if (GetTransitionState(COVER_SCREEN) && mainMenuStruct.quitHit == sfTrue)
	{
		sfRenderWindow_close(_window);
		mainMenuStruct.quitHit = sfFalse;
	}

	//enter game
	else if (GetTransitionState(COVER_SCREEN) && mainMenuStruct.enterGame == sfTrue)
	{
		sfMusic_stop(mainMenuStruct.popUpMusic);
		sfMusic_destroy(mainMenuStruct.popUpMusic);

		SetGameState(CUTSCENE);
		mainMenuStruct.enterGame = sfFalse;
		mainMenuStruct.clickPlayExe = sfFalse;
	}
}

void DeletePlayButton(sfMouseMoveEvent _mouseMove)
{
	if (IsColliding2Rectangles(sfSprite_getGlobalBounds(mainMenuStruct.sprite[PLAY_BUTTON]), sfSprite_getGlobalBounds(mainMenuStruct.sprite[QUIT_BUTTON])))
	{

		mainMenuStruct.quitHit = sfTrue;
		StartTransitionIris((sfVector2f) { _mouseMove.x, _mouseMove.y }, sfBlack, QUIT_FADE_SPEED);
	}
}

sfBool IconCollideWithAnother(sfFloatRect _positionSprite, int _index)
{
	for (unsigned int i = 1; i < NMB_SPRITE; i++)
	{
		if (IsColliding2Rectangles(_positionSprite, sfSprite_getGlobalBounds(mainMenuStruct.sprite[i])) && _index != i)
		{
			return sfTrue;
		}
	}

	return sfFalse;
}

void CleanUpPopup(void)
{
	for (unsigned int i = 0; i < NUMBER_POPUP; i++)
	{
		if (mainMenuStruct.popUp[i] != NULL)
		{
			sfSprite_destroy(mainMenuStruct.popUp[i]);
			mainMenuStruct.popUp[i] = NULL;
		}
	}

	sfMusic_stop(mainMenuStruct.popUpMusic);
	sfMusic_destroy(mainMenuStruct.popUpMusic);
}

void DoubleClick(float _dt)
{	//double click
	if (sfMouse_isButtonPressed(sfMouseLeft) || CheckMapping(GetMappingMenu()->click))
	{
		//if player already click once, and release between second click, then he can double click
		if (mainMenuStruct.hasClickOnce == sfTrue && mainMenuStruct.hasRelease == sfTrue)
		{
			if (mainMenuStruct.delayDouble <= 0.0f)
			{
				mainMenuStruct.doubleClick = sfTrue;
			}

			//decrease delay
			else
			{
				mainMenuStruct.delayDouble -= _dt;
			}
		}

		//set that player already click once
		else
		{
			mainMenuStruct.hasClickOnce = sfTrue;
		}
	}
	else
	{
		if (mainMenuStruct.hasClickOnce == sfTrue)
		{
			mainMenuStruct.hasRelease = sfTrue;
			mainMenuStruct.timerToDoubleClick -= _dt;

			//if timer over, then reset all
			if (mainMenuStruct.timerToDoubleClick <= 0.0f)
			{
				mainMenuStruct.timerToDoubleClick = DOUBLE_CLICK_TIMER;
				mainMenuStruct.hasClickOnce = sfFalse;
				mainMenuStruct.doubleClick = sfFalse;
				mainMenuStruct.delayDouble = DELAY_TO_DOUBLE_CLICK;
				mainMenuStruct.hasRelease = sfFalse;
			}
		}
	}
}

void LoadCow(void)
{
	mainMenuStruct.textureCow = sfTexture_createFromFile("Assets/Textures/Menu/MainMenu/Cow.png",NULL);
	mainMenuStruct.spriteCow = sfSprite_create();
	sfSprite_setTexture(mainMenuStruct.spriteCow, mainMenuStruct.textureCow, sfTrue);
	
	sfSprite_setPosition(mainMenuStruct.spriteCow, (sfVector2f) { COW_POS_X, COW_POS_Y});
	sfSprite_setScale(mainMenuStruct.spriteCow, (sfVector2f) { SCALE, SCALE });

	

}

void UpdateCow(float _dt)
{
	if (mainMenuStruct.runCow == sfTrue)
	{
		//The cow run away
		if (mainMenuStruct.timerCow >= COW_TIMER)
		{
			sfSprite_setScale(mainMenuStruct.spriteCow, (sfVector2f) { SCALE, SCALE });
			sfSprite_move(mainMenuStruct.spriteCow, (sfVector2f) { COW_SPEED, 0 });
		}
		else
		{
			// The cow is Fearing
			mainMenuStruct.timerCow += _dt;
			mainMenuStruct.timerCowFear += _dt;

			if (mainMenuStruct.timerCowFear >= COW_CHANGE)
			{
				if (mainMenuStruct.directionCow == sfFalse)
				{
					sfSprite_setScale(mainMenuStruct.spriteCow, (sfVector2f) { -SCALE, SCALE });
					mainMenuStruct.directionCow == sfTrue;
				}
				else
				{
					sfSprite_setScale(mainMenuStruct.spriteCow, (sfVector2f) { SCALE, SCALE });
					mainMenuStruct.directionCow == sfFalse;
				}
				mainMenuStruct.timerCowFear -= COW_CHANGE;
			}
		}
	}
}

