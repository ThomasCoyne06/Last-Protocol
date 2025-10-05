#include "Pickup.h"
#include "../Resources/Sounds.h"

void DestroyPickup(unsigned _arrayPos);

PickupData pickupData = { 0 };

void LoadPickup(void)
{
	pickupData.pickupList = CreateList();

	EntityData pickup = GetLevel()->entityList[PICKUP_FILE];
	for (unsigned i = 0; i < pickup.count; i++)
	{
		AddPickup(P_FILE, pickup.positionList[i]);
	}

	pickup = GetLevel()->entityList[PICKUP_LIFE];
	for (unsigned i = 0; i < pickup.count; i++)
	{
		AddPickup(P_LIFE, pickup.positionList[i]);
	}
}

void UpdatePickup(float _dt)
{
	//Update the animation of all pickups
	for (unsigned i = 0; i < pickupData.pickupList->count; i++)
	{
		Element* pickupElement = GetElement(pickupData.pickupList, i);
		Pickup* pickup = pickupElement->data;
		UpdateAnimation(pickup->currentAnimation, _dt);
		//Destroy the pickup if it is exploding
		if (pickup->isExploding)
		{
			sfFloatRect hudRect = { 0 };
			sfVector2f hudPos = { 0 };
			switch (pickup->type)
			{
			case P_FILE:
				hudRect = sfSprite_getGlobalBounds(GetHUD()->energyBar);
				hudPos = (sfVector2f){ hudRect.left + hudRect.width / 2, hudRect.top + hudRect.height / 2 };
				break;
			case P_LIFE:
				hudRect = sfSprite_getGlobalBounds(GetHUD()->lifeSprite);
				hudPos = (sfVector2f){ hudRect.left + hudRect.width / 2, hudRect.top + hudRect.height / 2 };
				break;
			}
			sfVector2f pickupPos = sfSprite_getPosition(pickup->sprite);
			float angle = GetAngle(pickupPos, hudPos);
			sfVector2f velocity = { PICKUP_MOVE_SPEED * cosf(angle) * _dt, PICKUP_MOVE_SPEED * sinf(angle) * _dt };
			sfSprite_move(pickup->sprite, velocity);
			if (AnimationIsFinished(pickup->currentAnimation))
			{
				RemoveElement(pickupData.pickupList, i);

				if (pickup->type == P_FILE)
				{
					fileCollected++;
				}
				
				SetHUD()->canShowEnergyReact = sfTrue;
				IncreaseEnergy(ENERGY_FILE);
			}
		}
	}
}

void DrawPickup(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < pickupData.pickupList->count; i++)
	{
		Element* pickupElement = GetElement(pickupData.pickupList, i);
		Pickup* pickup = pickupElement->data;
		if (!pickup->isExploding)
		{
			DrawAnimation(pickup->currentAnimation, _window);
		}
	}
}

void DrawObtainedPickup(sfRenderWindow* _window)
{
	for (unsigned i = 0; i < pickupData.pickupList->count; i++)
	{
		Element* pickupElement = GetElement(pickupData.pickupList, i);
		Pickup* pickup = pickupElement->data;
		if (pickup->isExploding)
		{
			DrawAnimation(pickup->currentAnimation, _window);
		}
	}
}
void CleanupPickup(void)
{
	//Cleanup each element of the pickupList
	Element* actualElement = pickupData.pickupList->first;

	while (actualElement != NULL)
	{
		Pickup* pickup = actualElement->data;

		//Cleanup the sprite and the texture
		sfSprite_destroy(pickup->sprite);
		pickup->sprite = NULL;

		sfTexture_destroy(pickup->texture);
		pickup->texture = NULL;

		//Go to the next element
		actualElement = actualElement->next;
	}

	DeleteList(pickupData.pickupList);

	pickupData = (PickupData){ NULL };
}

unsigned GetPickupNb(void)
{
	return pickupData.pickupList->count;
}

sfFloatRect GetPickup(unsigned _arrayPos)
{
	Element* element = GetElement(pickupData.pickupList, _arrayPos);
	Pickup* pickup = element->data;

	return sfSprite_getGlobalBounds(pickup->sprite);
}

int GetPickupType(unsigned _arrayPos)
{
	Element* element = GetElement(pickupData.pickupList, _arrayPos);
	Pickup* pickup = element->data;

	return pickup->type;
}

void AddPickup(PickupType _type, sfVector2f _position)
{
	Pickup* newPickup = (Pickup*)calloc(1, sizeof(Pickup));
	//Initiate the new Pickup
	if (newPickup != NULL)
	{
		switch (_type)
		{
		case P_FILE:

			newPickup->type = _type;
			newPickup->texture = sfTexture_createFromFile("Assets/Textures/Game/Pickup/PickupFile.png", NULL);
			newPickup->sprite = sfSprite_create();
			sfSprite_setTexture(newPickup->sprite, newPickup->texture, sfTrue);

			newPickup->idleAnimation = CreateAnimation(newPickup->sprite, FILE_IDLE_FRAMES, FILE_IDLE_FRAMERATE, (sfIntRect) { 0, 0, FILE_SPRITE_SIZE, FILE_SPRITE_SIZE }, sfTrue);
			newPickup->destroyAnimation = CreateAnimation(newPickup->sprite, FILE_DESTROY_FRAME, FILE_DESTROY_FRAMERATE, (sfIntRect) { 0, FILE_SPRITE_SIZE, FILE_SPRITE_SIZE, FILE_SPRITE_SIZE }, sfFalse);
			break;
		case P_LIFE:

			newPickup->type = _type;
			newPickup->texture = sfTexture_createFromFile("Assets/Textures/Game/Pickup/PickupLife.png", NULL);
			newPickup->sprite = sfSprite_create();
			sfSprite_setTexture(newPickup->sprite, newPickup->texture, sfTrue);

			newPickup->idleAnimation = CreateAnimation(newPickup->sprite, LIFE_IDLE_FRAMES, LIFE_IDLE_FRAMERATE, (sfIntRect) { 0, 0, LIFE_SPRITE_SIZE_X, LIFE_SPRITE_SIZE_Y }, sfTrue);
			newPickup->destroyAnimation = CreateAnimation(newPickup->sprite, LIFE_DESTROY_FRAME, LIFE_DESTROY_FRAMERATE, (sfIntRect) { 0, LIFE_SPRITE_SIZE_Y, LIFE_SPRITE_SIZE_X, LIFE_SPRITE_SIZE_Y }, sfFalse);
			break;
		default:
			break;
		}

		newPickup->currentAnimation = &newPickup->idleAnimation;

		//Set the sprite position
		sfSprite_setPosition(newPickup->sprite, _position);
		sfSprite_setScale(newPickup->sprite, (sfVector2f) { SCALE, SCALE });

		//Add the new pickup to the pickupList
		Element* pickupElement = CreateElement((void*)newPickup);
		InsertElement(pickupData.pickupList, pickupElement, 0);

	}
	else
	{
		printf("Fail Calloc in Add Pickup\n");
	}
}

void DestroyPickup(unsigned _arrayPos)
{

}

void ObtainPickup(unsigned _arrayPos)
{
	Element* pickupElement = GetElement(pickupData.pickupList, _arrayPos);
	Pickup* pickup = pickupElement->data;

	switch (pickup->type)
	{
	case P_FILE:

		if (pickup->isExploding == sfFalse)
		{
			PlaySound(PICK1 + rand() % 4, (sfVector2f) { 0, 0 }, sfFalse, sfFalse);
			//IncreaseEnergy(ENERGY_FILE);
			//AddFile();
		}

		break;
	case P_LIFE:
		if (pickup->isExploding == sfFalse)
		{
			PlaySound(PICK_LIFE, (sfVector2f) { 0, 0 }, sfFalse, sfFalse);
			EditPlayerHP(1);
		}
		break;
	default:
		break;
	}



	pickup->currentAnimation = &pickup->destroyAnimation;
	pickup->isExploding = sfTrue;
}