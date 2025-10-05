#include "Wave.h"

WaveStruct waveStruct;

void MoveWave(float _dt);
void CollisionWavePlayer(void);
void SetWavePosition(float _positionX);

void SpeedGestionWave(float _dt);

void LoadWave(void)
{
	//load wave image and create animation
	waveStruct.sprite = sfSprite_create();
	waveStruct.texture = sfTexture_createFromFile("Assets/Textures/Wave/Wave.png", NULL);
	sfSprite_setTexture(waveStruct.sprite, waveStruct.texture, sfFalse);
	sfIntRect wave = { 0, 0, WAVE_X_SIZE, WAVE_Y_SIZE };
	waveStruct.animation = CreateAnimation(waveStruct.sprite, 4, WAVE_FRAMERATE, wave, sfTrue);
	waveStruct.endOfRun = 0.0f;

	//rescale and set origin and position
	sfFloatRect hitbox = sfSprite_getGlobalBounds(waveStruct.sprite);
	sfSprite_setScale(waveStruct.sprite, (sfVector2f) { 3.0, 3.0 });
	sfSprite_setOrigin(waveStruct.sprite, (sfVector2f) { hitbox.width / 4, hitbox.height / 2 });
	sfSprite_setPosition(waveStruct.sprite, (sfVector2f) { 0, SCREEN_HEIGHT / 2 });

	//hitbox creation
	waveStruct.hitbox = sfRectangleShape_create();
	sfRectangleShape_setFillColor(waveStruct.hitbox, sfTransparent);
	sfRectangleShape_setOutlineColor(waveStruct.hitbox, sfRed);
	sfRectangleShape_setOutlineThickness(waveStruct.hitbox, 5.0f);
	sfRectangleShape_setSize(waveStruct.hitbox, (sfVector2f) { HITBOX_X_SIZE, HITBOX_Y_SIZE });
	sfRectangleShape_setOrigin(waveStruct.hitbox, (sfVector2f) { HITBOX_X_SIZE / 2, HITBOX_Y_SIZE / 2 });
	sfRectangleShape_setPosition(waveStruct.hitbox, (sfVector2f) { HITBOX_X_SIZE - 850, SCREEN_HEIGHT / 2 });


}

void UpdateWave(float _dt)
{
	//if wave at the end of the run, then stop moving
	if (sfSprite_getPosition(waveStruct.sprite).x <= waveStruct.endOfRun)
	{
		MoveWave(_dt);

	}
	else
	{
		SetInRun(sfFalse);
	}

	SpeedGestionWave(_dt);
	//animation and collision
	UpdateAnimation(&waveStruct.animation, _dt);
	CollisionWavePlayer();


}

void DrawWave(sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, waveStruct.sprite, NULL);
	//sfRenderWindow_drawRectangleShape(_window, waveStruct.hitbox, NULL);
}

void CleanupWave(void)
{
	sfSprite_destroy(waveStruct.sprite);
	waveStruct.sprite = NULL;

	sfTexture_destroy(waveStruct.texture);
	waveStruct.texture = NULL;

	sfRectangleShape_destroy(waveStruct.hitbox);
	waveStruct.hitbox = NULL;
}

void SetSynthWave(float _speed, float _spawnX, float _endOfRun)
{
	waveStruct.waveSpeed = _speed;
	waveStruct.newWaveSpeed = _speed;
	SetWavePosition(_spawnX);
	waveStruct.endOfRun = _endOfRun;

}

//at any time, change the speed to this value
//it will decrease with the time to make a deceleration
void SetWaveSpeed(float _speed)
{
	waveStruct.newWaveSpeed = _speed;
}

float GetWavePositionX(void)
{
	return sfSprite_getPosition(waveStruct.sprite).x - WAVE_POSITION_X_DECAL;
}

float getWaveSpeed(void)
{
	return waveStruct.waveSpeed;
}

sfBool IsRunFinished(void)
{

	return sfSprite_getPosition(waveStruct.sprite).x >= waveStruct.endOfRun;
}

//change wave position and hitbox at the right position needed
void SetWavePosition(float _positionX)
{
	sfSprite_setPosition(waveStruct.sprite, (sfVector2f) { _positionX, sfSprite_getPosition(waveStruct.sprite).y });
	sfRectangleShape_setPosition(waveStruct.hitbox, (sfVector2f) { _positionX - HITBOX_WAVE_DECAL, 0 });
}

void SpeedGestionWave(float _dt)
{
	if (waveStruct.waveSpeed > waveStruct.newWaveSpeed - ACCEPT_VALUE_WAVE_CLAMP &&
		waveStruct.waveSpeed < waveStruct.newWaveSpeed + ACCEPT_VALUE_WAVE_CLAMP)
	{
		waveStruct.waveSpeed = waveStruct.newWaveSpeed;
	}
	else if (waveStruct.waveSpeed > waveStruct.newWaveSpeed)
	{
		waveStruct.waveSpeed -= DECREASE_WAVE_SPEED * _dt;
	}
	else if (waveStruct.waveSpeed < waveStruct.newWaveSpeed)
	{
		waveStruct.waveSpeed += INCREASE_WAVE_SPEED * _dt;
	}
}

void MoveWave(float _dt)
{
	sfSprite_move(waveStruct.sprite, (sfVector2f) { waveStruct.waveSpeed* _dt, 0 });
	sfRectangleShape_move(waveStruct.hitbox, (sfVector2f) { waveStruct.waveSpeed* _dt, 0 });

}

void CollisionWavePlayer(void)
{
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
	if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(waveStruct.hitbox), playerHitbox))
	{
		//
		// PLAYER NEED TO DIE !!! /endgame
		//
	}
}
