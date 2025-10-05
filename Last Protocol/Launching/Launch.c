#include "Launch.h"

sfBool played;

void LoadLaunch()
{
	LoadLogo();
	played = sfFalse;
}

void UpdateLaunch(float _dt)
{
	UpdateLogo(_dt);
	
	if (!played)
	{
		PlaySound(OPEN_GAME, (sfVector2f) { 0, 0 }, sfFalse, sfFalse);
		played = sfTrue;
	}
}


void DrawLaunch(sfRenderWindow* _window)
{
	DrawLogo(_window);
}

void CleanUpLaunch()
{
	CleanUpLogo();
}