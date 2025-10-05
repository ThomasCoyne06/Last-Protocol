#include "Text.h"

TextData textData;

void LoadFont(void);
void LoadEveryChar(void);
void SortText(void);
void DrawTextLetterByLetter(sfRenderWindow* _window, int _arrayPos);

void LoadText(void)
{
	LoadFont();

	textData.texture = sfTexture_createFromFile("Assets/Textures/Game/TutoText/TutoScreen.png", NULL);

	LoadEveryChar();

	/*AddText(textData.textList[0], (sfVector2f) { 910, 980 }, (sfVector2f) { 820, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[1], (sfVector2f) { 1330, 980 }, (sfVector2f) { 1280, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[2], (sfVector2f) { 2290, 980 }, (sfVector2f) { 2176, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[3], (sfVector2f) { 3070, 980 }, (sfVector2f) { 2908, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[4], (sfVector2f) { 7180, -220 }, (sfVector2f) { 7120, -430 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[5], (sfVector2f) { 10030, -159 }, (sfVector2f) { 9800, -379 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[6], (sfVector2f) { 16270, 260 }, (sfVector2f) { 16208, 40 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[7], (sfVector2f) { 17410, 20 }, (sfVector2f) { 17252, -197 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[8], (sfVector2f) { 22690, 500 }, (sfVector2f) { 22530, 280 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[9], (sfVector2f) { 23770, 500 }, (sfVector2f) { 23620, 280 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[10], (sfVector2f) { 25450, -280 }, (sfVector2f) { 25310, -497 }, 30, sfColor_fromRGB(75, 228, 0));*/
}

void UpdateText(float _dt)
{
	for (unsigned i = 0; i < TEXT_NB; i++)
	{
		if (textData.text[i].isReading == sfTrue && textData.text[i].charLength >= textData.text[i].drawCharNb)
		{
			textData.text[i].timer += _dt;
			if (textData.text[i].timer > TIME_PER_LETTER)
			{
				textData.text[i].timer -= TIME_PER_LETTER;
				textData.text[i].drawCharNb += 1;
			}
		}
		UpdateAnimation(textData.text[i].currentAnimation, _dt);
	}
}

void DrawText(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < TEXT_NB; i++)
	{
		if (textData.text[i].isReading)
		{
			sfRenderWindow_drawRectangleShape(_window, textData.text[i].textBackground, NULL);
			DrawTextLetterByLetter(_window, i);
		}
		//sfRenderWindow_drawRectangleShape(_window, textData.text[i].hitbox, NULL);
		DrawAnimation(textData.text[i].currentAnimation, _window);

	}
}

void CleanupText()
{
	for (unsigned i = 0; i < TEXT_NB; i++)
	{
		sfText_destroy(textData.text[i].texte);
		textData.text[i].texte = NULL;

		sfSprite_destroy(textData.text[i].sprite);
		textData.text[i].sprite = NULL;
	}

	sfFont_destroy(textData.font01);
	textData.font01 = NULL;
	textData = (TextData){ 0 };

	sfTexture_destroy(textData.texture);
	textData.texture = NULL;
}

void LoadFont(void)
{
	textData.font01 = sfFont_createFromFile("Assets/Fonts/Pixeldu.ttf");
}

void LoadEveryChar(void)
{
	textData.textList[0] = " Welcome to\nthe tutorial";
	textData.textList[1] = "Press 'A'\nto jump";
	textData.textList[2] = "	Maintain 'A'\nto jump higher";
	textData.textList[3] = "Press 'A' while jumping\n 	to double jump";
	textData.textList[4] = "Press 'RB'\n to dash";
	textData.textList[5] = "You can cross long distance\n			using the dash";
	textData.textList[6] = "Press 'RT'\nto slide";
	textData.textList[7] = "	Faster you are,\nlonger is your slide";
	textData.textList[8] = "Press 'LT' to trigger\n	the slow motion";
	textData.textList[9] = " While in slow motion\nyou're more accurate";
	textData.textList[10] = "  Combine actions\nwhile in slow motion";
	textData.textList[11] = "A";
}

void SortText(void)
{
	Text textTemp = { 0 };

	for (int i = TEXT_NB - 1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			sfFloatRect rect1 = GetTextHitbox(j);
			int rect1X = (int)rect1.left;

			sfFloatRect rect2 = GetTextHitbox(j + 1);
			int rect2X = (int)rect2.left;

			if (rect1X > rect2X)
			{
				textTemp = textData.text[j];
				textData.text[j] = textData.text[j + 1];
				textData.text[j + 1] = textTemp;
			}
		}
	}

	for (int i = 0; i < TEXT_NB; i++)
	{
		sfFloatRect rect1 = GetTextHitbox(i);
		int rect1X = (int)rect1.left;
	}
}

void CombineTextBoxWithText(void)
{
	SortText();

	for (unsigned i = 0; i < TEXT_NB; i++)
	{
		//Create the text and associate the font
		textData.text[i].texte = sfText_create();
		textData.text[i].chainChar = textData.textList[i];
		sfText_setFont(textData.text[i].texte, textData.font01);

		//Set all informations for the text
		sfText_setString(textData.text[i].texte, textData.text[i].chainChar);

		sfFloatRect textHitbox = sfRectangleShape_getGlobalBounds(textData.text[i].hitbox);
		sfFloatRect  textSize = sfText_getGlobalBounds(textData.text[i].texte);

		sfVector2f textPos = { textHitbox.left - (textSize.width / 2),textHitbox.top + TEXT_POS_Y };

		sfText_setPosition(textData.text[i].texte, textPos);
		sfText_setCharacterSize(textData.text[i].texte, TEXT_CHAR_SIZE);
		sfText_setColor(textData.text[i].texte, TEXT_COLOR);
		sfText_setOutlineColor(textData.text[i].texte, TEXT_OUTLINE_COLOR);
		//sfText_setOutlineThickness(textData.text[i].texte, 1);

		textSize = sfText_getGlobalBounds(textData.text[i].texte);

		textData.text[i].charLength = (unsigned)strlen(textData.text[i].chainChar);

		//Set the text background
		textData.text[i].textBackground = sfRectangleShape_create();
		sfRectangleShape_setSize(textData.text[i].textBackground, (sfVector2f) { textSize.width + TEXT_BACKGROUND_SIZE_X, textSize.height * 2 });
		sfRectangleShape_setPosition(textData.text[i].textBackground, (sfVector2f) { textPos.x - TEXT_BACKGROUND_SIZE_X / 2, textPos.y - TEXT_BACKGROUND_SIZE_Y });

		sfRectangleShape_setFillColor(textData.text[i].textBackground, TEXT_BACKGROUND_COLOR);
		sfRectangleShape_setOutlineColor(textData.text[i].textBackground, TEXT_BACKGROUND_OUTLINE_COLOR);
		sfRectangleShape_setOutlineThickness(textData.text[i].textBackground, TEXT_BACKGROUND_OUTLINE_THIKNESS);
	}



	//sfText_getGlobalBounds
	/*AddText(textData.textList[0], (sfVector2f) { 910, 980 }, (sfVector2f) { 820, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[1], (sfVector2f) { 1330, 980 }, (sfVector2f) { 1280, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[2], (sfVector2f) { 2290, 980 }, (sfVector2f) { 2176, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[3], (sfVector2f) { 3070, 980 }, (sfVector2f) { 2908, 760 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[4], (sfVector2f) { 7180, -220 }, (sfVector2f) { 7120, -430 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[5], (sfVector2f) { 10030, -159 }, (sfVector2f) { 9800, -379 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[6], (sfVector2f) { 16270, 260 }, (sfVector2f) { 16208, 40 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[7], (sfVector2f) { 17410, 20 }, (sfVector2f) { 17252, -197 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[8], (sfVector2f) { 22690, 500 }, (sfVector2f) { 22530, 280 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[9], (sfVector2f) { 23770, 500 }, (sfVector2f) { 23620, 280 }, 30, sfColor_fromRGB(75, 228, 0));
	AddText(textData.textList[10], (sfVector2f) { 25450, -280 }, (sfVector2f) { 25310, -497 }, 30, sfColor_fromRGB(75, 228, 0));*/
}

void AddText(sfVector2f _textBoxPosition)
{
	int textPos = textData.textNb;

	textData.text[textPos].hitbox = sfRectangleShape_create();
	sfRectangleShape_setSize(textData.text[textPos].hitbox, (sfVector2f) { TEXT_HITBOX_SIZE_X, TEXT_HITBOX_SIZE_Y });
	sfRectangleShape_setPosition(textData.text[textPos].hitbox, (sfVector2f) { _textBoxPosition.x + TEXT_HITBOX_POS_X, _textBoxPosition.y + TEXT_HITBOX_POS_Y });

	textData.text[textPos].sprite = sfSprite_create();
	sfSprite_setTexture(textData.text[textPos].sprite, textData.texture, sfTrue);

	sfSprite_setPosition(textData.text[textPos].sprite, (sfVector2f) { _textBoxPosition.x + TEXT_SPRITE_POS_X, _textBoxPosition.y + TEXT_SPRITE_POS_Y });

	sfSprite_setScale(textData.text[textPos].sprite, (sfVector2f) { SCALE, SCALE });

	textData.text[textPos].idleAnimation = CreateAnimation(textData.text[textPos].sprite, TEXT_FRAME_NB, TEXT_FRAMERATE, (sfIntRect) { 0, 0, TEXT_SPRITE_SIZE_X, TEXT_SPRITE_SIZE_Y }, sfTrue);

	textData.text[textPos].currentAnimation = &textData.text[textPos].idleAnimation;

	textData.textNb++;
}

//void AddText(char* _texte, sfVector2f _textBoxPosition, sfVector2f _textPosition, unsigned _size, sfColor _color)
//{
//	Text* newText = (Text*)calloc(1, sizeof(Text));
//
//	if (newText != NULL)
//	{
//		//Create the text and associate the font
//		newText->texte = sfText_create();
//		newText->chainChar = _texte;
//		sfText_setFont(newText->texte, textData.font01);
//
//		//Set all informations done
//		sfText_setString(newText->texte, newText->chainChar);
//		sfText_setPosition(newText->texte, _textPosition);
//		sfText_setCharacterSize(newText->texte, _size);
//		sfText_setColor(newText->texte, _color);
//		sfText_setOutlineColor(newText->texte, sfBlack);
//		sfText_setOutlineThickness(newText->texte, 1);
//
//		newText->charLength = (unsigned)strlen(_texte);
//
//		newText->hitbox = sfRectangleShape_create();
//		sfRectangleShape_setSize(newText->hitbox, (sfVector2f) { TEXT_HITBOX_SIZE_X, TEXT_HITBOX_SIZE_Y });
//		sfRectangleShape_setPosition(newText->hitbox, (sfVector2f) { _textBoxPosition.x + TEXT_HITBOX_POS_X, _textBoxPosition.y + TEXT_HITBOX_POS_Y });
//
//
//		//Texture Sprite and animation
//		newText->texture = sfTexture_createFromFile("Assets/Textures/Game/TutoText/TutoScreen.png", NULL);
//		newText->sprite = sfSprite_create();
//		sfSprite_setTexture(newText->sprite, newText->texture, sfTrue);
//
//		sfSprite_setPosition(newText->sprite, (sfVector2f) { _textBoxPosition.x + TEXT_SPRITE_POS_X, _textBoxPosition.y + TEXT_SPRITE_POS_Y });
//
//		sfSprite_setScale(newText->sprite, (sfVector2f) { SCALE, SCALE });
//		newText->idleAnimation = CreateAnimation(newText->sprite, TEXT_FRAME_NB, TEXT_FRAMERATE, (sfIntRect) { 0, 0, TEXT_SPRITE_SIZE_X, TEXT_SPRITE_SIZE_Y}, sfTrue);
//
//		newText->currentAnimation = &newText->idleAnimation;
//
//		//Add the element to the list
//		Element* element = CreateElement((void*)newText);
//		InsertElement(textData.list, element, 0);
//	}
//	else
//	{
//		printf("Calloc fail in add Text\n");
//	}
//}

unsigned GetTextNb(void)
{
	return textData.textNb;
}

sfFloatRect GetTextHitbox(unsigned _arrayPos)
{
	return sfRectangleShape_getGlobalBounds(textData.text[_arrayPos].hitbox);
}

sfBool IsReading(unsigned _arrayPos)
{
	return textData.text[_arrayPos].isReading;
}

void SetReading(unsigned _arrayPos, sfBool _isReading)
{
	if (_isReading)
	{
		textData.text[_arrayPos].isReading = sfTrue;
	}
	else
	{
		textData.text[_arrayPos].isReading = sfFalse;
		textData.text[_arrayPos].drawCharNb = 0;
	}
}

void DrawTextLetterByLetter(sfRenderWindow* _window, int _arrayPos)
{
	char drawChar[300] = { 0 };

	memmove(drawChar, textData.text[_arrayPos].chainChar, textData.text[_arrayPos].drawCharNb);

	sfText* drawTexte = sfText_create();
	sfText_setString(drawTexte, drawChar);
	sfText_setFont(drawTexte, textData.font01);
	sfText_setPosition(drawTexte, sfText_getPosition(textData.text[_arrayPos].texte));
	sfText_setCharacterSize(drawTexte, sfText_getCharacterSize(textData.text[_arrayPos].texte));
	sfText_setColor(drawTexte, sfText_getColor(textData.text[_arrayPos].texte));
	sfText_setOutlineColor(drawTexte, sfText_getOutlineColor(textData.text[_arrayPos].texte));
	sfText_setOutlineThickness(drawTexte, sfText_getOutlineThickness(textData.text[_arrayPos].texte));

	sfRenderWindow_drawText(_window, drawTexte, NULL);
}