#include "HUD.h"
#include "Player.h"
#include "../Utility/Camera.h"
#include "../Resources/Music.h"
#include "Level.h"

HudData hudStruct;


//clamp between 0 and 1 a float value
float ClampValueHUD(float _value);

void SwitchLifeTexture(int _lifeValue, float _dt);

//void HideSlider(void);
void LoadHud(void)
{

	//sprite variable

	hudStruct.lifeSprite = sfSprite_create();

	sfTexture* fileName = sfTexture_createFromFile("Assets/Textures/HUD/Heal/Life.png", NULL);
	//sfSprite_setTexture(hudStruct.lifeSprite, fileName, NULL);
	sfIntRect frame = { 0, 82, 113, 30 };

	hudStruct.lifeSprite = CreateSpriteFromSpriteSheet(fileName, frame, sfFalse, SCALE * SCALE_TO_ADD);
	hudStruct.lifeAnimation1 = CreateAnimation(hudStruct.lifeSprite, 4, ONE_LIFE_LEFT_SPEED_ANIM, frame, sfTrue);

	//animation creation of life
	//sfSprite_setScale(hudStruct.lifeSprite, (sfVector2f) { SCALE * SCALE_TO_ADD, SCALE * SCALE_TO_ADD});

	//bullet time HUD slider
	frame = (sfIntRect){ 0, 544, 82, 10 };
	hudStruct.energyBar = CreateSpriteFromSpriteSheet(fileName, frame, sfFalse, SCALE * SCALE_TO_ADD);

	const sfIntRect rectReact = { 0, 604, 86, 22 };
	hudStruct.textureEnergyReact = sfTexture_createFromFile("Assets/Textures/HUD/Heal/Life.png", &rectReact);
	hudStruct.spriteEnergyReact = CreateSimpleSprite(hudStruct.textureEnergyReact, SCALE * SCALE_TO_ADD);

	//call all needed function for init
	hudStruct.decalSliderY = 0.0f;
	SetLifeHUD(6);

	hudStruct.fontScore = sfFont_createFromFile("Assets/Fonts/GlobalFont.ttf");
	char textBuffer[20];
	//sprintf_s(textBuffer, 19, "X %d", score);
	hudStruct.textScore = CreateText(hudStruct.fontScore, 0, 0, 15 * SCALE, sfWhite, sfFalse, textBuffer);

	hudStruct.fileTexture = sfTexture_createFromFile("Assets/Textures/HUD/ScoreFile.png", NULL);
	hudStruct.fileSprite = CreateSprite(hudStruct.fileTexture, (sfVector2f) {0,0}, sfTrue, SCALE);
}

void UpdateHud(float _dt, sfRenderWindow* _window)
{

	SwitchLifeTexture(hudStruct.life, _dt);


	LinkSpriteToCamera(hudStruct.lifeSprite, _window, (sfVector2f) { LIFE_X_POSITION, LIFE_Y_POSITION });

	float energyValue = GetEnergy();
	energyValue = energyValue / ENERGY_MAX;

	sfSprite_setTextureRect(hudStruct.energyBar, (sfIntRect) {0, 544, 82 * energyValue, 10});

	LinkSpriteToCamera(hudStruct.energyBar, _window, (sfVector2f) { LIFE_X_POSITION + OFFSET_ENERGY_BAR_X, LIFE_Y_POSITION + OFFSET_ENERGY_BAR_Y});
	LinkSpriteToCamera(hudStruct.spriteEnergyReact, _window, (sfVector2f) { LIFE_X_POSITION + OFFSET_ENERGY_BAR_X, LIFE_Y_POSITION + 50});
	
	LinkSpriteToCamera(hudStruct.textScore, _window, (sfVector2f) { SCORE_X_POSITION, SCORE_Y_POSITION});
	LinkSpriteToCamera(hudStruct.fileSprite, _window, (sfVector2f) { SCORE_X_POSITION, SCORE_Y_POSITION});
	char textBuffer[20];
	sprintf_s(textBuffer, 19, "%d / %d   ", fileCollected, GetLevel()->entityList[PICKUP_FILE].count);
	sfText_setString(hudStruct.textScore, textBuffer);
	sfFloatRect rect = sfText_getGlobalBounds(hudStruct.textScore);
	sfText_setOrigin(hudStruct.textScore, (sfVector2f) { rect.width, rect.height / 2 + 10 });

	if (hudStruct.canShowEnergyReact)
	{
		hudStruct.timerEnergyReact += _dt;
		if (hudStruct.timerEnergyReact > TIMER_REACT)
		{
			hudStruct.timerEnergyReact = 0;
			hudStruct.canShowEnergyReact = sfFalse;
		}
	}
}

void DrawHud(sfRenderWindow* _window)
{
	if (hudStruct.life > 0)
	{
		sfRenderWindow_drawSprite(_window, hudStruct.energyBar, NULL);
		sfRenderWindow_drawSprite(_window, hudStruct.lifeSprite, NULL);
	}

	sfRenderWindow_drawText(_window, hudStruct.textScore, NULL);
	sfRenderWindow_drawSprite(_window, hudStruct.fileSprite, NULL);

	if (hudStruct.canShowEnergyReact)
	{
		sfRenderWindow_drawSprite(_window, hudStruct.spriteEnergyReact, NULL);
	}
}

void CleanupHud(void)
{

	sfSprite_destroy(hudStruct.lifeSprite);


	hudStruct = (HudData){ 0 };
}



void SetLifeHUD(int _lifeValue)
{
	if (_lifeValue >= MIN_LIFE_HUD && _lifeValue <= MAX_LIFE_HUD)
	{
		hudStruct.life = _lifeValue;
	}

	//printf("%d", GetLevel()->number);
	if (GetLevel()->number == LEVEL_3)
	{
		PlayMusic(LIFE_LOW, sfTrue, sfFalse);
	}
	else if (_lifeValue > 4)
	{
		PlayMusic(LIFE_FULL, sfTrue, sfFalse);
	}
	else if (_lifeValue > 2)
	{
		PlayMusic(LIFE_MID, sfTrue, sfFalse);
	}
	else
	{
		PlayMusic(LIFE_LOW, sfTrue, sfFalse);
	}
}

void SwitchLifeTexture(int _lifeValue, float _dt)
{
	//switch texture on amount of life remaining
	//switch music too
	sfIntRect frame;
	switch (_lifeValue)
	{
	case 0:
		UpdateAnimation(&hudStruct.lifeAnimation1, _dt);
		break;
	case 1:
		UpdateAnimation(&hudStruct.lifeAnimation1, _dt);
		break;
	case 2:
		frame = (sfIntRect){ 0, 144, 113, 30 };
		sfSprite_setTextureRect(hudStruct.lifeSprite, frame);
		break;
	case 3:
		frame = (sfIntRect){ 0, 208, 113, 30 };
		sfSprite_setTextureRect(hudStruct.lifeSprite, frame);
		break;
	case 4:
		frame = (sfIntRect){ 0, 272, 113, 30 };
		sfSprite_setTextureRect(hudStruct.lifeSprite, frame);
		break;
	case 5:
		frame = (sfIntRect){ 0, 336, 113, 30 };
		sfSprite_setTextureRect(hudStruct.lifeSprite, frame);
		break;
	case 6:
		frame = (sfIntRect){ 0, 400, 113, 30 };
		sfSprite_setTextureRect(hudStruct.lifeSprite, frame);
		break;
	default:
		break;
	}
}

int GetLifeHUD(void)
{
	return hudStruct.life;
}

void showlife(sfBool _showIt)
{
	hudStruct.canShowLife = _showIt;
}




float ClampValueHUD(float _value)
{
	float returnValue = _value;

	//don't let icon go out of the slider range
	if (_value > POSITION_MAX)
	{
		returnValue = POSITION_MAX;
	}
	else if (_value < POSITION_MIN)
	{
		returnValue = POSITION_MIN;
	}

	return returnValue;
}


// .-. .-. .-.
// |.. |-   |
// `-' `-'  '

const HudData* GetHUD(void)
{
	return &hudStruct;
}

// .-. .-. .-.
// `-. |-   |
// `-' `-'  '

HudData* SetHUD(void)
{
	return &hudStruct;
}