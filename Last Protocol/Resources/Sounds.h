#ifndef SOUNDS_H
#define SOUNDS_H
#define _USE_MATH_DEFINES

//include
#include "../Last Protocol/Utility/Common.h"
#include "../Last Protocol/Game/Player.h"
#include <math.h>
//define

#define BASE_VOLUME_CALCUL 250
#define DELAY_AMBIENT_SOUND 25.0f

//struct and enum

//enumeration of all sound disponible in the game.
typedef enum
{
	DETECT_PLAYER_ENEMY,
	EXPLOSION_ENEMY,
	POP_UP,
	BOSS_APPEAR,
	BASIC_HIT,
	BIG_DIE,
	BIG_HIT,
	ENEMY_JUMP_ON,
	EYE_DIE,
	JUMPER_ACTIVE,
	JUMPER_DIE,
	JUMPER_HIT,
	JUMPER_ON,
	MINI_DIE,
	FIRE_ENEMY,
	FLYING_ENEMY,

	DASH_PLAYER,
	DEATH_PLAYER,
	SWORD_SLASH_PLAYER,
	SWORD_SLASH_2_PLAYER,
	SWORD_SLASH_3_PLAYER,
	JUMP_PLAYER,
	LAND_PLAYER,
	SLIDE_PLAYER,
	FALL_PLAYER,
	HIT_PLAYER,
	BUMPER_PLAYER,
	PICK_LIFE,
	PICK1,
	PICK2,
	PICK3,
	PICK4,

	AMBIENT1,
	AMBIENT2,
	AMBIENT3,
	AMBIENT4,
	AMBIENT5,
	AMBIENT6,

	OPEN_GAME,

	GRASS_SOUND,
	LIANA_SOUND,

	SOUND_NB
}SoundEnum;

typedef enum
{
	ENEMY,
	PLAYER,

	TYPEFILE_NB
}TypeFileEnum;

//structure to access all data about sound in the game.
typedef struct
{
	sfSound* sound[SOUND_NB];
	sfSoundBuffer* soundBuffer[SOUND_NB];
	sfSound** allSound;
	int numberOfPlayedSound;

	float soundVolume;

	sfBool canPlaySound;
	float timerAmbient;
	sfSound* soundTemp;
	//int countSound;
}SoundStruct;


//prototype function

//load all sound
void LoadAllSound(void);

void UpdateSound(float _dt);

//Play a new sound at the position choosed. the volume will depend of the _soundposition.
//looping and non spacial sound can be choose.
void PlaySound(SoundEnum _soundType, sfVector2f _soundPosition, sfBool _isLooping, sfBool _isSpacial);

//stop sound from playing
void StopSound(int _index);
//Cleanup Sound
void CleanupSound(void);

void DestroyAllSound(void);

float GetSoundsVolume(void);

void AddToSoundsVolume(float _value);

#endif // !SOUNDS
