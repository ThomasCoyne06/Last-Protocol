#ifndef SIGN_H
#define SIGN_H

#include "../Utility/Common.h"
#include "Level.h"

#define SIGN_NB 14
#define TIME_PER_LETTER 0.05f
#define SIGN_CHAR_SIZE 40
#define SIGN_OUTLINE_THIKNESS 1
#define SIGN_COLOR sfGreen
#define SIGN_OUTLINE_COLOR sfWhite

#define SIGN_HITBOX_SIZE_X 100
#define SIGN_HITBOX_SIZE_Y 20
#define SIGN_HITBOX_POS_X -30
#define SIGN_HITBOX_POS_Y 0

#define SIGN_SPRITE_SIZE_X 20
#define SIGN_SPRITE_SIZE_Y 40
#define SIGN_FRAME_NB 10
#define SIGN_FRAMERATE 5

#define SIGN_SPRITE_POS_X -10
#define SIGN_SPRITE_POS_Y -60

#define SIGN_POS_Y -280

#define SIGN_BACKGROUND_SIZE_X 20
#define SIGN_BACKGROUND_SIZE_Y 5

#define SIGN_BACKGROUND_COLOR sfBlack
#define SIGN_BACKGROUND_OUTLINE_COLOR sfGreen
#define SIGN_BACKGROUND_OUTLINE_THIKNESS 2


typedef struct
{
	sfText* texte;
	char* chainChar;
	unsigned charLength;

	unsigned drawCharNb;
	float timer;

	sfRectangleShape* hitbox;
	sfBool isReading;

	sfRectangleShape* textBackground;

	sfSprite* sprite;
	AnimationData idleAnimation;
	AnimationData* currentAnimation;
}Sign;

typedef struct
{
	sfFont* font01;
	Sign sign[SIGN_NB];
	char* textList[SIGN_NB];

	sfTexture* texture;
	unsigned int signNb;
}SignData;

void LoadSign(void);
void UpdateSign(float _dt);
void DrawSign(sfRenderWindow* _window);
void DrawSignPedestral(sfRenderWindow* _window);
void CleanupSign(void);

void AddSign(sfVector2f _textBoxPosition);
//void AddSign(char* _texte, sfVector2f _textBoxPosition, sfVector2f _textPosition, unsigned _size, sfColor _color);
unsigned GetSignNb(void);
sfFloatRect GetSignHitbox(unsigned _arrayPos);
sfBool IsReading(unsigned _arrayPos);
void SetReading(unsigned _arrayPos, sfBool _isReading);
void CombineTextBoxWithText(void);

#endif
