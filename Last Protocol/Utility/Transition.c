#include "Transition.h"

TransitionData transition;

void TransitionSettings(TransitionType _type, sfColor _color, float _speed);

void UpdateTransitionFade(float _dt);
void UpdateTransitionSlip(float _dt);
void UpdateTransitionIris(float _dt);

void TransitionSettings(TransitionType _type, sfColor _color, float _speed)
{
	transition.type = _type;
	transition.color = _color;
	transition.speed = _speed;
	transition.isDrawed = sfTrue;
	transition.state = STARTED;
}

void StartTransitionPicture(sfColor _color, float _coverScreenDuration, float _framerate)
{
	TransitionSettings(PICTURE, _color, _coverScreenDuration);

	transition.texture = sfTexture_createFromFile("Assets/Textures/Transition/Transition.png", NULL);
	transition.sprite = CreateSimpleSprite(transition.texture, SCALE);
	sfSprite_setColor(transition.sprite, _color);

	sfIntRect firstFrame = { 0, 0, 640, 360 };
	LoadAnimation(&transition.animation);
	transition.animation = CreateAnimation(transition.sprite, TRANSITION_NB_FRAME, _framerate, firstFrame, sfFalse);
}
void UpdateTransitionPicture(sfRenderWindow* _window, float _dt)
{
	if (GetCamera()->view != NULL)
	{
		LinkSpriteToCamera(transition.animation.sprite, _window, (sfVector2f) { 0, 0 });
	}

	if (transition.state == STARTED)
	{
		UpdateAnimation(&transition.animation, _dt);

		if (AnimationIsFinished(&transition.animation))
		{
			transition.state = COVER_SCREEN;
			transition.animation.isFinished = sfFalse;
		}
	}
	else if (COVER_SCREEN)
	{
		//if (GetCamera()->view != NULL)
		//{
		//	LinkSpriteToCamera(transition.animation.sprite, _window, (sfVector2f) { 0, 0 });
		//}
		//sfVector2f pos = sfSprite_getPosition(transition.animation.sprite);
		// 
		//else
		//{
		//	SetDefaultCamera(_window);
		//}
		if (GetCamera()->view != NULL)
		{
			LinkSpriteToCamera(transition.animation.sprite, _window, (sfVector2f) { 0, 0 });
		}

		if (GetGameState() != GAME)
		{
			sfSprite_setPosition(transition.animation.sprite, (sfVector2f) { 0, 0 });
		}

		transition.alpha += _dt;
		if (transition.alpha > transition.speed)
		{
			UpdateReverseAnimation(&transition.animation, _dt);

			if (AnimationIsFinished(&transition.animation))
			{
				CleanupTransition();
				return;
			}
		}
	}

}

void StartTransitionFade(sfColor _color, float _speed)
{
	TransitionSettings(FADE, _color, _speed);

	transition.alpha = 0;
	transition.color.a = (sfUint8)transition.alpha;

	sfFloatRect rect = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	transition.rectangle = CreateRectangle(rect, sfFalse, transition.color, 0, sfTransparent);
}
void UpdateTransitionFade(float _dt)
{
	if (transition.state == STARTED)
	{
		transition.alpha += transition.speed;
		if (transition.alpha >= 255)
		{
			transition.alpha = 255;
			transition.state = COVER_SCREEN;
		}
	}
	else
	{
		transition.alpha -= transition.speed;
		if (transition.alpha <= 0)
		{
			CleanupTransition();
			return;
		}
	}
	transition.color.a = (sfUint8)transition.alpha;
	sfRectangleShape_setFillColor(transition.rectangle, transition.color);
}

void StartTransitionSlip(sfColor _color, float _speed, float _directionDeg)
{
	TransitionSettings(SLIP, _color, _speed);
	transition.direction = ConvertDegToRad(_directionDeg);

	sfFloatRect screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	sfVector2f origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	sfVector2f point = origin;

	while (IsCollidingPointRectangle(point, screenRect))
	{
		transition.distance++;
		point = GetPointWithAngleAndDistance(origin, transition.distance, transition.direction);
	}

	transition.startPoint.x = point.x + transition.distance * cosf(transition.direction);
	transition.startPoint.y = point.y + transition.distance * sinf(transition.direction);

	sfFloatRect rect = { transition.startPoint.x,transition.startPoint.y,SCREEN_WIDTH, SCREEN_HEIGHT };
	transition.rectangle = CreateRectangle(rect, sfTrue, transition.color, 0, sfTransparent);
}
void UpdateTransitionSlip(float _dt)
{
	sfVector2f posititon = sfRectangleShape_getPosition(transition.rectangle);
	float distance = GetDistanceBetweenPoints(transition.startPoint, posititon);

	if (transition.state == STARTED)
	{
		distance >= transition.distance * 2 ? transition.state = COVER_SCREEN : STARTED;
	}
	else
	{
		if (distance >= transition.distance * 4)
		{
			CleanupTransition();
			return;
		}
	}

	sfVector2f move;
	move.x = -transition.speed * cosf(transition.direction) * _dt;
	move.y = -transition.speed * sinf(transition.direction) * _dt;
	sfRectangleShape_move(transition.rectangle, move);
}

void StartTransitionIris(sfVector2f _posititon, sfColor _color, float _speed)
{
	TransitionSettings(IRIS, _color, _speed);

	transition.circle = CreateCircle(_posititon, 0, sfTrue, transition.color, 0, sfTransparent);

	sfVector2f screenCorner[4];
	screenCorner[0] = (sfVector2f){ 0,0 };
	screenCorner[1] = (sfVector2f){ 0,SCREEN_HEIGHT };
	screenCorner[2] = (sfVector2f){ SCREEN_WIDTH,SCREEN_HEIGHT };
	screenCorner[3] = (sfVector2f){ SCREEN_WIDTH,0 };

	float temp = 0.f;
	sfVector2f posititon = sfCircleShape_getPosition(transition.circle);

	for (unsigned i = 0; i < 4; i++)
	{
		temp = GetDistanceBetweenPoints(posititon, screenCorner[i]);
		transition.distance < temp ? transition.distance = temp : transition.distance;
	}
}
void UpdateTransitionIris(float _dt)
{
	if (transition.state == STARTED)
	{
		transition.radius += transition.speed;
		transition.radius >= transition.distance ? transition.state = COVER_SCREEN : STARTED;
	}
	else
	{
		transition.radius -= transition.speed;
		if (transition.radius <= 0)
		{
			CleanupTransition();
			return;
		}
	}
	sfCircleShape_setOrigin(transition.circle, (sfVector2f) { transition.radius, transition.radius });
	sfCircleShape_setRadius(transition.circle, transition.radius);
}

void UpdateTransition(sfRenderWindow* _window, float _dt)
{
	if (transition.isDrawed)
	{
		switch (transition.type)
		{
		case PICTURE:
			UpdateTransitionPicture(_window, _dt);
			break;
		case FADE:
			UpdateTransitionFade(_dt);
			break;
		case SLIP:
			UpdateTransitionSlip(_dt);
			break;
		case IRIS:
			UpdateTransitionIris(_dt);
			break;
		}
	}
}

void DrawTransition(sfRenderWindow* _window)
{
	if (transition.isDrawed)
	{
		switch (transition.type)
		{
		case PICTURE:
		{
			DrawAnimation(&transition.animation, _window);
			break;
		}
		case FADE:
			sfRenderWindow_drawRectangleShape(_window, transition.rectangle, NULL);
			break;
		case SLIP:
			sfRenderWindow_drawRectangleShape(_window, transition.rectangle, NULL);
			break;
		case IRIS:
			sfRenderWindow_drawCircleShape(_window, transition.circle, NULL);
			break;
		}
	}
}

void CleanupTransition(void)
{
	if (transition.isDrawed)
	{
		switch (transition.type)
		{
		case PICTURE:
			sfTexture_destroy(transition.texture);
			sfSprite_destroy(transition.sprite);

			break;
		case FADE:
			sfRectangleShape_destroy(transition.rectangle);

			break;
		case SLIP:
			sfRectangleShape_destroy(transition.rectangle);

			break;
		case IRIS:
			sfCircleShape_destroy(transition.circle);

			break;
		}
	}
	transition = (TransitionData){ 0 };
}

sfBool GetTransitionState(short _state)
{
	return transition.state == _state;
}

//StartTransitionFade(sfBlack, 5);
//StartTransitionSlip(sfBlack, 2000, 45);
//StartTransitionIris(menu.mousePosition, sfBlack, 30);

// .-. .-. .-.
// |.. |-   |
// `-' `-'  '

const TransitionData* GetTransition(void)
{
	return &transition;
}

// .-. .-. .-.
// `-. |-   |
// `-' `-'  '

TransitionData* SetTransition(void)
{
	return &transition;
}