#ifndef GAME_H
#define GAME_H

#include "../Utility/Common.h"
#include "../GameState/Menu.h"
#include "../GameState/GameOver.h"

#include "HUD.h"
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
#include "Background.h"
#include "Pickup.h"
#include "Laser.h"
#include "Bumper.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Miscellenous.h"
#include "Sign.h"
#include "BulletTime.h"
#include "Door.h"
#include "../PauseMenu.h"
#include "Boss.h"
#include "../BossHUD.h"
#include "Props.h"
#include "../Stats.h"

#include "../Resources/Music.h"
#include "../Resources/Sounds.h"

#include "../GameState/Menu.h"
#include "../GameState/GameOver.h"

#define LOADING_TIME 3.f
#define TEXT_TIME 4.2


typedef enum
{
	TEXT_LEVEL_1,
	TEXT_LEVEL_2,
	TEXT_LEVEL_3,

	NB_TEXT_LEVEL
}TextLevel;
typedef struct
{
	sfTexture* textureBackground;
	sfSprite* spriteBackground;

	sfVector2f mousePosition;

	sfBool pause;
	sfBool drawHitbox;
	sfBool drawOrigin;

	sfView* camera;

	sfBool isPausing;

	float loadingTimer;
	unsigned actualLevel;

	sfFont* levelFont;
	sfText* levelText[NB_TEXT_LEVEL];

	float textTimer;
}GameData;

void LoadGame(LevelNumber _levelNumber);
void MouseMovedGame(sfMouseMoveEvent _mouseMove);
void UpdateGame(float _dt, sfRenderWindow* _window);
void DrawGame(sfRenderWindow* _window);
void CleanupGame(sfRenderWindow* _window);
void KeypressedGame(sfRenderWindow* _window, sfKeyEvent _key);


#endif
