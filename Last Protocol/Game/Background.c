#include "Background.h"

BackgroundData background;

void LoadBackground(LevelNumber _levelNumber)
{
	background.levelNumber = _levelNumber;
	switch (_levelNumber)
	{
	case LEVEL_1:
		background.textureBottom = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_1/Bottom.png", NULL);
		background.spriteBottom = CreateSimpleSprite(background.textureBottom, SCALE);

		background.paralax[PARALAX_1].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_1/Layer 1.png", NULL);
		background.paralax[PARALAX_2].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_1/Layer 2.png", NULL);
		background.paralax[PARALAX_3].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_1/Layer 3.png", NULL);

		for (unsigned i = 0; i < NB_PARALAX; i++)
		{
			background.paralax[i].sprite = CreateSimpleSprite(background.paralax[i].texture, SCALE);
			sfSprite_setPosition(background.paralax[i].sprite, (sfVector2f) { 0, PARALAX_OFFSET_Y });

			background.paralax[i].copySprite = sfSprite_copy(background.paralax[i].sprite);
			sfSprite_setPosition(background.paralax[i].copySprite, (sfVector2f) { PARALAX_WIDTH* SCALE, PARALAX_OFFSET_Y });

			sfIntRect firstFrame = { 0, 0, PARALAX_WIDTH, PARALAX_HEIGHT };

			LoadAnimation(&background.paralax[i].animation);
			background.paralax[i].animation = CreateAnimation(background.paralax[i].sprite, PARALAX_NB_FRAME, PARALAX_FRAME_RATE, firstFrame, sfTrue);

			LoadAnimation(&background.paralax[i].copyAnimation);
			background.paralax[i].copyAnimation = CreateAnimation(background.paralax[i].copySprite, PARALAX_NB_FRAME, PARALAX_FRAME_RATE, firstFrame, sfTrue);
		}
		background.paralax[PARALAX_1].speed.x = PARALAX_1_SPEED;
		background.paralax[PARALAX_2].speed.x = PARALAX_2_SPEED;
		background.paralax[PARALAX_3].speed.x = PARALAX_3_SPEED;
		break;

	case LEVEL_2:
		background.textureBottom = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Bottom.png", NULL);
		background.spriteBottom = CreateSimpleSprite(background.textureBottom, SCALE);
		sfFloatRect levelRect = GetLevelHitbox();
		sfFloatRect size = sfSprite_getGlobalBounds(background.spriteBottom);
		sfSprite_setPosition(background.spriteBottom, (sfVector2f) { 0, levelRect.top + levelRect.height - size.height });

		background.textureDegrade = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Degrade.png", NULL);
		background.spriteDegrade = CreateSimpleSprite(background.textureDegrade, SCALE);

		background.textureLight1 = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Light 1.png", NULL);
		background.spriteLight1 = CreateSimpleSprite(background.textureLight1, SCALE);

		background.textureLight2 = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Light 2.png", NULL);
		background.spriteLight2 = CreateSimpleSprite(background.textureLight2, SCALE);

		background.paralax[PARALAX_1].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Layer 1.png", NULL);
		background.paralax[PARALAX_2].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Layer 2.png", NULL);
		background.paralax[PARALAX_3].texture = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_2/Layer 3.png", NULL);

		for (unsigned i = 0; i < NB_PARALAX; i++)
		{
			background.paralax[i].sprite = CreateSimpleSprite(background.paralax[i].texture, SCALE);
			sfSprite_setPosition(background.paralax[i].sprite, (sfVector2f) { 0, levelRect.top + levelRect.height - size.height });

		}
		background.bottomSpeed = -LV2_BOTTOM_SPEED;
		background.paralax[PARALAX_1].speed.x = -LV2_PARALAX_1_SPEED;
		background.paralax[PARALAX_2].speed.x = -LV2_PARALAX_2_SPEED;
		background.paralax[PARALAX_3].speed.x = -LV2_PARALAX_3_SPEED;
		break;

	case LEVEL_3:
	{
		sfFloatRect rect = { 0, 0, SCREEN_WIDTH, RECTANGLE_RED_HEIGHT };
		sfColor color = sfColor_fromRGB(200, 16, 0);
		background.rectangleRed = CreateRectangle(rect, sfFalse, color, 0, sfTransparent);

		background.textureBottom = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_3/Bottom.png", NULL);
		background.spriteBottom = CreateSimpleSprite(background.textureBottom, SCALE);

		background.textureTree = sfTexture_createFromFile("Assets/Textures/Game/Background/Level_3/Tree.png", NULL);
		background.spriteTree = CreateSimpleSprite(background.textureTree, SCALE);

		sfIntRect firstFrame = { 0, 0, TREE_WIDTH, TREE_HEIGHT };
		background.animTree[IDLE_TREE] = CreateAnimation(background.spriteTree, TREE_IDLE_NB_FRAME, TREE_IDLE_FRAMERATE, firstFrame, sfTrue);

		firstFrame = (sfIntRect){ 0, TREE_HEIGHT, TREE_WIDTH, TREE_HEIGHT };
		background.animTree[BLINK_TREE] = CreateAnimation(background.spriteTree, TREE_IDLE_NB_FRAME, TREE_IDLE_FRAMERATE, firstFrame, sfTrue);

		sfSprite_setOrigin(background.spriteTree, (sfVector2f) { TREE_WIDTH / 2, TREE_HEIGHT / 2 });

		background.currentAnimTree = &background.animTree[IDLE_TREE];

		background.rectRedSpeed = RECTANGLE_RED_SPEED;
	}
	break;
	}

}

void UpdateBackground(sfRenderWindow* _window, float _dt)
{

	sfFloatRect rect[NB_PARALAX] = { 0 };
	sfFloatRect rectCopy[NB_PARALAX] = { 0 };
	sfVector2f cameraVelocity = GetCamera()->velocity;
	sfFloatRect cameraRect = GetCameraBounds();

	switch (background.levelNumber)
	{
	case LEVEL_1:
		LinkSpriteToCamera(background.spriteBottom, _window, (sfVector2f) { 0, 0 });

		for (unsigned i = PARALAX_3; i <= PARALAX_1; i++)
		{
			LinkSpriteToCameraVerti(background.paralax[i].sprite, _window, (sfVector2f) { 0, PARALAX_OFFSET_Y });
			LinkSpriteToCameraVerti(background.paralax[i].copySprite, _window, (sfVector2f) { 0, PARALAX_OFFSET_Y });

			rect[i] = sfSprite_getGlobalBounds(background.paralax[i].sprite);
			rectCopy[i] = sfSprite_getGlobalBounds(background.paralax[i].copySprite);

			sfSprite_move(background.paralax[i].sprite, (sfVector2f) { cameraVelocity.x* background.paralax[i].speed.x* _dt, 0 });
			sfSprite_move(background.paralax[i].copySprite, (sfVector2f) { cameraVelocity.x* background.paralax[i].speed.x* _dt, 0 });

			if (rect[i].left + rect[i].width < cameraRect.left)
			{
				sfSprite_setPosition(background.paralax[i].sprite, (sfVector2f) { rectCopy[i].left + rectCopy[i].width });
			}
			else if (rectCopy[i].left + rectCopy[i].width < cameraRect.left)
			{
				sfSprite_setPosition(background.paralax[i].copySprite, (sfVector2f) { rect[i].left + rect[i].width });
			}
			else if (rect[i].left > cameraRect.left + cameraRect.width)
			{
				sfSprite_setPosition(background.paralax[i].sprite, (sfVector2f) { rectCopy[i].left - rectCopy[i].width });
			}
			else if (rectCopy[i].left > cameraRect.left + cameraRect.width)
			{
				sfSprite_setPosition(background.paralax[i].copySprite, (sfVector2f) { rect[i].left - rect[i].width });
			}

			UpdateAnimationVerti(&background.paralax[i].animation, _dt);
			UpdateAnimationVerti(&background.paralax[i].copyAnimation, _dt);
		}
		break;

	case LEVEL_2:
		LinkSpriteToCameraHoriz(background.spriteBottom, _window, (sfVector2f) { 0, 0 });
		LinkSpriteToCamera(background.spriteDegrade, _window, (sfVector2f) { 0, 0 });
		LinkSpriteToCamera(background.spriteLight1, _window, (sfVector2f) { 0, 0 });
		LinkSpriteToCamera(background.spriteLight2, _window, (sfVector2f) { 0, 0 });
		sfSprite_move(background.spriteBottom, (sfVector2f) { 0, cameraVelocity.y* background.bottomSpeed* _dt });

		for (unsigned i = PARALAX_3; i <= PARALAX_1; i++)
		{
			LinkSpriteToCameraHoriz(background.paralax[i].sprite, _window, (sfVector2f) { 0, 0 });
			sfSprite_move(background.paralax[i].sprite, (sfVector2f) { 0, cameraVelocity.y* background.paralax[i].speed.x* _dt });
		}
		break;

	case LEVEL_3:
	{
		LinkSpriteToCameraHoriz(background.rectangleRed, _window, (sfVector2f) { 0, 0 });

		sfVector2f rectPos = sfRectangleShape_getPosition(background.rectangleRed);
		if (rectPos.y < cameraRect.top)
		{
			background.rectRedSpeed = RECTANGLE_RED_SPEED;
		}
		else if (rectPos.y > cameraRect.top + cameraRect.height)
		{
			background.rectRedSpeed = -RECTANGLE_RED_SPEED;
		}

		sfVector2f rectangleMove = { 0, background.rectRedSpeed * _dt };
		sfRectangleShape_move(background.rectangleRed, rectangleMove);

		LinkSpriteToCamera(background.spriteBottom, _window, (sfVector2f) { 0, 0 });

		sfVector2f positionTree = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		LinkSpriteToCamera(background.spriteTree, _window, positionTree);

		UpdateAnimation(background.currentAnimTree, _dt);
	}

		break;
	}
}

void DrawBackground(sfRenderWindow* _window)
{

	switch (background.levelNumber)
	{
	case LEVEL_1:
		sfRenderWindow_drawSprite(_window, background.spriteBottom, NULL);

		for (unsigned i = 0; i < NB_PARALAX; i++)
		{
			DrawAnimation(&background.paralax[i].animation, _window);
			DrawAnimation(&background.paralax[i].copyAnimation, _window);
		}
		break;

	case LEVEL_2:
		sfRenderWindow_drawSprite(_window, background.spriteBottom, NULL);

		sfRenderWindow_drawSprite(_window, background.spriteDegrade, NULL);

		sfRenderWindow_drawSprite(_window, background.paralax[PARALAX_3].sprite, NULL);
		sfRenderWindow_drawSprite(_window, background.spriteLight2, NULL);
		sfRenderWindow_drawSprite(_window, background.paralax[PARALAX_2].sprite, NULL);
		sfRenderWindow_drawSprite(_window, background.spriteLight1, NULL);
		sfRenderWindow_drawSprite(_window, background.paralax[PARALAX_1].sprite, NULL);
		break;

	case LEVEL_3:
	{
		sfColor color = sfColor_fromRGB(61, 9, 14);
		sfRenderWindow_clear(_window, color);
		sfRenderWindow_drawRectangleShape(_window, background.rectangleRed, NULL);
		sfRenderWindow_drawSprite(_window, background.spriteBottom, NULL);
		//DrawAnimation(background.currentAnimTree, _window);

	}
	break;
	}
}

void CleanupBackground(void)
{
	sfTexture_destroy(background.textureBottom);
	sfSprite_destroy(background.spriteBottom);

	sfTexture_destroy(background.textureDegrade);
	sfSprite_destroy(background.spriteDegrade);

	sfTexture_destroy(background.textureLight1);
	sfSprite_destroy(background.spriteLight1);

	sfTexture_destroy(background.textureLight2);
	sfSprite_destroy(background.spriteLight2);

	sfTexture_destroy(background.textureTree);
	sfSprite_destroy(background.spriteTree);

	for (unsigned i = 0; i < NB_PARALAX; i++)
	{
		sfTexture_destroy(background.paralax[i].texture);
		sfSprite_destroy(background.paralax[i].sprite);
	}
	background = (BackgroundData){ 0 };
}