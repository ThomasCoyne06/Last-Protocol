#include "Cutscene.h"

MovieData movie;
sfSprite* characterMoving;
sfTexture* characterMovingText;


void FadeSpriteMovie(int _shotToShow, float _dt, float _FadeIn, float _timeToShow, float _FadeOut);

void LoadShotMovie()
{

	//movie.animationInformation{}
	char buffer[50];
	

	for (int i = 0; i < NB_SHOT_MOVIE+1; i++)
	{
		sprintf_s(buffer, 50 - 1, "Assets/Textures/Cutscene/%d_Sheet.png",i);
		movie.shot[i].texture = sfTexture_createFromFile(buffer, NULL);
		
		
		//movie.shot[i].sprite = CreateSimpleSprite(movie.shot[i].texture, 5);
		movie.shot[i].sprite = sfSprite_create();
		// CreaJeu shot is too big :(

		sfSprite_setTexture(movie.shot[i].sprite, movie.shot[i].texture, sfTrue);
		sfSprite_setScale(movie.shot[i].sprite, (sfVector2f) { SCALE, SCALE });
		
		
		

		sfIntRect firstframe = { 0,0,SCREEN_WIDTH/3,SCREEN_HEIGHT};
		sfBool loop = sfTrue;
		if (i == 4)
		{
			loop = sfFalse;
		}
		
		movie.shot[i].animation =CreateAnimation(movie.shot[i].sprite,3,10,firstframe, loop);
	

		characterMovingText = sfTexture_createFromFile("Assets/Textures/Cutscene/4_Player.png", NULL);
		characterMoving = sfSprite_create();
		sfSprite_setTexture(characterMoving, characterMovingText, sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(characterMoving);
		sfSprite_setOrigin(characterMoving, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 }); 
		sfSprite_setScale(characterMoving, (sfVector2f) { -SCALE, SCALE });
		sfSprite_setPosition(characterMoving, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
		
	}

	
	movie.timer = 0;

	movie.musicCutsceneIntro = sfMusic_createFromFile("Assets/Musics/CutsceneBegin.wav");
	sfMusic_play(movie.musicCutsceneIntro);
}

void UpdateShotMovie(float _dt)
{
	
	movie.timer += _dt;
	float numerator = movie.timer - SCREEN_AT_START_MOVIE;

	int shotToShow = (int)((numerator) / DENOMINATOR);


	if (movie.timer - SCREEN_AT_START_MOVIE > 0 && shotToShow < NB_SHOT_MOVIE)
	{

		FadeSpriteMovie(shotToShow, _dt, FADE_IN_MOVIE, TIME_TO_SHOW_MOVIE, FADE_OUT_MOVIE);
		UpdateAnimation(&movie.shot[shotToShow].animation,_dt);
		if (shotToShow == 4)
		{
			sfSprite_move(characterMoving, (sfVector2f) { ((SCREEN_WIDTH*8) / (ANIMATION_TIME))* _dt, 0 });
			UpdateAnimation(&movie.shot[shotToShow + 1].animation, _dt);
		}
	}
	else if (shotToShow == NB_SHOT_MOVIE || sfKeyboard_isKeyPressed(sfKeyEnter))
	{
		////startgame
		if (GetTransitionState(READY))
		{
			StartTransitionPicture(sfWhite, 2, 10);
		}
		else if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(GAME);
		}
	}
}

void DrawShotMovie(sfRenderWindow* _window)
{
	float numerator = movie.timer - SCREEN_AT_START_MOVIE;


	int shotToShow = (int)((numerator) / DENOMINATOR);
	if ((movie.timer - SCREEN_AT_START_MOVIE > 0) && (shotToShow < NB_SHOT_MOVIE))
	{

		sfRenderWindow_drawSprite(_window, movie.shot[shotToShow].sprite, NULL);
		if (shotToShow == 4)
		{
			
			sfRenderWindow_drawSprite(_window, movie.shot[shotToShow+1].sprite, NULL);
			sfRenderWindow_drawSprite(_window, characterMoving, NULL);
		}
	}

}

void CleanUpShotMovie(void)
{
	sfMusic_stop(movie.musicCutsceneIntro);
	for (int i = 0; i < NB_SHOT_MOVIE+1; i++)
	{
		//movie.shot[i].sprite = CreateSimpleSprite(movie.shot[i].texture, 5);
		sfSprite_destroy(movie.shot[i].sprite);
		movie.shot[i].sprite = NULL;

		sfTexture_destroy(movie.shot[i].texture);
		movie.shot[i].texture = NULL;

		
		sfTexture_destroy(characterMovingText);
		characterMovingText = NULL;

		sfSprite_destroy(characterMoving);
		characterMoving= NULL;


	}
	
}

void FadeSpriteMovie(int _shotToShow, float _dt, float _FadeIn, float _timeToShow, float _FadeOut)
{

	movie.shot[_shotToShow].timer += _dt;



	if (movie.shot[_shotToShow].timer < _FadeIn)
	{
		sfSprite_setColor(movie.shot[_shotToShow].sprite, (sfColor) { 255, 255, 255, 255 * sin(M_PI_2 * movie.shot[_shotToShow].timer / _FadeIn) });
	}
	else if (movie.shot[_shotToShow].timer >= _FadeIn && movie.shot[_shotToShow].timer <= _FadeIn + _timeToShow)
	{
		sfSprite_setColor(movie.shot[_shotToShow].sprite, (sfColor) { 255, 255, 255, 255 });
	}
	else if (movie.shot[_shotToShow].timer >= _FadeIn + _timeToShow && movie.shot[_shotToShow].timer <= _FadeIn + _timeToShow + _FadeOut)
	{
		sfSprite_setColor(movie.shot[_shotToShow].sprite, (sfColor) { 250, 255, 255, 255 - (255 * sin(((movie.shot[_shotToShow].timer - (_FadeIn + _timeToShow)) * M_PI_2) / _FadeOut)) });
	}
	else /*if (_timer >= _FadeIn + _timeToShow + _FadeOut)*/
	{
		sfSprite_setColor(movie.shot[_shotToShow].sprite, (sfColor) { 255, 255, 255, 0 });
	}



}

