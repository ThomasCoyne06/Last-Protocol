#ifndef BOSSHUD_H
#define BOSSHUD_H

//include
#include "Utility/Common.h"
#include "Game/Boss.h"

//define
#define BOSS_HUD_POSITION (sfVector2f) {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60}
#define OFFSET_LITTLE_FILL_HUD 228


//struct and enum

typedef enum
{

	BORDER_BOSS_HAND_HUD,
	BAR_FILL_BOSS_HAND_LEFT_HUD,
	BAR_FILL_BOSS_HAND_RIGHT_HUD,

	BORDER_BOSS_HEAD_HUD,
	BAR_FILL_BOSS_HEAD_HUD,

	LIFE_BAR_BOSS_OBJECT_NUMBER

}LifeBarBossObjectEnum;

typedef struct
{

	sfSprite* sprite[LIFE_BAR_BOSS_OBJECT_NUMBER];
	sfTexture* texture;

	sfIntRect frameRect[LIFE_BAR_BOSS_OBJECT_NUMBER];

}LifeBarBossStruct;


//proto function
void LoadHUDboss(void);
void UpdateHUDboss(float _dt);
void DrawHUDboss(sfRenderWindow* _window);
void CleanupHUDBoss(void);



#endif // !BOSSHUD_H
