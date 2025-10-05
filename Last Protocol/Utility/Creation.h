#ifndef CREATE_H
#define CREATE_H

#include "Common.h"

sfRectangleShape* CreateRectangle(sfFloatRect _rect, sfBool _centerOrigin, sfColor _color, float _lineSize, sfColor _lineColor);

sfCircleShape* CreateCircle(sfVector2f _position, float _r, sfBool _centerOrigin, sfColor _color, float _lineSize, sfColor _lineColor);

sfVertexArray* CreateLine(float _aX, float _aY, float _bX, float _bY, sfColor _color);

sfSprite* CreateSimpleSprite(sfTexture* _texture, float _scale);

sfSprite* CreateSprite(sfTexture* _texture, sfVector2f _position, sfBool _centerOrigin, float _scale);

sfSprite* CreateSpriteFromSpriteSheet(sfTexture* _texture, sfIntRect _frameRect, sfBool _centerOrigin, float _scale);

sfRectangleShape* CreateHitbox(sfFloatRect _rect, sfBool _centerOrigin, sfColor _lineColor, float _scale);

sfText* CreateText(sfFont* _font, float _x, float _y, int _charaSize, sfColor _color, sfBool _centerOrigin, char _buffer[]);

sfMusic* CreateMusic(sfBool _isLooping, float _volume, float _speed, char _file[]);

sfSound* CreateSound(sfSoundBuffer* _buffer, sfBool _isLooping, float _volume, float _speed);


#endif


