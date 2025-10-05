#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "../Utility/Common.h"
#include "../GameState/Menu.h"

#define NB_SHOT_MOVIE 5

#pragma region timing


#define SCREEN_SEPARATOR_MOVIE 0
#define SCREEN_AT_START_MOVIE 1

#define FADE_IN_MOVIE 0.2
#define TIME_TO_SHOW_MOVIE 3
#define FADE_OUT_MOVIE 0

#define ANIMATION_TIME (FADE_IN_MOVIE+TIME_TO_SHOW_MOVIE+FADE_OUT_MOVIE)

#define DENOMINATOR (SCREEN_SEPARATOR_MOVIE + ANIMATION_TIME)

#pragma endregion timing



typedef struct
{
	sfSprite* sprite;
	sfTexture* texture;
	AnimationData animation;
	float timer;


}ShotMovie;

typedef struct
{
	ShotMovie shot[NB_SHOT_MOVIE + 1];
	//int animationInformation[NB_SHOT_MOVIE][2];
	float timer;
	sfMusic* musicCutsceneIntro;

}MovieData;

void LoadShotMovie(void);
void UpdateShotMovie(float _dt);
void DrawShotMovie(sfRenderWindow* _window);
void CleanUpShotMovie(void);

#endif