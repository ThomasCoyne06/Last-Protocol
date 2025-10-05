#include "Laser.h"

LaserData laserData = { 0 };

void LoadLaser(void)
{
	laserData.laserList = CreateList();
	laserData.textureHorizontal = sfTexture_createFromFile("Assets/Textures/Game/Laser/LaserHorizontal.png", NULL);
	laserData.textureVertical = sfTexture_createFromFile("Assets/Textures/Game/Laser/LaserVertical.png", NULL);
	laserData.timer = LASER_TIMER_ON;

	EntityData laser = GetLevel()->entityList[LASER_VERTICAL];
	for (unsigned i = 0; i < laser.count; i++)
	{
		AddLaser(LASER_TYPE_VERTICAL, laser.positionList[i]);
	}

	laser = GetLevel()->entityList[LASER_HORIZONTAL];
	for (unsigned i = 0; i < laser.count; i++)
	{
		AddLaser(LASER_TYPE_HORIZONTAL,laser.positionList[i]);
	}
}


void UpdateLaser(float _dt)
{
	laserData.timer -= _dt;

	if (laserData.timer < 0)
	{
		if (laserData.isOn)
		{
			laserData.isOn = sfFalse;
			laserData.timer += LASER_TIMER_OFF;
		}
		else
		{
			laserData.isOn = sfTrue;
			laserData.timer += LASER_TIMER_ON;
		}
	}

	for (unsigned i = 0; i < laserData.laserList->count; i++)
	{
		Element* laserElement = GetElement(laserData.laserList, i);
		Laser* laser = laserElement->data;
		UpdateAnimation(laser->currentAnimation, _dt);
	}
}

void DrawLaser(sfRenderWindow* _window)
{
	if (laserData.isOn)
	{
		for (unsigned i = 0; i < laserData.laserList->count; i++)
		{
			Element* laserElement = GetElement(laserData.laserList, i);
			Laser* laser = laserElement->data;
			DrawAnimation(laser->currentAnimation, _window);
			//Draw the Hitbox
			//sfRenderWindow_drawRectangleShape(_window, laser->hitbox, NULL);
		}
	}
}

void CleanupLaser(void)
{
	//Cleanup each element of the pickupList
	Element* actualElement = laserData.laserList->first;

	while (actualElement != NULL)
	{
		Laser* laser = actualElement->data;

		//Cleanup the sprite and the texture
		sfSprite_destroy(laser->sprite);
		laser->sprite = NULL;

		sfRectangleShape_destroy(laser->hitbox);
		laser->hitbox = NULL;

		//Go to the next element
		actualElement = actualElement->next;
	}

	sfTexture_destroy(laserData.textureHorizontal);
	laserData.textureHorizontal = NULL;

	sfTexture_destroy(laserData.textureVertical);
	laserData.textureVertical = NULL;

	DeleteList(laserData.laserList);
	laserData = (LaserData){ NULL };
}

void AddLaser(LaserType _laserType, sfVector2f _position)
{
	Laser* newLaser = (Laser*)calloc(1, sizeof(Laser));
	//Initiate the new Pickup
	if (newLaser != NULL)
	{
		switch (_laserType)
		{
		case LASER_TYPE_HORIZONTAL :
			newLaser->sprite = sfSprite_create();
			sfSprite_setTexture(newLaser->sprite, laserData.textureHorizontal, sfTrue);

			newLaser->idleAnimation = CreateAnimation(newLaser->sprite, LASER_IDLE_FRAMES, LASER_FRAMERATE, (sfIntRect) { 0, 0, LASER_HORIZONTAL_WIDTH, LASER_HORIZONTAL_HEIGHT}, sfTrue);
			newLaser->currentAnimation = &newLaser->idleAnimation;

			//Set the sprite position
			sfSprite_setPosition(newLaser->sprite, _position);
			sfSprite_setScale(newLaser->sprite, (sfVector2f) { SCALE, SCALE });

			//Set the hitbox
			newLaser->hitbox = sfRectangleShape_create();
			sfRectangleShape_setSize(newLaser->hitbox, (sfVector2f) { LASER_VERTICAL_HITBOX_SIZE_X, LASER_VERTICAL_HITBOX_SIZE_Y });
			sfRectangleShape_setPosition(newLaser->hitbox, (sfVector2f) { _position.x, _position.y + LASER_VERTICAL_HITBOX_SHIFT});
			sfRectangleShape_setOutlineColor(newLaser->hitbox, sfRed);
			sfRectangleShape_setOutlineThickness(newLaser->hitbox, 1);
			sfRectangleShape_setFillColor(newLaser->hitbox, sfColor_fromRGBA(0, 0, 0, 0));
			break;

		case LASER_TYPE_VERTICAL :
			newLaser->sprite = sfSprite_create();
			sfSprite_setTexture(newLaser->sprite, laserData.textureVertical, sfTrue);

			newLaser->idleAnimation = CreateAnimation(newLaser->sprite, LASER_IDLE_FRAMES, LASER_FRAMERATE, (sfIntRect) { 0, 0, LASER_VERTICAL_SIZE, LASER_VERTICAL_SIZE }, sfTrue);
			newLaser->currentAnimation = &newLaser->idleAnimation;

			//Set the sprite position
			sfSprite_setPosition(newLaser->sprite, _position);
			sfSprite_setScale(newLaser->sprite, (sfVector2f) { SCALE, SCALE });

			//Set the hitbox
			newLaser->hitbox = sfRectangleShape_create();
			sfRectangleShape_setSize(newLaser->hitbox, (sfVector2f) { LASER_HORIZONTAL_HITBOX_SIZE_X, LASER_HORIZONTAL_HITBOX_SIZE_Y});
			sfRectangleShape_setPosition(newLaser->hitbox, (sfVector2f) { _position.x + LASER_HORIZONTAL_HITBOX_SHIFT, _position.y });
			sfRectangleShape_setOutlineColor(newLaser->hitbox, sfRed);
			sfRectangleShape_setOutlineThickness(newLaser->hitbox, 1);
			sfRectangleShape_setFillColor(newLaser->hitbox, sfColor_fromRGBA(0, 0, 0, 0));
			break;

		default : 
			break;
		}
		
		//Add the new pickup to the pickupList
		Element* laserElement = CreateElement((void*)newLaser);
		InsertElement(laserData.laserList, laserElement, 0);

	}
	else
	{
		printf("Fail Calloc in Add Pickup\n");
	}
}

unsigned GetLaserNb(void)
{
	return laserData.laserList->count;
}

sfFloatRect GetLaser(unsigned _arrayPos)
{
	Element* element = GetElement(laserData.laserList, _arrayPos);
	Laser* laser = element->data;

	return sfRectangleShape_getGlobalBounds(laser->hitbox);
}

sfBool GetLaserActivity()
{
	
	return laserData.isOn;
}