#include "logo.h"

LogoData picture;

void FadeSprite(int _logoToShow, float _dt, float _FadeIn, float _timeToShow, float _FadeOut);

void LoadLogo()
{

	picture.logo[FELLOW_SHEEP].texture = sfTexture_createFromFile("Assets/Textures/Logo/Fellowsheep.png", NULL);
	picture.logo[CREA_JEUX].texture = sfTexture_createFromFile("Assets/Textures/Logo/logo-Creajeux-blc.png", NULL);

	for (int i = 0; i < NB_LOGO; i++)
	{
		//picture.logo[i].sprite = CreateSimpleSprite(picture.logo[i].texture, 5);
		picture.logo[i].sprite = sfSprite_create();
		// CreaJeu logo is too big :(
	
		sfSprite_setTexture(picture.logo[i].sprite, picture.logo[i].texture, sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(picture.logo[i].sprite);
		sfSprite_setOrigin(picture.logo[i].sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		if (i == CREA_JEUX)
		{
			sfSprite_setScale(picture.logo[CREA_JEUX].sprite, (sfVector2f) { SCALING_LOGO_CREAJEUX, SCALING_LOGO_CREAJEUX });
		}
		sfSprite_setPosition(picture.logo[i].sprite, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	}

	picture.timer = 0;


}

void UpdateLogo(float _dt)
{
	picture.timer += _dt;
	float numerator = picture.timer - SCREEN_AT_START;
	
	int logoToShow = (int)((numerator)/DENOMINATOR);
	

	if (picture.timer - SCREEN_AT_START > 0 && logoToShow < NB_LOGO)
	{

		FadeSprite(logoToShow, _dt, FADE_IN, TIME_TO_SHOW, FADE_OUT);
	}
	else if (logoToShow == NB_LOGO)
	{
		// startgame
		LoadMenu();
		CleanUpLogo();
		SetGameState(MENU);

	}
}

void DrawLogo(sfRenderWindow* _window)
{
	float numerator = picture.timer - SCREEN_AT_START;
	int logoToShow = (int)((numerator) / DENOMINATOR);

	
	if ((picture.timer - SCREEN_AT_START > 0) && (logoToShow < NB_LOGO))
	{
		
		sfRenderWindow_drawSprite(_window, picture.logo[logoToShow].sprite, NULL);
	}

}

void CleanUpLogo(void)
{
	for (int i = 0; i < NB_LOGO; i++)
	{
		//picture.logo[i].sprite = CreateSimpleSprite(picture.logo[i].texture, 5);
		sfSprite_destroy(picture.logo[i].sprite);
		picture.logo[i].sprite = NULL;
		
		sfTexture_destroy(picture.logo[i].texture);
		picture.logo[i].texture = NULL;

		

	}
}

void FadeSprite(int _logoToShow, float _dt, float _FadeIn, float _timeToShow, float _FadeOut)
{

	picture.logo[_logoToShow].timer += _dt;



	if (picture.logo[_logoToShow].timer < _FadeIn)
	{
		sfSprite_setColor(picture.logo[_logoToShow].sprite, (sfColor) { 255, 255, 255, 255 * sin(M_PI_2 * picture.logo[_logoToShow].timer / _FadeIn) });
	}
	else if (picture.logo[_logoToShow].timer >= _FadeIn && picture.logo[_logoToShow].timer <= _FadeIn + _timeToShow)
	{
		sfSprite_setColor(picture.logo[_logoToShow].sprite, (sfColor) { 255, 255, 255, 255 });
	}
	else if (picture.logo[_logoToShow].timer >= _FadeIn + _timeToShow && picture.logo[_logoToShow].timer <= _FadeIn + _timeToShow + _FadeOut)
	{
		sfSprite_setColor(picture.logo[_logoToShow].sprite, (sfColor) { 250, 255, 255, 255 - (255 * sin(((picture.logo[_logoToShow].timer - (_FadeIn + _timeToShow)) * M_PI_2) / _FadeOut)) });
	}
	else /*if (_timer >= _FadeIn + _timeToShow + _FadeOut)*/
	{
		sfSprite_setColor(picture.logo[_logoToShow].sprite, (sfColor) { 255, 255, 255, 0 });
	}



}

