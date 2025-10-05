#include "BossHUD.h"

LifeBarBossStruct lifebar;

float GetSliderValueBossHUD(float _value, float _max);
void SetSliderWidth(LifeBarBossObjectEnum _index, float _value);


void LoadHUDboss(void)
{
	lifebar.texture = sfTexture_createFromFile("Assets/Textures/Boss/LifeBar.png", NULL);

	lifebar.frameRect[0] = (sfIntRect){ 0, 85, 304, 20 };
	lifebar.frameRect[1] = (sfIntRect){ 0, 257, 148, 12 };
	lifebar.frameRect[2] = (sfIntRect){ 0, 257, 148, 12 };
	lifebar.frameRect[3] = (sfIntRect){ 0, 139, 304, 20 };
	lifebar.frameRect[4] = (sfIntRect){ 0, 319, 300, 12 };

	for (unsigned int i = 0; i < LIFE_BAR_BOSS_OBJECT_NUMBER; i++)
	{
		lifebar.sprite[i] = CreateSpriteFromSpriteSheet(lifebar.texture, lifebar.frameRect[i], sfTrue, SCALE);
		if (i == BAR_FILL_BOSS_HAND_LEFT_HUD)
		{
			sfSprite_setPosition(lifebar.sprite[i], (sfVector2f) { BOSS_HUD_POSITION.x + OFFSET_LITTLE_FILL_HUD, BOSS_HUD_POSITION.y });
		}
		else if (i == BAR_FILL_BOSS_HAND_RIGHT_HUD)
		{
			sfSprite_setPosition(lifebar.sprite[i], (sfVector2f) { BOSS_HUD_POSITION.x - OFFSET_LITTLE_FILL_HUD, BOSS_HUD_POSITION.y });
			sfSprite_setScale(lifebar.sprite[i], (sfVector2f) { -SCALE, SCALE });
		}
		else
		{
			sfSprite_setPosition(lifebar.sprite[i], BOSS_HUD_POSITION);
		}
	}
}

void UpdateHUDboss(float _dt)
{
	if (GetActualPhase() == PHASE_HAND)
	{
		float sliderValue = GetSliderValueBossHUD((float)GetLifeOfHand(LEFT_HAND), (float)HP_HAND);
		SetSliderWidth(BAR_FILL_BOSS_HAND_LEFT_HUD, sliderValue);

		sliderValue = GetSliderValueBossHUD((float)GetLifeOfHand(RIGHT_HAND), (float)HP_HAND);
		SetSliderWidth(BAR_FILL_BOSS_HAND_RIGHT_HUD, sliderValue);

	}
	else if (GetActualPhase() == PHASE_HEAD)
	{

		float sliderValue = GetSliderValueBossHUD((float)GetLifeOfHead(), (float)HP_HEAD);
		SetSliderWidth(BAR_FILL_BOSS_HEAD_HUD, sliderValue);

	}
}

void DrawHUDboss(sfRenderWindow* _window)
{
	if (GetActualPhase() == PHASE_HAND)
	{
		sfRenderWindow_drawSprite(_window, lifebar.sprite[BAR_FILL_BOSS_HAND_LEFT_HUD], NULL);
		sfRenderWindow_drawSprite(_window, lifebar.sprite[BAR_FILL_BOSS_HAND_RIGHT_HUD], NULL);
		sfRenderWindow_drawSprite(_window, lifebar.sprite[BORDER_BOSS_HAND_HUD], NULL);
	}
	else if (GetActualPhase() == PHASE_HEAD)
	{
		sfRenderWindow_drawSprite(_window, lifebar.sprite[BAR_FILL_BOSS_HEAD_HUD], NULL);
		sfRenderWindow_drawSprite(_window, lifebar.sprite[BORDER_BOSS_HEAD_HUD], NULL);
	}
}

void CleanupHUDBoss(void)
{
}

float GetSliderValueBossHUD(float _value, float _max)
{
	return _value / _max;
}

void SetSliderWidth(LifeBarBossObjectEnum _index, float _value)
{
	sfIntRect frameRect = lifebar.frameRect[_index];


	frameRect.left = (int)((float)(frameRect.width) * (1.0f - _value));


	sfSprite_setTextureRect(lifebar.sprite[_index], frameRect);
}
