#ifndef MAINMENU_H
#define MAINMENU_H

//include
#include "Utility/Common.h"
#include "Utility/Collision.h"
#include "MainMenuWindow.h"
#include "Utility/Camera.h"


//define

#define QUIT_FADE_SPEED 20.0f

#define ICON_X_POSITION 200.0f
#define ICON_Y_POSITION 200.0f

#define BORDER_POSITION_LEFT 140.0f
#define BORDER_POSITION_RIGHT SCREEN_WIDTH - 150.0f
#define BORDER_POSITION_BOT SCREEN_HEIGHT - 150.0f
#define BORDER_POSITION_TOP 140.0f

#define MUSIC_TIMER 6.f

#define MODULO_SIZE 20

#define COW_POS_X 1430
#define COW_POS_Y 700
#define COW_SPEED 20
#define COW_TIMER 4.f
#define COW_CHANGE 0.5f

//define for popup spawn and random position
#define NUMBER_POPUP 100
#define SCALE_POPUP 2.5f
#define NUMBER_RANDOM_LOC_POPUP 10
#define BASE_DELAY_POPUP 0.85f;
#define LIMIT_DELAY_POPUP 0.025f
#define COEF_APPARITION 0.75f
#define RANDOM_POSITION_MODULO 150
#define BASE_RANDOM_POSITION 30
#define BASE_RESPAWN_POPUP_POSITION_X 100
#define BASE_RESPAWN_POPUP_POSITION_Y 100
#define LOCATIONX_POPUP_TO_ADD 100
#define LOCATIONY_POPUP_TO_ADD 100

//double click
#define DOUBLE_CLICK_TIMER 0.1f
#define DELAY_TO_DOUBLE_CLICK 0.03f
#define IF_YOU_ARE_555_THEN_I_AM 666.0f

#define QUIT_BUTTON_POSITION (sfVector2f) { 700, 687 }

//enum and struct
typedef enum
{
	WALLPAPER,

	PLAY_BUTTON,
	OPTION_BUTTON,
	CREDITS_BUTTON,
	QUIT_BUTTON,
	COW,

	BACKGROUND,
	CROSS,
	OK_BUTTON,


	NMB_SPRITE
}SpriteNameEnum;



typedef struct
{
	sfSprite* sprite[NMB_SPRITE];
	sfSprite* popUp[NUMBER_POPUP];

	sfTexture* texture[NMB_SPRITE];
	sfTexture* wallpaperAnimTexture;
	sfTexture* popUpTexture;

	sfBool isBouncing;
	sfBool playPopup;
	sfBool quitHit;
	sfBool isGrabing;
	sfBool enterGame;
	sfBool clickPlayExe;
	sfBool hasClickOnce;
	sfBool doubleClick;
	sfBool hasRelease;
	sfBool showCow;

	float delaySpawnPopup;
	float timerPopUp;
	float timerToDoubleClick;
	float delayDouble;

	sfVector2f location;

	int grabIndex;

	sfRectangleShape* popUpHitbox[3];

	AnimationData cowAnim;
	AnimationData backgroundAnim;

	sfVector2f positionCursor;

	sfTexture* textureCow;
	sfSprite* spriteCow;
	sfBool runCow;
	float timerCow;
	sfBool directionCow;
	float timerCowFear;

	sfSoundBuffer* popUpBuffer;
	sfSound* popUpSound;

	sfMusic* popUpMusic;

}MainMenuStruct;


//prototype function

void LoadMainMenu(void);
void UpdateMainMenu(float _dt, sfMouseMoveEvent _mouseMove, sfRenderWindow* _window);
void DrawMainMenu(sfRenderWindow* _window);
void CleanupMainMenu(void);

//grab function

void GrabFileMainMenu(int _index, sfMouseMoveEvent _mouseMove);
void DropFileMainMenu(int _index, sfMouseMoveEvent _mouseMove);

void SpawnPopUpWindow(float _dt);
void CreatePopupSprite(int _index);
void RandomPopupLocation(void);
void SetUpPopupInFallingStyle(void);

#endif // !MAINMENU_H
