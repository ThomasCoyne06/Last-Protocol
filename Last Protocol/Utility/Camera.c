#include "Camera.h"

CameraData camera;
void UpdateRandomCamShake(sfRenderWindow* _window, float _dt);


void LoadCamera(void)
{
	camera.view = sfView_create();
	sfFloatRect levelHitbox = GetLevelHitbox();
	sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
	//sfView_setCenter(camera.view, (sfVector2f) { levelHitbox.left + SCREEN_WIDTH / 2, levelHitbox.top + SCREEN_HEIGHT / 2 });
	//sfView_setCenter(camera.view, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfView_setCenter(camera.view, playerPos);
	sfView_setSize(camera.view, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
}

void UpdateCamera(sfRenderWindow* _window, sfVector2f _playerPosition, float _dt)
{
	sfVector2f cameraPos = sfView_getCenter(camera.view);
	float distance = GetDistanceBetweenPoints(_playerPosition, cameraPos);
	float angle = GetAngle(cameraPos, _playerPosition);

	sfFloatRect levelHitbox = GetLevelHitbox();
	sfFloatRect cameraHitbox = GetCameraBounds();

	if (_playerPosition.x > levelHitbox.left + SCREEN_WIDTH / 2 && _playerPosition.x < levelHitbox.left + levelHitbox.width - SCREEN_WIDTH / 2)
	{
		camera.velocity.x = floorf(distance * CAMERA_SPEED * cosf(angle) * _dt);
	}
	else
	{
		if (cameraHitbox.left < levelHitbox.left)
		{
			sfView_move(camera.view, (sfVector2f) { -cameraHitbox.left, 0 });
			camera.velocity.x = 0;

		}
		else if (cameraHitbox.left + cameraHitbox.width > levelHitbox.left + levelHitbox.width)
		{
			sfView_move(camera.view, (sfVector2f) { levelHitbox.left + levelHitbox.width - (cameraHitbox.left + cameraHitbox.width), 0 });
			camera.velocity.x = 0;

		}
	}

	if (_playerPosition.y > levelHitbox.top + SCREEN_HEIGHT / 2 && _playerPosition.y < levelHitbox.top + levelHitbox.height - SCREEN_HEIGHT / 2)
	{
		camera.velocity.y = floorf(distance * CAMERA_SPEED * sinf(angle) * _dt);
	}
	else
	{
		if (cameraHitbox.top < levelHitbox.top)
		{
			sfView_move(camera.view, (sfVector2f) { 0, -cameraHitbox.top });
			camera.velocity.y = 0;
		}
		else if (cameraHitbox.top + cameraHitbox.height > levelHitbox.top + levelHitbox.height)
		{
			sfView_move(camera.view, (sfVector2f) { 0, levelHitbox.top + levelHitbox.height - (cameraHitbox.top + cameraHitbox.height) });
			camera.velocity.y = 0;
		}
	}

	UpdateRandomCamShake(_window, _dt);

	sfView_move(camera.view, camera.velocity);
	sfRenderWindow_setView(_window, camera.view);
}

void DrawCamera(sfRenderWindow* _window)
{
	//sfFloatRect rect = GetCameraBounds();
	//rect = (sfFloatRect){ rect.left + 10, rect.top +10, rect.width-20, rect.height-20};
	//sfRectangleShape* rectangle = CreateRectangle(rect, sfFalse, sfTransparent, 10, sfYellow);

	//sfRenderWindow_drawRectangleShape(_window, rectangle, NULL);
	//sfRectangleShape_destroy(rectangle);
	//rectangle = NULL;
}

void CleanupCamera(sfRenderWindow* _window)
{
	SetDefaultCamera(_window);
	sfView_destroy(camera.view);
	camera = (CameraData){ 0 };
}

void SetDefaultCamera(sfRenderWindow* _window)
{
	sfView* DefaultView = sfRenderWindow_getDefaultView(_window);
	sfRenderWindow_setView(_window, DefaultView);
}

void SetTextCenterCamera(sfRenderWindow* _window, sfText* _text)
{
	//sfFloatRect hitboxText = sfText_getGlobalBounds(_text);
	sfView* currentView = sfRenderWindow_getView(_window);
	sfVector2f midScreen = sfView_getCenter(currentView);
	sfText_setPosition(_text, midScreen);

}

void SetSpriteCenterCamera(sfRenderWindow* _window, sfSprite* _sprite)
{
	//sfFloatRect hitboxSprite = sfSprite_getGlobalBounds(_sprite);
	sfView* currentView = sfRenderWindow_getView(_window);
	sfVector2f midScreen = sfView_getCenter(currentView);
	sfSprite_setPosition(_sprite, midScreen);
}

// first argument is the shooter, the second is the window
float GetAngleWithCamera(sfVector2f _p1, sfRenderWindow* _window)
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_window);
	sfView* view = sfRenderWindow_getView(_window);
	sfVector2f center = sfView_getCenter(view);
	sfVector2f size = sfView_getSize(view);
	sfVector2f offset = { center.x - (size.x / 2),center.y - (size.y / 2) };
	sfVector2f posPlayerOnScreen = { _p1.x - offset.x, _p1.y - offset.y };

	return atan2f(mousePos.y - posPlayerOnScreen.y, mousePos.x - posPlayerOnScreen.x);
}


sfVector2f MousePositionWithCamera(sfRenderWindow* _window)
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_window);
	sfView* view = sfRenderWindow_getView(_window);
	sfVector2f center = sfView_getCenter(view);
	sfVector2f size = sfView_getSize(view);

	sfVector2f offset = { center.x - (size.x / 2) + mousePos.x,center.y - (size.y / 2) + mousePos.y };

	return offset;
}

// Use this in update, the sprite will stay on the screen in the same spot whatever you do
void LinkSpriteToCamera(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position)
{
	if (_sprite != NULL)
	{
	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);

	// met en haut à gauche de l'écran
	sfVector2f offset = { center.x - (size.x / 2),center.y - (size.y / 2) };
	//sfVector2f offset = { 0,0 };

	sfSprite_setPosition(_sprite, (sfVector2f) { offset.x + _position.x, offset.y + _position.y });
}
}
// Use this in update, the sprite will stay on the screen in the same spot whatever you do
void LinkTextToCamera(sfText* _text, sfRenderWindow* _window, sfVector2f _position)
{

	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);

	// met en haut à gauche de l'écran
	sfVector2f offset = { center.x - (size.x / 2),center.y - (size.y / 2) };
	//sfVector2f offset = { 0,0 };

	sfText_setPosition(_text, (sfVector2f) { offset.x + _position.x, offset.y + _position.y });
}

// Use this in update, the sprite will stay on the screen in the same spot whatever you do
void LinkRectangleToCamera(sfRectangleShape* _rect, sfRenderWindow* _window, sfVector2f _position)
{

	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);

	// met en haut à gauche de l'écran
	sfVector2f offset = { center.x - (size.x / 2),center.y - (size.y / 2) };
	//sfVector2f offset = { 0,0 };

	sfRectangleShape_setPosition(_rect, (sfVector2f) { offset.x + _position.x, offset.y + _position.y });
}

void LinkSpriteToCameraVerti(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position)
{
	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);
	sfVector2f position = sfSprite_getPosition(_sprite);

	sfVector2f offset = { center.x - (size.x / 2) - position.x , center.y - (size.y / 2) - position.y };

	sfSprite_move(_sprite, (sfVector2f) { 0, offset.y + _position.y });
}

void LinkSpriteToCameraHoriz(sfSprite* _sprite, sfRenderWindow* _window, sfVector2f _position)
{
	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);
	sfVector2f position = sfSprite_getPosition(_sprite);

	sfVector2f offset = { center.x - (size.x / 2) - position.x , center.y - (size.y / 2) - position.y };

	sfSprite_move(_sprite, (sfVector2f) {offset.x + _position.x , 0});
}

const CameraData* GetCamera(void)
{
	return &camera;
}

CameraData* SetCamera(void)
{
	return &camera;
}

sfFloatRect GetCameraBounds(void)
{
	sfVector2f center = sfView_getCenter(camera.view);
	sfVector2f size = sfView_getSize(camera.view);

	sfFloatRect rect = { center.x - (size.x / 2),center.y - (size.y / 2), size.x, size.y };
	return rect;
}

void SetRandomCamShake(float _radius, float _weakening, float _duration)
{
	camera.camShake.radius = _radius;
	camera.camShake.weakening = _weakening;
	camera.camShake.duration = _duration;
	camera.camShake.isShake = sfTrue;
}

void UpdateRandomCamShake(sfRenderWindow* _window, float _dt)
{
	CamShake* shake = &camera.camShake;
	if (shake->isShake)
	{
		int randAngle = rand() % 360;

		float rad = ConvertDegToRad(randAngle);
		sfVector2f velocityShake = { floorf(shake->radius * cosf(rad)), floorf(shake->radius * sinf(rad)) };

		shake->radius -= shake->weakening * _dt;
		sfView_move(camera.view, velocityShake);

		shake->timer += _dt;
		if (shake->timer >= shake->duration)
		{
			shake->isShake = sfFalse;
			shake->timer = 0;
		}
	}
}