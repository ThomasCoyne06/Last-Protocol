#ifndef HUD_H
#define HUD_H

#include "../Utility/Common.h"

#define SCALE_SPRITE_SIZE_CELL 50 
#define SCALE_WIDTH_SPRITE 50
#define SCALE_HEIGHT_SPRITE 50



//for slider maximum value
#define POSITION_MAX 0.97f

//for slider minimum value
#define POSITION_MIN 0.03f

#define LIFE_SIZE_SPRITE 100

//minimum size of the scale object
#define SCALE_MINIMUM 1
//maximum size of the scale object
#define SCALE_MAXIMUM 3

#define SCORETEXT_X_LOCATION 150
#define SCORETEXT_Y_POSITION 150

#define LIFE_X_POSITION 60
#define LIFE_Y_POSITION 70

#define LIFE_X_SIZE 96
#define LIFE_Y_SIZE 14
#define DECAL_HUD_WAVE 150.0f

#define MIN_LIFE_HUD 0
#define MAX_LIFE_HUD 6
#define ONE_LIFE_LEFT_SPEED_ANIM 16

#define SCALE_TO_ADD 1.5f

#define OFFSET_ENERGY_BAR_X 29 * SCALE * SCALE_TO_ADD
#define OFFSET_ENERGY_BAR_Y 18 * SCALE * SCALE_TO_ADD

#define SCORE_X_POSITION SCREEN_WIDTH - 50
#define SCORE_Y_POSITION 50

#define TIMER_REACT 0.15f

typedef struct
{
	int life;
	sfSprite* lifeSprite;
	sfTexture* lifeTexture;
	AnimationData lifeAnimation1;
	AnimationData lifeAnimation2;
	AnimationData lifeAnimation3;
	AnimationData lifeAnimation4;
	AnimationData lifeAnimation5;
	AnimationData lifeAnimation6;

	sfTexture* textureEnergyReact;
	sfSprite* spriteEnergyReact;
	float timerEnergyReact;
	sfSprite* energyBar;
	sfTexture* bulletTexture;

	float decalSliderY;
	sfBool canShowSlider;
	sfBool canShowEnergyReact;
	sfBool canShowLife;

	sfFont* fontScore;
	sfText* textScore;

	sfTexture* fileTexture;
	sfSprite* fileSprite;

}HudData;

//base function

void LoadHud();

void UpdateHud(float _dt, sfRenderWindow* _windows);

void DrawHud(sfRenderWindow* _windows);

void CleanupHud();


//set value of life remaining of the player on the HUD
void SetLifeHUD(int _lifeValue);
int GetLifeHUD(void);
void showlife(sfBool _showIt);

const HudData* GetHUD(void);
HudData* SetHUD(void);

#endif
