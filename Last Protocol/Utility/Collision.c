#include "Collision.h"

void CollisionSpriteScreen(sfSprite* _sprite)
{
	sfFloatRect spriteHitbox = sfSprite_getGlobalBounds(_sprite);

	if (spriteHitbox.left < 0)
	{
		sfSprite_move(_sprite, (sfVector2f) { -spriteHitbox.left, 0 });
	}
	else if (spriteHitbox.left + spriteHitbox.width > SCREEN_WIDTH)
	{
		sfSprite_move(_sprite, (sfVector2f) { SCREEN_WIDTH - (spriteHitbox.left + spriteHitbox.width), 0 });
	}

	if (spriteHitbox.top < 0)
	{
		sfSprite_move(_sprite, (sfVector2f) { 0, -spriteHitbox.top });
	}
	else if (spriteHitbox.top + spriteHitbox.height > SCREEN_HEIGHT)
	{
		sfSprite_move(_sprite, (sfVector2f) { 0, SCREEN_HEIGHT - (spriteHitbox.top + spriteHitbox.height) });
	}
}


void CollisionSpriteScreenBounce(sfSprite* _sprite, sfVector2f* _velocity)
{
	sfFloatRect spriteHitbox = sfSprite_getGlobalBounds(_sprite);
	if (spriteHitbox.left < 0)
	{
		sfSprite_move(_sprite, (sfVector2f) { -spriteHitbox.left, 0 });
		_velocity->x *= -1;
	}
	if (spriteHitbox.left + spriteHitbox.width > SCREEN_WIDTH)
	{
		sfSprite_move(_sprite, (sfVector2f) { SCREEN_WIDTH - (spriteHitbox.left + spriteHitbox.width), 0 });
		_velocity->x *= -1;
	}
	if (spriteHitbox.top < 0)
	{
		sfSprite_move(_sprite, (sfVector2f) { 0, -spriteHitbox.top });
		_velocity->y *= -1;
	}
	if (spriteHitbox.top + spriteHitbox.height > SCREEN_HEIGHT)
	{
		sfSprite_move(_sprite, (sfVector2f) { 0, SCREEN_HEIGHT - (spriteHitbox.top + spriteHitbox.height) });
		_velocity->y *= -1;
	}
}

sfBool CheckHitboxLeaveScreen(sfFloatRect _hitbox)
{
	if (_hitbox.left + _hitbox.width < 0)
	{
		return sfTrue;
	}
	if (_hitbox.left > SCREEN_WIDTH)
	{
		return sfTrue;
	}
	if (_hitbox.top + _hitbox.height < 0)
	{
		return sfTrue;
	}
	if (_hitbox.top > SCREEN_HEIGHT)
	{
		return sfTrue;
	}
	return sfFalse;
}

sfBool IsColliding2Rectangles(sfFloatRect _rect1, sfFloatRect _rect2)
{
	return _rect2.left + _rect2.width > _rect1.left && _rect2.left < _rect1.left + _rect1.width &&
		_rect2.top + _rect2.height > _rect1.top && _rect2.top < _rect1.top + _rect1.height;
}

sfBool isColliding2Circles(const sfCircleShape* _circle1, const sfCircleShape* _circle2)
{
	sfVector2f circle1Position = sfCircleShape_getPosition(_circle1);
	sfVector2f circle2Position = sfCircleShape_getPosition(_circle2);
	float circle1Radius = sfCircleShape_getRadius(_circle1);
	float circle2Radius = sfCircleShape_getRadius(_circle2);
	float distance = sqrtf((circle1Position.x - circle2Position.x) * (circle1Position.x - circle2Position.x) + (circle1Position.y - circle2Position.y) * (circle1Position.y - circle2Position.y));

	return distance < circle1Radius + circle2Radius;
}

sfBool IsCollidingPointRectangle(sfVector2f _p, sfFloatRect _rect)
{
	return _rect.left + _rect.width > _p.x && _rect.left < _p.x &&
		_rect.top + _rect.height > _p.y && _rect.top < _p.y;
}

sfBool IsColliding2Ellipses(sfFloatRect _hitbox1, sfFloatRect _hitbox)
{
	//p = position
	sfVector2f p1 = { _hitbox1.left + _hitbox1.width / 2, _hitbox1.top + _hitbox1.height / 2 };
	sfVector2f p2 = { _hitbox.left + _hitbox.width / 2, _hitbox.top + _hitbox.height / 2 };

	//r = radius
	sfVector2f r1 = { _hitbox1.width / 2,_hitbox1.height / 2 };
	sfVector2f r2 = { _hitbox.width / 2,_hitbox.height / 2 };

	float angle1 = atan2f(p2.y - p1.y, p2.x - p1.x);
	float radius1 = (r1.x * r1.y) / sqrtf((r1.y * r1.y) * (cosf(angle1) * cosf(angle1)) + (r1.x * r1.y) * (sinf(angle1) * sinf(angle1)));

	float angle2 = atan2f(p1.y - p2.y, p1.x - p2.x);
	float radius2 = (r2.x * r2.y) / sqrtf((r2.y * r2.y) * (cosf(angle2) * cosf(angle2)) + (r2.x * r2.y) * (sinf(angle2) * sinf(angle2)));

	float distance = sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));

	return distance <= radius1 + radius2;
}

CollisionData CollisionRectanglesOffset(sfFloatRect _r1, sfFloatRect _r2)
{
	CollisionData col = { 0 };

	if (IsColliding2Rectangles(_r1, _r2))
	{
		float colLeft = _r2.left + _r2.width - _r1.left;
		float colRight = _r1.left + _r1.width - _r2.left;
		float colUp = _r2.top + _r2.height - _r1.top;
		float colDown = _r1.top + _r1.height - _r2.top;
		float colMin = (float)fmin(colLeft, fmin(colRight, fmin(colUp, colDown)));

		if (colMin == colLeft)
		{
			col.direction = COLLISION_LEFT;
			col.offset = colMin;

		}
		else if (colMin == colRight)
		{
			col.direction = COLLISION_RIGHT;
			col.offset = -colMin;

		}
		else if (colMin == colUp)
		{
			col.direction = COLLISION_UP;
			col.offset = colMin;

		}
		else if (colMin == colDown)
		{
			col.direction = COLLISION_DOWN;
			col.offset = -colMin;

		}
	}

	return col;
}

sfBool CollisionCircleRectangle(sfCircleShape* _circle, sfFloatRect _rect)
{
	sfFloatRect rectangleHb = _rect;
	sfVector2f circlePos = sfCircleShape_getPosition(_circle);
	sfVector2f rectanglePos = { _rect.left + _rect.width / 2, _rect.top + _rect.height / 2 };
	int circleRadius = sfCircleShape_getRadius(_circle);

	sfVector2f distance = { 0 };

	distance.x = abs(circlePos.x - rectanglePos.x);
	distance.y = abs(circlePos.y - rectanglePos.y);
	if (distance.x > (rectangleHb.width / 2 + circleRadius))
	{
		return sfFalse;
	}
	if (distance.y > (rectangleHb.height / 2 + circleRadius))
	{
		return sfFalse;
	}
	if (distance.x <= (rectangleHb.width / 2))
	{
		return sfTrue;
	}
	if (distance.y <= (rectangleHb.height / 2))
	{
		return sfTrue;
	}
	float newDistance = sqrt((distance.x + rectangleHb.width / 2 * distance.x + rectangleHb.width / 2) + (distance.y + rectangleHb.height / 2 * distance.y + rectangleHb.height / 2));

	return (newDistance <= (circleRadius * circleRadius));
}
sfBool CollisionCircleRectangleWeapon(sfCircleShape* _circle, sfRectangleShape* _rectangleShape, sfFloatRect _rectangle)
{
	sfVector2f circlePos = sfCircleShape_getPosition(_circle);
	sfVector2f rectanglePos = sfRectangleShape_getPosition(_rectangleShape);
	int circleRadius = sfCircleShape_getRadius(_circle);

	sfVector2f distance = { 0 };

	distance.x = abs(circlePos.x - rectanglePos.x);
	distance.y = abs(circlePos.y - rectanglePos.y);
	if (distance.x > (_rectangle.width / 2 + circleRadius))
	{
		return sfFalse;
	}
	if (distance.y > (_rectangle.height / 2 + circleRadius))
	{
		return sfFalse;
	}
	if (distance.x <= (_rectangle.width / 2))
	{
		return sfTrue;
	}
	if (distance.y <= (_rectangle.height / 2))
	{
		return sfTrue;
	}
	float newDistance = sqrt((distance.x + _rectangle.width / 2 * distance.x + _rectangle.width / 2) + (distance.y + _rectangle.height / 2 * distance.y + _rectangle.height / 2));

	return (newDistance <= (circleRadius * circleRadius));
}

sfVector2i* MakeCardinalHitbox(sfFloatRect _hitbox, sfFloatRect _firstCellRect)
{
	unsigned nbPoint = (unsigned)ceilf(_hitbox.height / _firstCellRect.width) + 1;

	sfVector2i* cell = calloc(nbPoint, sizeof(CellCoordinates));
	if (cell != NULL)
	{
		for (unsigned i = 0; i < nbPoint; i++)
		{
			sfVector2f detectPoint = { _hitbox.left, _hitbox.top + (_hitbox.height * i / (nbPoint - 1)) };
			CellCoordinates coordinates = GetColumnLineWithPosition(detectPoint, _firstCellRect);
			cell[i] = (sfVector2i){ coordinates.column, coordinates.line };
		}
	}
	return cell;
}