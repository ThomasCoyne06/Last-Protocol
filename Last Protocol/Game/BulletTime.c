#include "BulletTime.h"


BulletTimeData bullet_Time;

void LoadCircleBulletTime();
void LoadSquareBulletTime();

void LoadBulletTime()
{
	bullet_Time.textureCircle = sfTexture_createFromFile("Assets/Textures/Game/BulletTime/bullet timeSplit-Sheet.png", NULL);
	LoadCircleBulletTime();
	LoadSquareBulletTime();
	

}

void UpdateBulletTimeAnimation(float _dt, sfRenderWindow* _window)
{

	sfVector2f pos = sfSprite_getPosition(GetPlayer()->sprite);
	sfSprite_setPosition(bullet_Time.spriteCircle, pos );
	SetSpriteCenterCamera(_window, bullet_Time.spriteSquare); 
	
	

	if (GetPlayerBulletTime())
	{

		UpdateAnimation(&bullet_Time.animation,_dt /* * BULLET_TIME_SLOW*/);

	}
	else if (!GetPlayerBulletTime() && bullet_Time.animation.frame !=0)
	{
		UpdateReverseAnimation(&bullet_Time.animation, _dt);
	}

	if ((GetPlayerBulletTime() && bullet_Time.animation.frame == bullet_Time.animation.nbFrame - 1))
	{
		UpdateAnimation(&bullet_Time.animationSquare, _dt  * BULLET_TIME_SLOW);
	}

	
}

void DrawBulletTimeAnimation(sfRenderWindow* _window)
{
	if ((GetPlayerBulletTime() || bullet_Time.animation.frame > 0) && bullet_Time.animation.frame != bullet_Time.animation.nbFrame-1 )
	{
		DrawAnimation(&bullet_Time.animation, _window);
	}
	else if (GetPlayerBulletTime() && bullet_Time.animation.frame == bullet_Time.animation.nbFrame - 1)
	{
		
		DrawAnimation(&bullet_Time.animationSquare, _window);
	}
}


void CleanUpBulletTimeAnimation()
{
	sfSprite_destroy(bullet_Time.spriteCircle);
	bullet_Time.spriteCircle = NULL;

	sfTexture_destroy(bullet_Time.textureCircle);
	bullet_Time.textureCircle = NULL;
 
}

void LoadCircleBulletTime()
{
	
	bullet_Time.spriteCircle = sfSprite_create();
	
	sfSprite_setTexture(bullet_Time.spriteCircle, bullet_Time.textureCircle, sfTrue);
	
	sfSprite_setScale(bullet_Time.spriteCircle, (sfVector2f) { SCALE, SCALE });
	sfFloatRect rect = sfSprite_getGlobalBounds(bullet_Time.spriteCircle);



	sfIntRect firstFrame = { 0,0,1280,1440 };
	bullet_Time.animation = CreateAnimation(bullet_Time.spriteCircle, 6, FRAME_RATE_BULLETTIME, firstFrame, sfFalse);
	sfSprite_setOrigin(bullet_Time.spriteCircle, (sfVector2f) { 1280 / 2, 1440 / 2 });
	
}

void LoadSquareBulletTime()
{
	//bullet_Time.spriteSquare = sfSprite_create();
	//sfSprite_setTexture(bullet_Time.spriteSquare, bullet_Time.textureCircle, sfTrue);

	//sfSprite_setScale(bullet_Time.spriteSquare, (sfVector2f) { SCALE, SCALE });
	sfIntRect firstFrame = { 0,1770,640,360 };
	bullet_Time.spriteSquare = CreateSpriteFromSpriteSheet(bullet_Time.textureCircle, firstFrame,sfTrue,SCALE);

	/*sfIntRect firstFrame = { 0,1770,640,360 };*/
	//sfIntRect firstFrame = { 0,0,1280,1440 };
	bullet_Time.animationSquare = CreateAnimation(bullet_Time.spriteSquare, 3, 5, firstFrame, sfTrue);
	//sfSprite_setOrigin(bullet_Time.spriteSquare, (sfVector2f) { firstFrame.width / 2, firstFrame.height/2 });
	

}