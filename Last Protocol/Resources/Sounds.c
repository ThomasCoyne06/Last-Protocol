#include "Sounds.h"

SoundStruct soundStruct;


//load buffer for all sound
void LoadBufferSound(void);

//destroy sound
void DestroySound(void);

//return distance between player and sound (Pythagora)
float GetDistancePlayerSound(int _index);

//calcul the value of new volume, depending of _value : 
//return BASE_VOLUME_CALCUL/(BASE_VOLUME_CALCUL+_value)
float GetValueOfVolume(float _value);

//set new sound for each sound in structure
//will be more/less loud by the distance
void SetSoundVolumeOnDistance(void);

//sort the list of Sound (important for destroy)
void SortSoundList(int _index);

//realloc the structure of sound
//return True if realloc worked
sfBool ReallocStructSound(int _decalSound);

//if modification is needed on a song
void ModificationOnSound(SoundEnum _soundType);

//the pitch of the sound will be generated randomly between _min and _max value
//default pitch is 1.
void RandomizePitch(float _min, int _max);

void RandomAmbientSound(float _dt);



void LoadBufferSound(void)
{

	soundStruct.soundVolume = 50.0f;
	soundStruct.canPlaySound = sfTrue;

	//sound name
	char* soundName[] = { "DetectPlayerEnemy", "ExplosionEnemy","PopUp", "BossAppear", "BasicHit", "BigDie", "BigHit", "EnemyJumpOn", "EyeDie", "JumperActive", "JumperDie", "JumperHit", "JumperOn", "MiniDie", "FireEnemy", "FlyingEnemy",
							"DashPlayer", "DeathPlayer", "SwordSlash", "SwordSlash2","SwordSlash3", "JumpPlayer", "LandPlayer", "SlidePlayer", "FallPlayer", "HitPlayer", "BumperPlayer", "PickLife", "Pick1", "Pick2", "Pick3", "Pick4",
							"Ambient1", "Ambient2", "Ambient3", "Ambient4", "Ambient5", "Ambient6", "OpenGame", "Grass", "Liana"};
	char* fileSoundName[] = { "Enemy", "Player", "Ambient", "GameOpen", "Props"};

	//open all files from Sounds file in assets, with a loop and all file name in the tab "soundName"

	for (unsigned int i = 0 + 1; i < DASH_PLAYER; i++)
	{
		char fileName[70];
		sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.wav", fileSoundName[0], soundName[i]);//sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.ogg", fileSoundName[0], soundName[i]);
		soundStruct.soundBuffer[i] = sfSoundBuffer_createFromFile(fileName);
	}
	for (unsigned int i = DASH_PLAYER; i <= PICK4; i++)
	{
		char fileName[70];
		sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.wav", fileSoundName[1], soundName[i]);
		soundStruct.soundBuffer[i] = sfSoundBuffer_createFromFile(fileName);
	}
	for (unsigned int i = AMBIENT1; i <= AMBIENT6; i++)
	{
		char fileName[70];
		sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.wav", fileSoundName[2], soundName[i]);
		soundStruct.soundBuffer[i] = sfSoundBuffer_createFromFile(fileName);
	}
	for (unsigned int i = OPEN_GAME; i <= OPEN_GAME; i++)
	{
		char fileName[70];
		sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.wav", fileSoundName[3], soundName[i]);
		soundStruct.soundBuffer[i] = sfSoundBuffer_createFromFile(fileName);
	}
	for (unsigned int i = GRASS_SOUND; i <= LIANA_SOUND; i++)
	{
		char fileName[70];
		sprintf_s(fileName, sizeof(fileName), "Assets/Sounds/%s/%s.wav", fileSoundName[4], soundName[i]);
		soundStruct.soundBuffer[i] = sfSoundBuffer_createFromFile(fileName);
	}

}


void LoadAllSound(void)
{
	//load buffer
	LoadBufferSound();


	//init list allSound
	soundStruct.allSound = calloc(1, sizeof(sfSound*));

}

void UpdateSound(float _dt)
{
	if (soundStruct.numberOfPlayedSound != 0)
	{
		SetSoundVolumeOnDistance();
		DestroySound();
	}

	if (GetGameState() != LAUNCHING_GAME || GetGameState() != MENU)
	{
		RandomAmbientSound(_dt);
	}
}



void CleanupSound(void)
{
	for (int i = 0; i < soundStruct.numberOfPlayedSound; i++)
	{
		sfSound_destroy(soundStruct.allSound[i]);
	}
	if (soundStruct.allSound != NULL)
	{
		free(soundStruct.allSound);
	}
	float temp = soundStruct.soundVolume;
	soundStruct = (SoundStruct){ 0 };
	soundStruct.soundVolume = temp;
}

void DestroyAllSound(void)
{
	for (int i = 0; i < soundStruct.numberOfPlayedSound; i++)
	{
		sfSound_destroy(soundStruct.allSound[i]);
		soundStruct.allSound[i] = NULL;
		SortSoundList(i);
		soundStruct.numberOfPlayedSound--;

		if (soundStruct.numberOfPlayedSound > 0)
		{
			ReallocStructSound(0);
		}
	}
}

float GetSoundsVolume(void)
{
	return soundStruct.soundVolume;
}

void AddToSoundsVolume(float _value)
{
	if (soundStruct.soundVolume + _value >= 100.0f)
	{
		soundStruct.soundVolume = 100.0f;
	}
	else if (soundStruct.soundVolume + _value <= 0.0f)
	{
		soundStruct.soundVolume = 0.0f;
	}
	else
	{
		soundStruct.soundVolume += _value;
	}
}

void DestroySound(void)
{
	for (int i = 0; i < soundStruct.numberOfPlayedSound; i++)
	{
		//if a sound is not used, enter the loop
		if (sfSound_getStatus(soundStruct.allSound[i]) == sfStopped || sfSound_getStatus(soundStruct.allSound[i]) == sfPaused)
		{
			//destroy and reduce number of sound
			sfSound_destroy(soundStruct.allSound[i]);
			soundStruct.allSound[i] = NULL;
			SortSoundList(i);
			soundStruct.numberOfPlayedSound--;

			//realloc only if nmbr of sound is positive
			if (soundStruct.numberOfPlayedSound > 0)
			{
				ReallocStructSound(0);
			}

		}

	}
	//soundStruct.numberOfPlayedSound = 0;
}

float GetDistancePlayerSound(int _index)
{
	//get the position of the Player
	sfVector2f playerPosition = sfSprite_getPosition(GetPlayer()->sprite);

	//create function to get index position X and Y (convert sfVector3f in sfVector2f)
	sfVector2i indexPosition = { 0, 0 };
	indexPosition.x = (int)sfSound_getPosition(soundStruct.allSound[_index]).x;
	indexPosition.y = (int)sfSound_getPosition(soundStruct.allSound[_index]).y;

	//pythagora

	//get both value for pytha
	int value1 = abs(indexPosition.x - (int)playerPosition.x);
	int value2 = abs(indexPosition.y - (int)playerPosition.y);

	//calculus with square
	double pytha = value1 * value1 + value2 * value2;

	//return square root of float value
	return (float)sqrt(pytha);
}

float GetValueOfVolume(float _value)
{
	//return a float, that will be less than 1 and more than 0.
	//will be close to 0, when the _value is a big number.
	return BASE_VOLUME_CALCUL / (BASE_VOLUME_CALCUL + _value);
}

void SetSoundVolumeOnDistance(void)
{
	//init value of the volume to use it in loop
	float volumeValue = 0.0f;

	//change volume for each sound
	for (int i = 0; i < soundStruct.numberOfPlayedSound; i++)
	{
		//the volume can only be reduce or increase IF the sound is not relative to listener
		//if sound is spatializable
		if (soundStruct.allSound[i] != NULL)
		{
			if (sfSound_isRelativeToListener(soundStruct.allSound[i]))
			{
				volumeValue = GetValueOfVolume(GetDistancePlayerSound(i));
				sfSound_setVolume(soundStruct.allSound[i], volumeValue * soundStruct.soundVolume);
			}
		}

	}
}

void SortSoundList(int _index)
{
	for (int i = _index; i < soundStruct.numberOfPlayedSound - 1; i++)
	{
		soundStruct.allSound[i] = soundStruct.allSound[i + 1];
		soundStruct.allSound[i + 1] = NULL;
	}
}

void PlaySound(SoundEnum _soundType, sfVector2f _soundPosition, sfBool _isLooping, sfBool _isSpacial)
{
	if (soundStruct.soundVolume >= 1)
	{

		ReallocStructSound(1);

		//create sound variable and set buffer in
		soundStruct.allSound[soundStruct.numberOfPlayedSound] = sfSound_create();
		sfSound_setBuffer(soundStruct.allSound[soundStruct.numberOfPlayedSound], soundStruct.soundBuffer[_soundType]);
		if (soundStruct.soundBuffer[_soundType] == NULL)
		{
			printf("buffer NULL\n");
		}

		ModificationOnSound(_soundType);

		if (_isSpacial)
		{
			//init position of the sound at the same position of _soundPosition in argument
			sfSound_setPosition(soundStruct.allSound[soundStruct.numberOfPlayedSound], (sfVector3f) { _soundPosition.x, _soundPosition.y, 0 });

			//change spatialization of the sound
			sfSound_setRelativeToListener(soundStruct.allSound[soundStruct.numberOfPlayedSound], _isSpacial);

		}

		//make sound low in pitch. it affects the playing speed, so so length of the sound.
		if (GetPlayerBulletTime())
		{
			sfSound_setPitch(soundStruct.allSound[soundStruct.numberOfPlayedSound], 1 / BULLET_TIME_SLOW);
		}

		//play the sound, loop it if needed, and change number of playedSound for a duration (until it get destroyed)
		sfSound_play(soundStruct.allSound[soundStruct.numberOfPlayedSound]);
		sfSound_setLoop(soundStruct.allSound[soundStruct.numberOfPlayedSound], _isLooping);
		soundStruct.numberOfPlayedSound++;
	}
}

void StopSound(int _index)
{
	sfSound_stop(soundStruct.allSound[_index]);
}

sfBool ReallocStructSound(int _decalNumber)
{
	sfSound** soundListTemp = NULL;
	//realloc the variable all sound, in a new variable (to not loose all data)

	soundListTemp = realloc(soundStruct.allSound, (soundStruct.numberOfPlayedSound + _decalNumber) * sizeof(sfSound*));



	//protection : if it didn't work stop the function
	if (soundListTemp == NULL)
	{
		printf("NULL realloc didn't worked\n");
		return sfFalse;
	}

	soundStruct.allSound = soundListTemp;
	return sfTrue;
}

void ModificationOnSound(SoundEnum _soundType)
{
	//if they're is modification on the sound to make


}

void RandomizePitch(float _min, int _max)
{
	sfSound_setPitch(soundStruct.allSound[soundStruct.numberOfPlayedSound], (float)(rand() % _max + _min));
}

void RandomAmbientSound(float _dt)
{
	if (GetGameState == GAME)
	{
		if (soundStruct.canPlaySound == sfTrue)
		{
			PlaySound(AMBIENT1 + rand() % 6, (sfVector2f) { 0, 0 }, sfFalse, sfFalse);
			soundStruct.canPlaySound = sfFalse;
			soundStruct.timerAmbient = 0.0f;
		}
		else
		{
			soundStruct.timerAmbient += _dt;

			if (soundStruct.timerAmbient >= DELAY_AMBIENT_SOUND)
			{
				soundStruct.canPlaySound = sfTrue;
				soundStruct.timerAmbient = 0.0f;
			}
		}
	}
}