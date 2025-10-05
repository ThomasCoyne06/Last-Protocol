#ifndef TRANSITION_H
#define TRANSITION_H

#include "Common.h"
#include "Camera.h"

#define TRANSITION_NB_FRAME 12
#define TRANSITION_FRAME_RATE 10

typedef enum
{
	PICTURE,
	FADE,
	SLIP,
	IRIS,
	//CROSSFADE
}TransitionType;

typedef enum
{
	READY,
	STARTED,
	COVER_SCREEN

}TransitionState;

typedef struct
{
	sfTexture* texture;
	sfSprite* sprite;
	AnimationData animation;

	TransitionType type;
	sfColor color;
	float speed;

	sfRectangleShape* rectangle;
	sfCircleShape* circle;
	sfBool isDrawed;

	float alpha;
	float direction;
	sfVector2f startPoint;
	short state;
	float distance;
	float radius;

}TransitionData;

void StartTransitionPicture(sfColor _color, float _coverScreenDuration, float _framerate);

void StartTransitionFade(sfColor _color, float _speed);

void StartTransitionSlip(sfColor _color, float _speed, float _directionDeg);

void StartTransitionIris(sfVector2f _posititon, sfColor _color, float _speed);

void UpdateTransition(sfRenderWindow* _window, float _dt);

void DrawTransition(sfRenderWindow* _window);

void CleanupTransition(void);

sfBool GetTransitionState(short _state);

const TransitionData* GetTransition(void);

TransitionData* SetTransition(void);

#endif

