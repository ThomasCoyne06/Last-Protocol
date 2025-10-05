#include "Sign.h"

SignData signData;

void LoadFont(void);
void LoadEveryChar(void);
void SortSign(void);
void DrawSignLetterByLetter(sfRenderWindow* _window, int _arrayPos);

void LoadSign(void)
{
	LoadFont();

	signData.texture = sfTexture_createFromFile("Assets/Textures/Game/TutoText/TutoScreen.png", NULL);

	LoadEveryChar();

	EntityData sign = GetLevel()->entityList[SIGN];
	for (unsigned i = 0; i < sign.count; i++)
	{
		AddSign(sign.positionList[i]);
	}
}

void UpdateSign(float _dt)
{
	for (unsigned i = 0; i < signData.signNb; i++)
	{
		if (signData.sign[i].isReading == sfTrue && signData.sign[i].charLength >= signData.sign[i].drawCharNb)
		{
			signData.sign[i].timer += _dt;
			if (signData.sign[i].timer > TIME_PER_LETTER)
			{
				signData.sign[i].timer -= TIME_PER_LETTER;
				signData.sign[i].drawCharNb += 1;
			}
		}
		UpdateAnimation(signData.sign[i].currentAnimation, _dt);
	}
}

void DrawSign(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < signData.signNb; i++)
	{
		if (signData.sign[i].isReading)
		{
			sfRenderWindow_drawRectangleShape(_window, signData.sign[i].textBackground, NULL);
			DrawSignLetterByLetter(_window, i);
		}
	}
}

void DrawSignPedestral(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < signData.signNb; i++)
	{
		DrawAnimation(signData.sign[i].currentAnimation, _window);
	}
}

void CleanupSign()
{
	for (unsigned i = 0; i < signData.signNb; i++)
	{
		sfText_destroy(signData.sign[i].texte);
		signData.sign[i].texte = NULL;

		sfSprite_destroy(signData.sign[i].sprite);
		signData.sign[i].sprite = NULL;
	}

	sfFont_destroy(signData.font01);
	signData.font01 = NULL;

	sfTexture_destroy(signData.texture);
	signData.texture = NULL;

	signData = (SignData){ 0 };

}

void LoadFont(void)
{
	signData.font01 = sfFont_createFromFile("Assets/Fonts/Pixeldu.ttf");
}

void LoadEveryChar(void)
{
	signData.textList[0] = " Welcome to\nthe tutorial";
	signData.textList[1] = "Press 'A' or' Space key'\n 		  to jump";
	signData.textList[2] = "Maintain 'A' or 'Space key' \n		to jump higher";
	signData.textList[3] = "	Press 'A' or 'Space key' \nwhile jumping to double jump";
	signData.textList[4] = "	Press 'X' or 'K key'\nto slash with the sworD";
	signData.textList[5] = "Press 'RB' or 'LShift key'\n			to dash";
	signData.textList[6] = "You can cross a long distance\n			using the dash";
	signData.textList[7] = "Press 'Rt' or 'S key'\n	    to slide";
	signData.textList[8] = "   the faster you are,\nthe longer your slide is";
	signData.textList[9] = "Press 'Lt' or 'M key' to trigger\n	  	the slow motion";
	signData.textList[10] = " While in slow motion\nyou're more accurate";
	signData.textList[11] = "   Combine actions\nwhile in slow motion";
	signData.textList[12] = "Press 'Rt' or 'L key'\n to resize weapon";
	signData.textList[13] = "	You can resize\nyour weapon several times";

}

void SortSign(void)
{
	Sign textTemp = { 0 };

	for (int i = signData.signNb - 1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			sfFloatRect rect1 = GetSignHitbox(j);
			int rect1X = (int)rect1.left;

			sfFloatRect rect2 = GetSignHitbox(j + 1);
			int rect2X = (int)rect2.left;

			if (rect1X > rect2X)
			{
				textTemp = signData.sign[j];
				signData.sign[j] = signData.sign[j + 1];
				signData.sign[j + 1] = textTemp;
			}
		}
	}

	for (int i = 0; i < signData.signNb; i++)
	{
		sfFloatRect rect1 = GetSignHitbox(i);
		int rect1X = (int)rect1.left;
	}
}

void CombineTextBoxWithText(void)
{
	SortSign();

	for (unsigned i = 0; i < signData.signNb; i++)
	{
		//Create the text and associate the font
		signData.sign[i].texte = sfText_create();
		signData.sign[i].chainChar = signData.textList[i];
		sfText_setFont(signData.sign[i].texte, signData.font01);

		//Set all informations for the text
		sfText_setString(signData.sign[i].texte, signData.sign[i].chainChar);

		sfFloatRect textHitbox = sfRectangleShape_getGlobalBounds(signData.sign[i].hitbox);
		sfFloatRect  textSize = sfText_getGlobalBounds(signData.sign[i].texte);

		sfVector2f textPos = { textHitbox.left - (textSize.width / 2),textHitbox.top + SIGN_POS_Y };

		sfText_setPosition(signData.sign[i].texte, textPos);
		sfText_setCharacterSize(signData.sign[i].texte, SIGN_CHAR_SIZE);
		sfText_setColor(signData.sign[i].texte, SIGN_COLOR);
		sfText_setOutlineColor(signData.sign[i].texte, SIGN_OUTLINE_COLOR);
		//sfText_setOutlineThickness(textData.sign[i].texte, 1);

		textSize = sfText_getGlobalBounds(signData.sign[i].texte);

		signData.sign[i].charLength = (unsigned)strlen(signData.sign[i].chainChar);

		//Set the text background
		signData.sign[i].textBackground = sfRectangleShape_create();
		sfRectangleShape_setSize(signData.sign[i].textBackground, (sfVector2f) { textSize.width + SIGN_BACKGROUND_SIZE_X, textSize.height * 2 });
		sfRectangleShape_setPosition(signData.sign[i].textBackground, (sfVector2f) { textPos.x - SIGN_BACKGROUND_SIZE_X / 2, textPos.y - SIGN_BACKGROUND_SIZE_Y });

		sfRectangleShape_setFillColor(signData.sign[i].textBackground, SIGN_BACKGROUND_COLOR);
		sfRectangleShape_setOutlineColor(signData.sign[i].textBackground, SIGN_BACKGROUND_OUTLINE_COLOR);
		sfRectangleShape_setOutlineThickness(signData.sign[i].textBackground, SIGN_BACKGROUND_OUTLINE_THIKNESS);
	}
}

void AddSign(sfVector2f _textBoxPosition)
{
	int textPos = signData.signNb;

	signData.sign[textPos].hitbox = sfRectangleShape_create();
	sfRectangleShape_setSize(signData.sign[textPos].hitbox, (sfVector2f) { SIGN_HITBOX_SIZE_X, SIGN_HITBOX_SIZE_Y });
	sfRectangleShape_setPosition(signData.sign[textPos].hitbox, (sfVector2f) { _textBoxPosition.x + SIGN_HITBOX_POS_X, _textBoxPosition.y + SIGN_HITBOX_POS_Y });

	signData.sign[textPos].sprite = sfSprite_create();
	sfSprite_setTexture(signData.sign[textPos].sprite, signData.texture, sfTrue);

	sfSprite_setPosition(signData.sign[textPos].sprite, (sfVector2f) { _textBoxPosition.x + SIGN_SPRITE_POS_X, _textBoxPosition.y + SIGN_SPRITE_POS_Y });

	sfSprite_setScale(signData.sign[textPos].sprite, (sfVector2f) { SCALE, SCALE });

	signData.sign[textPos].idleAnimation = CreateAnimation(signData.sign[textPos].sprite, SIGN_FRAME_NB, SIGN_FRAMERATE, (sfIntRect) { 0, 0, SIGN_SPRITE_SIZE_X, SIGN_SPRITE_SIZE_Y }, sfTrue);

	signData.sign[textPos].currentAnimation = &signData.sign[textPos].idleAnimation;
	signData.signNb++;
}

unsigned GetSignNb(void)
{
	return signData.signNb;
}

sfFloatRect GetSignHitbox(unsigned _arrayPos)
{
	return sfRectangleShape_getGlobalBounds(signData.sign[_arrayPos].hitbox);
}

sfBool IsReading(unsigned _arrayPos)
{
	return signData.sign[_arrayPos].isReading;
}

void SetReading(unsigned _arrayPos, sfBool _isReading)
{
	if (_isReading)
	{
		signData.sign[_arrayPos].isReading = sfTrue;
	}
	else
	{
		signData.sign[_arrayPos].isReading = sfFalse;
		signData.sign[_arrayPos].drawCharNb = 0;
	}
}

void DrawSignLetterByLetter(sfRenderWindow* _window, int _arrayPos)
{
	char drawChar[300] = { 0 };

	memmove(drawChar, signData.sign[_arrayPos].chainChar, signData.sign[_arrayPos].drawCharNb);

	sfText* drawTexte = sfText_create();
	sfText_setString(drawTexte, drawChar);
	sfText_setFont(drawTexte, signData.font01);
	sfText_setPosition(drawTexte, sfText_getPosition(signData.sign[_arrayPos].texte));
	sfText_setCharacterSize(drawTexte, sfText_getCharacterSize(signData.sign[_arrayPos].texte));
	sfText_setColor(drawTexte, sfText_getColor(signData.sign[_arrayPos].texte));
	sfText_setOutlineColor(drawTexte, sfText_getOutlineColor(signData.sign[_arrayPos].texte));
	sfText_setOutlineThickness(drawTexte, sfText_getOutlineThickness(signData.sign[_arrayPos].texte));

	sfRenderWindow_drawText(_window, drawTexte, NULL);
}