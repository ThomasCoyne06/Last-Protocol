#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "../Game/Level.h"
#include "../Game/Player.h"

#define CAMERA_SPEED 5

typedef struct
{
	float radius;
	float weakening;
	float duration;
	float timer;
	sfBool isShake;
}CamShake;

typedef struct
{
	sfView* view;
	sfVector2f velocity;
	CamShake camShake;
}CameraData;

void LoadCamera(void);

void UpdateCamera(sfRenderWindow* _window, sfVector2f _playerPosition, float _dt);

void DrawCamera(sfRenderWindow* _window);

void CleanupCamera(sfRenderWindow* _window);

void SetDefaultCamera(sfRenderWindow* _window);

// the origin MUST be at the center of the text 
void SetTextCenterCamera(sfRenderWindow* _window, sfText* _text);

// the origin MUST be at the center of the text sprite
void SetSpriteCenterCamera(sfRenderWindow* _window, sfSprite* _sprite);

float GetAngleWithCamera(sfVector2f _p1, sfRenderWindow* _window);

sfVector2f MousePositionWithCamera(sfRenderWindow* _window);

void LinkSpriteToCamera(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position);

void LinkSpriteToCameraVerti(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position);

void LinkSpriteToCameraHoriz(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position);

const CameraData* GetCamera(void);

CameraData* SetCamera(void);

sfFloatRect GetCameraBounds(void);

void SetRandomCamShake(float _radius, float _weakening, float _duration);

#endif // 
