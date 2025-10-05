#include "Bumper.h"

BumperData bumperData = { 0 };

void LoadBumper()
{
	bumperData.bumperList = CreateList();

	//Load the animation reference
	bumperData.texture = sfTexture_createFromFile("Assets/Textures/Game/Bumper/Bumper.png", NULL);
	bumperData.sprite = sfSprite_create();
	sfSprite_setTexture(bumperData.sprite, bumperData.texture, sfTrue);
	bumperData.idleAnimation = CreateAnimation(bumperData.sprite, BUMPER_IDLE_FRAMES, BUMPER_IDLE_FRAMERATE, (sfIntRect) { 0, 0, BUMPER_SPRITE_WIDTH, BUMPER_SPRITE_HEIGHT }, sfTrue);
	bumperData.currentAnimation = &bumperData.idleAnimation;


	//Load all Bumper
	EntityData bumper = GetLevel()->entityList[BUMPER];
	for (unsigned i = 0; i < bumper.count; i++)
	{
		AddBumper(bumper.positionList[i]);
	}
}

void UpdateBumper(float _dt)
{

	UpdateAnimation(bumperData.currentAnimation, _dt);

	//Update the animation of all Bumper
	for (unsigned i = 0; i < bumperData.bumperList->count; i++)
	{
		Element* bumperElement = GetElement(bumperData.bumperList, i);
		Bumper* bumper = bumperElement->data;
		UpdateAnimation(bumper->currentAnimation, _dt);

		//Set the idleAnimation when the bump animation is finished
		if (bumper->isBumping == sfTrue && AnimationIsFinished(bumper->currentAnimation) == sfTrue)
		{
			bumper->currentAnimation = &bumper->idleAnimation;
			bumper->currentAnimation->frame = bumperData.currentAnimation->frame;
			bumper->currentAnimation->timer = bumperData.currentAnimation->timer;
			bumper->isBumping = sfFalse;
		}
	}
}

void DrawBumper(sfRenderWindow* _window)
{
	for (unsigned i = 0; i <bumperData.bumperList->count; i++)
	{
		Element* bumperElement = GetElement(bumperData.bumperList, i);
		Bumper* bumper = bumperElement->data;
		DrawAnimation(bumper->currentAnimation, _window);
	}
}

void CleanupBumper()
{
	//Cleanup each element of the pickupList
	Element* actualElement = bumperData.bumperList->first;

	while (actualElement != NULL)
	{
		Bumper* bumper = actualElement->data;

		//Cleanup the sprite and the texture
		sfSprite_destroy(bumper->sprite);
		bumper->sprite = NULL;

		sfTexture_destroy(bumper->texture);
		bumper->texture = NULL;

		//Go to the next element
		actualElement = actualElement->next;
	}
	DeleteList(bumperData.bumperList);

	sfTexture_destroy(bumperData.texture);
	bumperData.texture = NULL;

	sfSprite_destroy(bumperData.sprite);
	bumperData.sprite = NULL;

	bumperData = (BumperData){ NULL };
}

void AddBumper(sfVector2f _BumperPosition)
{
	Bumper* newBumper = (Bumper*)calloc(1, sizeof(Bumper));
	//Initiate the new Bumper
	if (newBumper != NULL)
	{
		newBumper->texture = sfTexture_createFromFile("Assets/Textures/Game/Bumper/Bumper.png", NULL);
		newBumper->sprite = sfSprite_create();
		sfSprite_setTexture(newBumper->sprite, newBumper->texture, sfTrue);

		newBumper->idleAnimation = CreateAnimation(newBumper->sprite, BUMPER_IDLE_FRAMES, BUMPER_IDLE_FRAMERATE, (sfIntRect) { 0, 0, BUMPER_SPRITE_WIDTH, BUMPER_SPRITE_HEIGHT }, sfTrue);
		newBumper->bumpAnimation = CreateAnimation(newBumper->sprite, BUMPER_BUMP_FRAMES, BUMPER_BUMP_FRAMERATE, (sfIntRect) { 0, BUMPER_SPRITE_HEIGHT, BUMPER_SPRITE_WIDTH, BUMPER_SPRITE_HEIGHT }, sfFalse);

		newBumper->currentAnimation = &newBumper->idleAnimation;

		//Set the sprite position

		sfVector2f bumperPosition = { _BumperPosition.x,_BumperPosition.y - (BUMPER_ADJUSTING_POS*SCALE) };
		sfSprite_setPosition(newBumper->sprite, bumperPosition);
		sfSprite_setScale(newBumper->sprite, (sfVector2f) { SCALE, SCALE });

		//Add the new bumper to the bumperList
		Element* bumperElement = CreateElement((void*)newBumper);
		InsertElement(bumperData.bumperList, bumperElement, 0);
	}
	else
	{
		printf("Fail Calloc in Add Pickup\n");
	}
}

void ActivateBumper(unsigned _arrayPos)
{
	Element* bumperElement = GetElement(bumperData.bumperList, _arrayPos);
	Bumper* bumper = bumperElement->data;

	LoadAnimation(&bumper->bumpAnimation);
	bumper->currentAnimation = &bumper->bumpAnimation;
	bumper->isBumping = sfTrue;
}

unsigned GetBumperNb(void)
{
	return bumperData.bumperList->count;
}

sfFloatRect GetBumperHitbox(unsigned _arrayPos)
{
	Element* bumperElement = GetElement(bumperData.bumperList, _arrayPos);
	Bumper* bumper = bumperElement->data;

	return sfSprite_getGlobalBounds(bumper->sprite);
}

sfBool GetBumperActivity(unsigned _arrayPos)
{
	Element* bumperElement = GetElement(bumperData.bumperList, _arrayPos);
	Bumper* bumper = bumperElement->data;

	return bumper->isBumping;
}