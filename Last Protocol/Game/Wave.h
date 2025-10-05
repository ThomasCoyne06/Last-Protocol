#ifndef WAVE_H
#define WAVE_H

//include
#include "../Utility/Common.h"
#include "../Game/Player.h"
#include "../Game/HUD.h"

//define
#define WAVE_X_SIZE 733
#define WAVE_Y_SIZE 360

#define WAVE_FRAMERATE 10

#define HITBOX_X_SIZE 500
#define HITBOX_Y_SIZE 2000

#define INCREASE_WAVE_SPEED 25.0f
#define DECREASE_WAVE_SPEED 25.0f
#define ACCEPT_VALUE_WAVE_CLAMP 2.0f

#define HITBOX_WAVE_DECAL 350.0f

#define WAVE_POSITION_X_DECAL 210.0f

//struct and enum
typedef struct
{
	sfSprite* sprite;
	sfTexture* texture;

	sfVector2f spawnPosition;
	float endOfRun;

	AnimationData animation;

	sfRectangleShape* hitbox;

	float waveSpeed;
	float newWaveSpeed;
}WaveStruct;


//prototype and function
void LoadWave(void);
void UpdateWave(float _dt);
void DrawWave(sfRenderWindow* _window);
void CleanupWave(void);

//create a new wave with new start/end/speed
void SetSynthWave(float _speed, float _spawnX, float _endOfRun);

void SetWaveSpeed(float _speed);

float GetWavePositionX(void);

float getWaveSpeed(void);

sfBool IsRunFinished(void);




#endif // !WAVE_H
