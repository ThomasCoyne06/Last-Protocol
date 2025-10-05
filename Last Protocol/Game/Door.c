#include "Door.h"

DoorData doorData = { 0 };

void LoadDoor(void)
{
	doorData.doorList = CreateList();
	doorData.texture = sfTexture_createFromFile("Assets/Textures/Game/Door/Door.png", NULL);

	EntityData door = GetLevel()->entityList[DOOR];
	for (unsigned i = 0; i < door.count; i++)
	{
		door.positionList[i].x -= HITBOX_SIZE_X;
		AddDoor(door.positionList[i]);
	}
}

void UpdateDoor(float _dt)
{
	for (unsigned i = 0; i < doorData.doorList->count; i++)
	{
		Element* doorElement = GetElement(doorData.doorList, i);
		Door* door = doorElement->data;
		if (door->isOpen)
		{
			UpdateAnimation(door->currentAnimation, _dt);
		}
	}
}

void DrawDoor(sfRenderWindow* _window)
{
		for (unsigned i = 0; i < doorData.doorList->count; i++)
		{
			Element* doorElement = GetElement(doorData.doorList, i);
			Door* door = doorElement->data;
			DrawAnimation(door->currentAnimation, _window);
			//Draw the Hitbox
			//sfRectangleShape* rect = CreateHitbox(door->crossedHitbox, sfTrue, sfBlue, SCALE);
			//sfRenderWindow_drawRectangleShape(_window, rect, NULL);
			//sfRectangleShape_destroy(rect);
			//rect = NULL;
		}
}

void CleanupDoor(void)
{
	Element* actualElement = doorData.doorList->first;

	while (actualElement != NULL)
	{
		Door* door = actualElement->data;

		//Cleanup the sprite and the texture
		sfSprite_destroy(door->sprite);
		door->sprite = NULL;

		sfRectangleShape_destroy(door->hitbox);
		door->hitbox = NULL;

		//Go to the next element
		actualElement = actualElement->next;
	}

	sfTexture_destroy(doorData.texture);
	doorData.texture = NULL;

	DeleteList(doorData.doorList);
	doorData = (DoorData){ NULL };
}

void AddDoor(sfVector2f _position)
{
	Door* newDoor = (Door*)calloc(1, sizeof(Door));
	//Initiate the new Pickup
	if (newDoor != NULL)
	{

		sfIntRect rect = { 0, 0, DOOR_SPRITE_WIDTH, DOOR_SPRITE_HEIGHT };
		newDoor->sprite = CreateSpriteFromSpriteSheet(doorData.texture, rect, sfFalse, SCALE);

		newDoor->idleAnimation = CreateAnimation(newDoor->sprite, DOOR_IDLE_FRAMES, DOOR_FRAMERATE, rect, sfFalse);
		newDoor->currentAnimation = &newDoor->idleAnimation;

		//Set the sprite position
		sfSprite_setPosition(newDoor->sprite, _position);
		sfSprite_setScale(newDoor->sprite, (sfVector2f) { SCALE, SCALE });

		//Set the hitbox
		sfFloatRect hitboxRect = { _position.x, _position.y, HITBOX_SIZE_X, HITBOX_SIZE_Y };
		newDoor->hitbox = CreateHitbox(hitboxRect, sfFalse,  sfRed, SCALE);

		newDoor->crossedHitbox = (sfFloatRect) { _position.x + HITBOX_SIZE_X + CROSSED_SIZE_X, _position.y, CROSSED_SIZE_X, HITBOX_SIZE_Y };


		//newDoor->hitbox = sfRectangleShape_create();
		//sfRectangleShape_setSize(newDoor->hitbox, (sfVector2f) { HITBOX_SIZE_X, HITBOX_SIZE_Y });
		//sfRectangleShape_setPosition(newDoor->hitbox, (sfVector2f) { _position.x, _position.y });
		//sfRectangleShape_setOutlineColor(newDoor->hitbox, sfRed);
		//sfRectangleShape_setOutlineThickness(newDoor->hitbox, 1);
		//sfRectangleShape_setFillColor(newDoor->hitbox, sfColor_fromRGBA(0, 0, 0, 0));

		//Add the new pickup to the pickupList
		Element* doorElement = CreateElement((void*)newDoor);
		InsertElement(doorData.doorList, doorElement, 0);

	}
	else
	{
		printf("Fail Calloc in Add Pickup\n");
	}
}

unsigned GetDoorNb(void)
{
	return doorData.doorList->count;
}

sfFloatRect GetDoorHitbox(unsigned _arrayPos)
{
	Element* element = GetElement(doorData.doorList, _arrayPos);
	Door* door = element->data;

	return sfRectangleShape_getGlobalBounds(door->hitbox);
}

sfFloatRect GetDoorCrossedHitbox(unsigned _arrayPos)
{
	Element* element = GetElement(doorData.doorList, _arrayPos);
	Door* door = element->data;

	return door->crossedHitbox;
}

void DoorIsOpen(void)
{
	for (unsigned i = 0; i < doorData.doorList->count; i++)
	{
		Element* doorElement = GetElement(doorData.doorList, i);
		Door* door = doorElement->data;
		door->isOpen = sfTrue;
	}
}

void DoorIsCrossed(void)
{

}
