#include "Utility/Common.h"
#include "GameState/Menu.h"
#include "MainMenu.h"
#include "MainMenuWindow.h"
#include "Game/Game.h"
#include "GameState/GameOver.h"
#include "GameState/Win.h"
#include "Cutscene/Cutscene.h"
#include "GameState/GameState.h"
#include "Launching/Launch.h"
#include "Utility/Mapping.h"


typedef struct
{
	sfRenderWindow* window;
	sfEvent event;
	sfClock* gameClock;
	GameState saveGameState;

}MainData;

MainData mainData;

#pragma region functions
void LoadGameState(void);
void Load(void);
void PollEvent(void);
void KeyPressed(void);
void MouseButtonPressed(void);
void MouseMoved(void);
void Update(void);
void Draw(void);
void CleanupGameState(void);
void Cleanup(void);
void Reload(void);
#pragma endregion

int main(void)
{
	Load();
	LoadControl();

	while (sfRenderWindow_isOpen(mainData.window))
	{
		PollEvent();
		Update();
		Draw();
	}
	Cleanup();
	return EXIT_SUCCESS;
}

void LoadGameState(void)
{
	switch (GetGameState())
	{
	case CUTSCENE:
		LoadShotMovie();
		break;
	case LAUNCHING_GAME:
		LoadLaunch();
		break;
	case MENU:
		LoadMainMenu();
		break;
	case GAME:
		LoadGame(LEVEL_1);
		break;
	case GAME_OVER:
		LoadGameOver(mainData.window);
		break;
	case WIN:
		LoadWin(mainData.window);
		break;
	}
}

void Load(void)
{
	sfVideoMode mode = { SCREEN_WIDTH, SCREEN_HEIGHT, 32 };
	mainData.window = sfRenderWindow_create(mode, "Last Protocol", sfFullscreen, NULL);
	sfRenderWindow_setFramerateLimit(mainData.window, 60);

	sfImage* icon = sfImage_createFromFile("Assets/Icon.png");
	sfRenderWindow_setIcon(mainData.window, sfImage_getSize(icon).x, sfImage_getSize(icon).y, sfImage_getPixelsPtr(icon));

	sfRenderWindow_setMouseCursorVisible(mainData.window, sfFalse);

	// Set the window's position to the right of the screen
	sfVideoMode desktopMode = sfVideoMode_getDesktopMode();
	//sfVector2i windowPos = { desktopMode.width / 2 - SCREEN_WIDTH / 2, desktopMode.height / 2 - SCREEN_HEIGHT / 2 };
	sfVector2i windowPos = { desktopMode.width - SCREEN_WIDTH, 0 };
	sfRenderWindow_setPosition(mainData.window, windowPos);

	sfRenderWindow_setVerticalSyncEnabled(mainData.window, sfTrue);

	srand((unsigned)time(NULL));

	mainData.gameClock = sfClock_create();

	SetGameState(LAUNCHING_GAME);
	mainData.saveGameState = GetGameState();

	LoadMusic();
	LoadGameState();
	LoadAllSound();
	//LoadMainMenu();
	LoadWindow();
	//PlayMusic(MAIN_MENU1, sfTrue, sfFalse);
}

void PollEvent(void)
{
	while (sfRenderWindow_pollEvent(mainData.window, &mainData.event))
	{
		switch (mainData.event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(mainData.window);
			break;
		case sfEvtKeyPressed:
			KeyPressed();
			break;
		case sfEvtMouseButtonPressed:
			MouseButtonPressed();
			break;
		case sfEvtMouseMoved:
			MouseMoved();
			break;
		}
		//Reload();
	}
}

void KeyPressed(void)
{
	sfKeyEvent key = mainData.event.key;
	switch (GetGameState())
	{
	case CUTSCENE:
		break;

	case MENU:
		KeypressedMenu(mainData.window, key);
		break;
	case GAME:
		KeypressedGame(mainData.window, key);
		break;
	case GAME_OVER:
		KeypressedGameOver(mainData.window, key);
		break;
	case WIN:
		KeypressedWin(mainData.window, key);
		break;
	}
	Reload();
}

void MouseButtonPressed(void)
{
	//sfMouseButtonEvent mouseButton = mainData.event.mouseButton;

	switch (GetGameState())
	{
	case GAME:
		//MouseButtonPressedGame(&mainData.game, mouseButton);
		break;
	}

	Reload();
}

void MouseMoved(void)
{
	sfMouseMoveEvent mouseMove = mainData.event.mouseMove;

	switch (GetGameState())
	{
	
	case MENU:
		MouseMovedCursor(mouseMove);
		break;
	case GAME:
		MouseMovedCursor(mouseMove);
		break;
	case GAME_OVER:
		MouseMovedGameOver(mouseMove);
		break;
	case WIN:
		MouseMovedWin(mouseMove);
		break;
	}
}

void Update(void)
{
	float dt = sfTime_asSeconds(sfClock_restart(mainData.gameClock));
	sfMouseMoveEvent mouseMove = mainData.event.mouseMove;

	switch (GetGameState())
	{
	case CUTSCENE:
		UpdateShotMovie(dt);
		break;
	case LAUNCHING_GAME:
		UpdateSound(dt);
		UpdateLaunch(dt);
		break;
	case MENU:
		UpdatePauseMenu(dt, mainData.window);
		UpdateMainMenu(dt, mouseMove, mainData.window);
		UpdateWindow(dt, mouseMove, mainData.window);
		UpdateMusic(dt);
		UpdateSound(dt);
		break;
	case GAME:
		UpdateGame(dt, mainData.window);
		break;
	case GAME_OVER:
		UpdateGameOver(mainData.window, dt);
		break;
	case WIN:
		UpdateWin(mainData.window, dt);
		break;
	}


	UpdateTransition(mainData.window, dt);

	Reload();
}

void Draw(void)
{
	sfRenderWindow_clear(mainData.window, sfBlack);

	switch (GetGameState())
	{
	case CUTSCENE:
		DrawShotMovie(mainData.window);
		break;

	case LAUNCHING_GAME:
		DrawLaunch(mainData.window);
		break;
	case MENU:

		DrawMainMenu(mainData.window);
		DrawWindow(mainData.window);
		DrawSettingsMenu(mainData.window);
		DrawBorderScreen(mainData.window);
		break;
	case GAME:
		DrawGame(mainData.window);
		break;
	case GAME_OVER:
		DrawGameOver(mainData.window);
		break;
	case WIN:
		DrawWin(mainData.window);
		break;
	}
	DrawTransition(mainData.window);

	sfRenderWindow_display(mainData.window);


	void CleanUpShotMovie(void);

}

void CleanupGameState(void)
{
	switch (GetGameState())
	{
	case CUTSCENE:
		CleanUpShotMovie();
		break;
	case LAUNCHING_GAME:
		CleanUpLaunch();
	case MENU:
		CleanupMainMenu();
		CleanupMainMenu();
		break;
	case GAME:
		CleanupGame(mainData.window);
		break;
	case GAME_OVER:
		CleanupGameOver();
		break;
	case WIN:
		CleanupWin();
		break;
	}
}

void Cleanup(void)
{
	CleanupGameState();

	CleanupTransition();

	sfClock_destroy(mainData.gameClock);
	mainData.gameClock = NULL;

	sfRenderWindow_destroy(mainData.window);
	mainData.window = NULL;
}

void Reload(void)
{
	if (GetGameState() != mainData.saveGameState)
	{
		GameState temp = GetGameState();
		SetGameState(mainData.saveGameState);
		CleanupGameState();

		SetGameState(temp);
		LoadGameState();

		//StartTransitionPicture(sfWhite, 2, 10);


		mainData.saveGameState = GetGameState();
	}
}

