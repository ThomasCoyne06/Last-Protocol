#ifndef TEXT_H
#define TEXT_H

#include "../Utility/Common.h"

#define TEXT_NB 12
#define TIME_PER_LETTER 0.05f
#define TEXT_CHAR_SIZE 40
#define TEXT_OUTLINE_THIKNESS 1
#define TEXT_COLOR sfGreen
#define TEXT_OUTLINE_COLOR sfWhite

#define TEXT_HITBOX_SIZE_X 60
#define TEXT_HITBOX_SIZE_Y 20
#define TEXT_HITBOX_POS_X -10
#define TEXT_HITBOX_POS_Y 0

#define TEXT_SPRITE_SIZE_X 20
#define TEXT_SPRITE_SIZE_Y 40
#define TEXT_FRAME_NB 10
#define TEXT_FRAMERATE 5

#define TEXT_SPRITE_POS_X -10
#define TEXT_SPRITE_POS_Y -60

#define TEXT_POS_Y -280

#define TEXT_BACKGROUND_SIZE_X 20
#define TEXT_BACKGROUND_SIZE_Y 5

#define TEXT_BACKGROUND_COLOR sfBlack
#define TEXT_BACKGROUND_OUTLINE_COLOR sfGreen
#define TEXT_BACKGROUND_OUTLINE_THIKNESS 2


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
}Text;

typedef struct
{
	sfFont* font01;
	Text text[TEXT_NB];
	char* textList[TEXT_NB];

	sfTexture* texture;
	unsigned int textNb;
}TextData;

void LoadText();
void UpdateText(float _dt);
void DrawText(sfRenderWindow* _window);
void CleanupText();

void AddText(sfVector2f _textBoxPosition);
//void AddText(char* _texte, sfVector2f _textBoxPosition, sfVector2f _textPosition, unsigned _size, sfColor _color);
unsigned GetTextNb(void);
sfFloatRect GetTextHitbox(unsigned _arrayPos);
sfBool IsReading(unsigned _arrayPos);
void SetReading(unsigned _arrayPos, sfBool _isReading);
void CombineTextBoxWithText(void);

#endif
