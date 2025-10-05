#include "Creation.h"

sfRectangleShape* CreateRectangle(sfFloatRect _rect, sfBool _centerOrigin, sfColor _color, float _lineSize, sfColor _lineColor)
{
	sfRectangleShape* shape = sfRectangleShape_create();

	sfVector2f position = { _rect.left, _rect.top };
	sfRectangleShape_setPosition(shape, position);

	sfVector2f size = { _rect.width, _rect.height };
	sfRectangleShape_setSize(shape, size);

	if (_centerOrigin)
	{
		sfVector2f origin = { _rect.width / 2, _rect.height / 2 };
		sfRectangleShape_setOrigin(shape, origin);
	}

	sfRectangleShape_setFillColor(shape, _color);

	sfRectangleShape_setOutlineThickness(shape, _lineSize);
	sfRectangleShape_setOutlineColor(shape, _lineColor);

	return shape;
}

sfCircleShape* CreateCircle(sfVector2f _position, float _r, sfBool _centerOrigin, sfColor _color, float _lineSize, sfColor _lineColor)
{
	sfCircleShape* shape = sfCircleShape_create();

	sfCircleShape_setPosition(shape, _position);

	sfCircleShape_setRadius(shape, _r);

	if (_centerOrigin)
	{
		sfCircleShape_setOrigin(shape, (sfVector2f) { _r, _r });
	}

	sfCircleShape_setFillColor(shape, _color);

	sfCircleShape_setOutlineThickness(shape, _lineSize);
	sfCircleShape_setOutlineColor(shape, _lineColor);

	return shape;
}

sfVertexArray* CreateLine(float _aX, float _aY, float _bX, float _bY, sfColor _color)
{
	sfVertexArray* newLine = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(newLine, sfLines);

	sfVertex pA = { _aX, _aY, _color };
	sfVertexArray_append(newLine, pA);

	sfVertex pB = { _bX, _bY, _color };
	sfVertexArray_append(newLine, pB);

	return newLine;
}

sfSprite* CreateSimpleSprite(sfTexture* _texture, float _scale)
{
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, _texture, sfTrue);
	sfSprite_scale(sprite, (sfVector2f) { _scale, _scale });
	return sprite;
}

sfSprite* CreateSprite(sfTexture* _texture, sfVector2f _position, sfBool _centerOrigin, float _scale)
{
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, _texture, sfTrue);

	sfSprite_setPosition(sprite, _position);

	if (_centerOrigin)
	{
		sfFloatRect rect = sfSprite_getGlobalBounds(sprite);
		sfSprite_setOrigin(sprite, (sfVector2f) { rect.width / 2, rect.height / 2 });
	}

	sfSprite_scale(sprite, (sfVector2f) { _scale, _scale });

	return sprite;
}

sfSprite* CreateSpriteFromSpriteSheet(sfTexture* _texture, sfIntRect _frameRect, sfBool _centerOrigin, float _scale)
{
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, _texture, sfTrue);

	sfSprite_setTextureRect(sprite, _frameRect);

	if (_centerOrigin)
	{
		sfFloatRect spriteBounds = sfSprite_getLocalBounds(sprite);
		sfSprite_setOrigin(sprite, (sfVector2f) { spriteBounds.width / 2, spriteBounds.height / 2 });
	}

	sfSprite_scale(sprite, (sfVector2f) { _scale, _scale });

	return sprite;
}

sfRectangleShape* CreateHitbox(sfFloatRect _rect, sfBool _centerOrigin, sfColor _lineColor, float _scale)
{
	sfRectangleShape* shape = sfRectangleShape_create();

	sfVector2f position = { _rect.left, _rect.top };
	sfRectangleShape_setPosition(shape, position);

	sfVector2f size = { _rect.width, _rect.height };
	sfRectangleShape_setSize(shape, size);

	if (_centerOrigin)
	{
		sfVector2f origin = { _rect.width / 2, _rect.height / 2 };
		sfRectangleShape_setOrigin(shape, origin);
	}

	sfRectangleShape_setFillColor(shape, sfTransparent);

	sfVector2f scale = { _scale, _scale };
	sfRectangleShape_setScale(shape, scale);

	sfRectangleShape_setOutlineThickness(shape, -HITBOX_THICKNESS);
	sfRectangleShape_setOutlineColor(shape, _lineColor);

	return shape;
}

sfText* CreateText(sfFont* _font, float _x, float _y, int _charaSize, sfColor _color, sfBool _centerOrigin, char _buffer[])
{
	sfText* text = sfText_create();

	sfText_setFont(text, _font);

	sfText_setPosition(text, (sfVector2f) { _x, _y });

	sfText_setCharacterSize(text, _charaSize);

	sfText_setColor(text, _color);

	sfText_setString(text, _buffer);

	if (_centerOrigin)
	{
		sfFloatRect rect = sfText_getGlobalBounds(text);
		sfText_setOrigin(text, (sfVector2f) { rect.width / 2, rect.height / 2 });
	}

	return text;
}

sfMusic* CreateMusic(sfBool _isLooping,float _volume, float _speed, char _file[])
{
	sfMusic* music = sfMusic_createFromFile(_file);
	//Loop
	sfMusic_setLoop(music, _isLooping);
	//Volume
	sfMusic_setVolume(music, _volume);
	//Speed
	sfMusic_setPitch(music, _speed);

	sfMusic_play(music);

	return music;
}

sfSound* CreateSound(sfSoundBuffer* _buffer, sfBool _isLooping, float _volume, float _speed)
{
	sfSound* sound = sfSound_create();
	sfSound_setBuffer(sound, _buffer);

	sfSound_setLoop(sound, _isLooping);
	sfSound_setVolume(sound, _volume);
	sfSound_setPitch(sound, _speed);

	return sound;
}

