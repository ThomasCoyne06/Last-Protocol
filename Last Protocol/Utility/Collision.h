#ifndef COLLISION_H
#define COLLISION_H

#include "Common.h"
#include "../Utility/Mathematique.h"


typedef enum
{
	COLLISION_NONE,
	COLLISION_LEFT,
	COLLISION_RIGHT,
	COLLISION_UP,
	COLLISION_DOWN
}CollisionDirection;

typedef enum
{
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST,
	NB_CARDINAL
}CollisionCardinal;

typedef struct
{
	CollisionDirection direction;
	float offset;
	sfBool isColliding;

}CollisionData;

void CollisionSpriteScreen(sfSprite* _sprite);

void CollisionSpriteScreenBounce(sfSprite* _sprite, sfVector2f* _velocity);

sfBool CheckHitboxLeaveScreen(sfFloatRect _hitbox);

sfBool IsColliding2Rectangles(sfFloatRect _rect1, sfFloatRect _rect2);

sfBool isColliding2Circles(const sfCircleShape* _circle1, const sfCircleShape* _circle2);

sfBool IsCollidingPointRectangle(sfVector2f _p, sfFloatRect _rect);

sfBool IsColliding2Ellipses(sfFloatRect _hitbox1, sfFloatRect _hitbox);

CollisionData CollisionRectanglesOffset(sfFloatRect _r1, sfFloatRect _r2);

sfBool CollisionCircleRectangle(sfCircleShape* _circle, sfFloatRect _rect);

sfBool CollisionCircleRectangleWeapon(sfCircleShape* _circle, sfRectangleShape* _rectangleShape, sfFloatRect _rectangle);

sfVector2i* MakeCardinalHitbox(sfFloatRect _hitbox, sfFloatRect _firstCellRect);

#endif

