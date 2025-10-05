#ifndef MUSIC_H
#define MUSIC_H

//include
#include "../Utility/Common.h"

//define
#define MUSIC_VOLUME_FADE 20.0f
#define MUSIC_VOLUME_BASE 100.0f
#define MUSIC_FADE_END_RANGE 0.5f


#define DECAL_MUSIC_KEEP_TIME -20000

//struct and enum
typedef enum
{
	LIFE_FULL,
	LIFE_MID,
	LIFE_LOW,

	MAIN_MENU1,
	MAIN_MENU2,

	PAUSE_MENU,

	MUSIC_NMB,
	NO_MUSIC
}MusicEnum;

typedef struct
{
	sfMusic* music[MUSIC_NMB];

	sfBool isFading;
	sfBool isFinished;

	MusicEnum playingMusic;
	MusicEnum wasPlayingMusic;

	float musicLevel;

}MusicStruct;

//prototype function

//load all the music for the game
void LoadMusic(void);

void UpdateMusic(float _dt);

//play a music, and choose if it's loopable
void PlayMusic(MusicEnum _MusicType, sfBool _isLoop, sfBool _keepPlayingOffset);

//stop _musicType from playing if it's playing
void StopMusic(void);

//Make the end of the music.
//fade out, reset the music to NO_MUSIC
//stop music
void SetFadingMusic(void);

//get if the music is finished
sfBool IsMusicFinish(void);

MusicEnum GetMusicPlayed(void);

void AddToMusicVolume(float _value);

float GetMusicVolume(void);


#endif // !MUSIC_H
