#ifndef LOGO_H
#define LOGO_H

#include "../Utility/Common.h"
#include "../GameState/Menu.h"

#define NB_LOGO 2

#pragma region timing


#define SCREEN_SEPARATOR 2
#define SCREEN_AT_START 4

#define FADE_IN 1.5
#define TIME_TO_SHOW 2
#define FADE_OUT 1.5

#define ANIMATION_TIME (FADE_IN+TIME_TO_SHOW+FADE_OUT)

#define SCALING_LOGO_CREAJEUX 0.09




#define DENOMINATOR (SCREEN_SEPARATOR + ANIMATION_TIME)
#pragma endregion timing


typedef enum
{
	FELLOW_SHEEP,
	CREA_JEUX
}LogoName;

typedef struct
{
	sfSprite* sprite;
	sfTexture* texture;
	float timer;


}Logo;

typedef struct
{
	Logo logo[NB_LOGO];
	float timer;

}LogoData;

void LoadLogo(void);
void UpdateLogo(float _dt);
void DrawLogo(sfRenderWindow* _window);
void CleanUpLogo(void);

#endif // !LOGO_H
