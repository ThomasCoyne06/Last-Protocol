#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Utility/Common.h"
#include "Utility/Creation.h"

//define
#define SETTINGS_TEXT_SIZE 25
#define SLIDER_SPEED 1.0f

#define CROSS_PAUSE_SETTINGS_POSITION {810, 563}
#define DECREASE_FX_VOLUME_POSITION {320, 730}
#define INCREASE_FX_VOLUME_POSITION {490,730}
#define DECREASE_MUSIC_VOLUME_POSITION {570,730}
#define INCREASE_MUSIC_VOLUME_POSITION {740,730}

#define SETTINGS_BUTTON_PAUSE_POSITION {755, 505}
#define CONTINUE_BUTTON_PAUSE_POSITION { 750, 450}
#define QUIT_PAUSE_POSITION {730, 555}
#define CROSS_PAUSE_POSITION {1325, 345}

#define SETTINGS_POPUP_POSITION (sfVector2f) { 250, 550 }

#define BORDER_POSITION_LEFT 100.0f
#define BORDER_POSITION_RIGHT SCREEN_WIDTH - 100.0f
#define BORDER_POSITION_BOT SCREEN_HEIGHT - 100.0f
#define BORDER_POSITION_TOP 100.0f

//struct and enum
typedef enum
{
	WINDOW_PAUSE,

	SETTINGS_BUTTON_PAUSE,
	CONTINUE_BUTTON_PAUSE,
	QUIT_PAUSE,
	CROSS_PAUSE,

	OBJECT_PAUSE_NMB
}PauseMenuObjectEnum;

typedef enum
{
	SETTINGS_POPUP,
	CROSS_SETTINGS,

	SETTINGS_OBJECT_NMB

}SettingsObjectEnum;

typedef enum
{
	CROSS_PAUSE_SETTINGS,
	DECREASE_FX_VOLUME,
	INCREASE_FX_VOLUME,

	DECREASE_MUSIC_VOLUME,
	INCREASE_MUSIC_VOLUME,

	ALL_NAME_NMB
}SettingsHitboxNameEnum;

typedef struct
{
	//base popup
	sfSprite* smellsLikeTeenSprite[OBJECT_PAUSE_NMB];
	sfTexture* texture[OBJECT_PAUSE_NMB];
	sfRectangleShape* hitbox[OBJECT_PAUSE_NMB - 1];
	PauseMenuObjectEnum buttonClicked;
	sfSprite* cursor;
	sfVector2f positionCursor;

	AnimationData backAnim;

	//settings
	sfSprite* settingsSprite[SETTINGS_OBJECT_NMB];
	sfTexture* settingsTexture[SETTINGS_OBJECT_NMB];
	sfRectangleShape* hitboxSettings[ALL_NAME_NMB];
	sfBool settingsOpen;
	sfText* textLevelSound[2];

}PauseMenuStruct;

void LoadPauseMenu(void);
void UpdatePauseMenu(float _dt, sfRenderWindow* _window);
void DrawPauseMenu(sfRenderWindow* _window);
void DrawSettingsMenu(sfRenderWindow* _window);
void CleanupPauseMenu(void);

MouseMovedCursor(sfMouseMoveEvent _mouseMove);

PauseMenuObjectEnum GetButtonClicked(void);
void ResetClickedButton(void);
void SetSettingsVisibility(sfBool _isvisi);

sfVector2f GetCursorPositionOnPause(void);


#endif // !PAUSEMENU_H
