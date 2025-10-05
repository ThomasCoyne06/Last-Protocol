#include "Music.h"
#include "Sounds.h"

MusicStruct musicStruct;


//cleanup all music
void CleanupMusic(void);

//fade music, from 100 volume to 0 volume
sfBool FadeOutMusic(float _dt);

//when fade out finished, set music status to finish
void SetMusicFinish(void);

//reset the music variable
void ResetMusic(void);

void KeepOffset(MusicEnum _musicType, sfBool _keepPlayingOffset);

void LoadMusic(void)
{
	//music name
	char* musicName[] = { "FullLife", "MidLife", "LowLife", "MainMenuPart1", "MainMenuPart2", "PauseMenu" };

	//open all files from Sounds file in assets, with a loop and all file name in the tab "soundName"
	for (unsigned int i = 0; i < MUSIC_NMB; i++)
	{
		char fileName[70];

		sprintf_s(fileName, sizeof(fileName), "Assets/Musics/%s.ogg", musicName[i]);
		musicStruct.music[i] = sfMusic_createFromFile(fileName);
	}

	//init boolean
	musicStruct.isFading = sfFalse;
	musicStruct.isFinished = sfFalse;
	musicStruct.playingMusic = NO_MUSIC;
	musicStruct.wasPlayingMusic = NO_MUSIC;
	musicStruct.musicLevel = 10.0f;
}

void UpdateMusic(float _dt)
{
	//fade in music
	if (musicStruct.musicLevel == 0)
	{
		sfMusic_setVolume(musicStruct.music[musicStruct.playingMusic], 0);
	}
	else
	{
		if (sfMusic_getVolume(musicStruct.music[musicStruct.playingMusic]) <= musicStruct.musicLevel)
		{
			sfMusic_setVolume(musicStruct.music[musicStruct.playingMusic], sfMusic_getVolume(musicStruct.music[musicStruct.playingMusic]) + 1);
			if (musicStruct.wasPlayingMusic != NO_MUSIC)
			{
				sfMusic_setVolume(musicStruct.music[musicStruct.wasPlayingMusic], sfMusic_getVolume(musicStruct.music[musicStruct.wasPlayingMusic]) - 1);
			}
		}
		else if (sfMusic_getVolume(musicStruct.music[musicStruct.playingMusic]) > musicStruct.musicLevel)
		{
			sfMusic_setVolume(musicStruct.music[musicStruct.playingMusic], musicStruct.musicLevel);
		}
	}


	//reset music if music is finished
	if (musicStruct.playingMusic != NO_MUSIC)
	{
		if ((sfMusic_getStatus(musicStruct.music[musicStruct.playingMusic]) == sfStopped ||
			sfMusic_getStatus(musicStruct.music[musicStruct.playingMusic]) == sfPaused)
			&& musicStruct.isFinished == sfTrue)
		{
			ResetMusic();
			musicStruct.isFinished = sfFalse;
		}
	}

	//change music pitch if player is in bullet time
	if (musicStruct.playingMusic != NO_MUSIC)
	{
		if (GetPlayerBulletTime())
		{
			sfMusic_setPitch(musicStruct.music[musicStruct.playingMusic], 1 / BULLET_TIME_SLOW);
		}
		else
		{
			sfMusic_setPitch(musicStruct.music[musicStruct.playingMusic], 1.0f);

		}
	}
}

void PlayMusic(MusicEnum _musicType, sfBool _isLoop, sfBool _keepPlayingOffset)
{
	if (_musicType != musicStruct.playingMusic)
	{
		//base : play music and loop
		sfMusic_play(musicStruct.music[_musicType]);
		sfMusic_setLoop(musicStruct.music[_musicType], _isLoop);
		sfMusic_setVolume(musicStruct.music[_musicType], 0);

		if (_keepPlayingOffset)
		{
			KeepOffset(_musicType, _keepPlayingOffset);
		}

		//set new value in the variable
		musicStruct.wasPlayingMusic = musicStruct.playingMusic;
		musicStruct.playingMusic = _musicType;
		musicStruct.isFinished = sfFalse;
	}
}

void StopMusic()
{
	if (GetMusicPlayed() != NO_MUSIC)
	{
		sfMusic_stop(musicStruct.music[GetMusicPlayed()]);
	}
}

void SetFadingMusic(void)
{
	musicStruct.isFading = sfTrue;
}

sfBool IsMusicFinish(void)
{
	return musicStruct.isFinished;
}

MusicEnum GetMusicPlayed(void)
{
	return musicStruct.playingMusic;
}

void AddToMusicVolume(float _value)
{
	if (musicStruct.musicLevel + _value >= 100.0f)
	{
		musicStruct.musicLevel = 100.0f;
	}
	else if (musicStruct.musicLevel + _value <= 0.0f)
	{
		musicStruct.musicLevel = 0.0f;
	}
	else
	{
		musicStruct.musicLevel += _value;
	}
}

float GetMusicVolume(void)
{
	return musicStruct.musicLevel;
}

void CleanupMusic(void)
{
	for (unsigned int i = 0; i < MUSIC_NMB; i++)
	{
		sfMusic_destroy(musicStruct.music[i]);
		musicStruct.music[i] = NULL;
	}
}

sfBool FadeOutMusic(float _dt)
{
	//only if music can fade
	if (musicStruct.isFading == sfTrue)
	{
		//if music - value is higher than 0 or egal to 0
		if (sfMusic_getVolume(musicStruct.music[musicStruct.playingMusic]) <= MUSIC_FADE_END_RANGE)
		{
			ResetMusic();
			return sfTrue;
		}

		//reduce volume (based on the delta time)
		sfMusic_setVolume(musicStruct.music[musicStruct.playingMusic], sfMusic_getVolume(musicStruct.music[musicStruct.playingMusic]) - MUSIC_VOLUME_FADE * _dt);
		return sfFalse;
	}
	return sfFalse;
}

void SetMusicFinish(void)
{
	musicStruct.isFinished = sfTrue;
}

void ResetMusic(void)
{
	musicStruct.isFading = sfFalse;

	//reset music and var
	StopMusic(musicStruct.playingMusic);
	sfMusic_setVolume(musicStruct.music[musicStruct.playingMusic], MUSIC_VOLUME_BASE);

	musicStruct.playingMusic = NO_MUSIC;
}

//get playing position, and set it to the new music playing (only if _keepPlayingOffSet == sfTrue !)
void KeepOffset(MusicEnum _musicType, sfBool _keepPlayingOffset)
{
	if (_keepPlayingOffset && musicStruct.playingMusic != NO_MUSIC)
	{
		sfTime musicOffset = sfMusic_getPlayingOffset(musicStruct.music[musicStruct.playingMusic]);
		musicOffset.microseconds = musicOffset.microseconds + DECAL_MUSIC_KEEP_TIME;

		sfMusic_setPlayingOffset(musicStruct.music[_musicType], musicOffset);
		StopMusic(musicStruct.playingMusic);
	}
}