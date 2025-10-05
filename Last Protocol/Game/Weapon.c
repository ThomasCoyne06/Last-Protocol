#include "Weapon.h"

void LoadSprite(void);
void LoadWeaponHitbox(void);
void LoadWeaponAnimation(void);

void SetWeapon(WeaponType _weaponType);
void SetWeaponScale(WeaponScale _scale);
void SetWeaponPosition(sfSprite* _sprite, sfVector2f _position);
void SetLoadPosition(void);
void SetIdleAnimation(void);
void WeaponAttack(void);
void WeaponLoadSpeed(void);
void SetLoadHitbox(void);
void SetLoadHitboxPosition(void);
void ResetAttackHitbox(void);

void UpdateWeaponPosition(void);
void PlayWeaponAttack(WeaponAnimation _attack);

void CollisionWeaponEnemy(void);

void SetAttackHitbox(void);


WeaponData weaponData = { 0 };

void LoadWeapon(void)
{
	LoadSprite();
	LoadWeaponHitbox();
	LoadWeaponAnimation();
	SetWeapon(SWORD);
}

void UpdateWeapon(float _dt)
{
	if (sfKeyboard_isKeyPressed(sfKeyY))
	{
		weaponData.isAttacking = sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyU))
	{
		ResetWeapon();
		weaponData.isAttacking = sfFalse;
	}

	if (weaponData.isAttacking == sfTrue)
	{

		SetAttackHitbox();
	}

	//if (CheckMapping(GetMapping()->resize))
	//{
	//	IncreaseSizeWeapon();
	//}
	//else
	//{
	//	weaponData.isSwapingWeapon = sfFalse;
	//}

	//Set the weapon load Speed
	if (weaponData.weaponType == LOAD)
	{
		SetLoadHitbox();
		WeaponLoadSpeed();
		SetLoadHitboxPosition();
	}

	//Set the position to the right and the left
	UpdateWeaponPosition();

	CollisionWeaponEnemy();

	//Update the animation
	UpdateAnimation(weaponData.currentAnimation, _dt);
}

void DrawWeapon(sfRenderWindow* _window)
{
	if (weaponData.isAttacking == sfFalse)
	{
		DrawAnimation(weaponData.currentAnimation, _window);
	}

	//Draw Hitboxes
	if (weaponData.isAttacking)
	{
		//sfRenderWindow_drawRectangleShape(_window, weaponData.hitbox[0], NULL);
	}

}

void CleanupWeapon(void)
{
	//Destroy all textures
	for (unsigned int i = 0; i < WEAPON_NB; i++)
	{
		sfTexture_destroy(weaponData.textureArray[i]);
	}

	//Destroy all sprites
	for (unsigned int i = 0; i < WEAPON_NB; i++)
	{
		sfSprite_destroy(weaponData.spriteArray[i]);
	}

	//Reset the weaponData infos
	weaponData = (WeaponData){ 0 };
}

int GetWeaponHitboxNumber(void)
{
	if (weaponData.isAttacking)
	{
		switch (weaponData.weaponType)
		{
		case SWORD:
			return 1;
			break;
		case LOAD:
			return LOAD_HITBOX_NB;
			break;
		default:
			break;
		}
	}
	return 0;
}

sfRectangleShape* GetWeaponHitbox(int _arrayPos)
{
	return weaponData.hitbox[_arrayPos];
}

void LoadSprite(void)
{
	//Load Swords texture and sprite
	weaponData.textureArray[SWORD] = sfTexture_createFromFile("Assets/Textures/Game/Weapon/Epee-spritesheet.png", NULL);
	weaponData.spriteArray[SWORD] = sfSprite_create();
	sfSprite_setTexture(weaponData.spriteArray[SWORD], weaponData.textureArray[SWORD], sfTrue);
	sfSprite_setScale(weaponData.spriteArray[SWORD], (sfVector2f) { SCALE, SCALE });

	//Load Load texture and sprite
	weaponData.textureArray[LOAD] = sfTexture_createFromFile("Assets/Textures/Game/Weapon/LoadAnimation.png", NULL);
	weaponData.spriteArray[LOAD] = sfSprite_create();
	sfSprite_setTexture(weaponData.spriteArray[LOAD], weaponData.textureArray[LOAD], sfTrue);
	//sfFloatRect loadHitbox = sfSprite_getGlobalBounds(weaponData.spriteArray[LOAD]);
	sfSprite_setOrigin(weaponData.spriteArray[LOAD], (sfVector2f) { LOAD_ORIGIN_X, LOAD_ORIGIN_Y });
}

void LoadWeaponHitbox(void)
{
	for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
	{
		weaponData.hitbox[i] = sfRectangleShape_create();
		sfRectangleShape_setOutlineColor(weaponData.hitbox[i], sfRed);
		sfRectangleShape_setOutlineThickness(weaponData.hitbox[i], 1);
		sfRectangleShape_setFillColor(weaponData.hitbox[i], sfColor_fromRGBA(0, 0, 0, 0));
	}
}

void LoadWeaponAnimation(void)
{
	//SWORD
	//SWORD_NORMAL_IDLE
	weaponData.animationArray[SWORD_IDLE_SMALL] = CreateAnimation(weaponData.spriteArray[SWORD], SWORD_IDLE_FRAME_NB, SWORD_IDLE_FRAMERATE, (sfIntRect) { 0, SWORD_HEIGHT, SWORD_WIDTH, SWORD_HEIGHT }, sfTrue);

	//SWORD_VERTICAL_IDLE
	weaponData.animationArray[SWORD_IDLE_MEDIUM] = CreateAnimation(weaponData.spriteArray[SWORD], SWORD_IDLE_FRAME_NB, SWORD_IDLE_FRAMERATE, (sfIntRect) { 0, 0, SWORD_WIDTH, SWORD_HEIGHT }, sfTrue);

	//SWORD_HORIZONTAL_IDLE
	weaponData.animationArray[SWORD_IDLE_BIG] = CreateAnimation(weaponData.spriteArray[SWORD], 8, 10, (sfIntRect) { 0, SWORD_HEIGHT * 2, SWORD_WIDTH, 73 }, sfTrue);


	sfSprite_setOrigin(weaponData.spriteArray[SWORD], (sfVector2f) { SWORD_WIDTH / 2, SWORD_HEIGHT * 2 / 3 });
	//LOAD

	weaponData.animationArray[LOAD_SMALL] = CreateAnimation(weaponData.spriteArray[LOAD], 1, 10, (sfIntRect) { 0, 0, LOAD_SIZE_X, LOAD_SIZE_Y }, sfTrue);


}

void SetWeapon(WeaponType _weaponType)
{
	//Set the equipped Type and the IDLE animation of the weapon
	weaponData.weaponType = _weaponType;
	switch (_weaponType)
	{
	case SWORD:
		weaponData.currentAnimation = &weaponData.animationArray[SWORD_IDLE_SMALL];
		LoadAnimation(&weaponData.animationArray[SWORD_IDLE_SMALL]);
		break;
	case LOAD:
		weaponData.currentAnimation = &weaponData.animationArray[LOAD_SMALL];
		LoadAnimation(&weaponData.animationArray[LOAD_SMALL]);
		break;
	default:
		break;
	}
}

void SetIdleAnimation(void)
{
	if (weaponData.weaponType == SWORD)
	{
		switch (weaponData.weaponScale)
		{
		case STRETCH_SMALL:
			weaponData.currentAnimation = &weaponData.animationArray[SWORD_IDLE_SMALL];
			LoadAnimation(&weaponData.animationArray[SWORD_IDLE_SMALL]);
			break;
		case STRETCH_MEDIUM:
			weaponData.currentAnimation = &weaponData.animationArray[SWORD_IDLE_MEDIUM];
			LoadAnimation(&weaponData.animationArray[SWORD_IDLE_MEDIUM]);
			break;
		case STRETCH_BIG:
			weaponData.currentAnimation = &weaponData.animationArray[SWORD_IDLE_BIG];
			LoadAnimation(&weaponData.animationArray[SWORD_IDLE_BIG]);
			break;
		default:
			break;
		}
	}

}

void WeaponAttack(void)
{
	switch (weaponData.weaponScale)
	{
	case STRETCH_SMALL:
		PlayWeaponAttack(SWORD_NORMAL_SLASH);
		break;
	case STRETCH_MEDIUM:
		PlayWeaponAttack(SWORD_VERTICAL_SLASH);
		break;
	case STRETCH_BIG:
		PlayWeaponAttack(SWORD_HORIZONTAL_SLASH);
		break;
	default:
		break;
	}
}

void WeaponLoadSpeed(void)
{
	switch (weaponData.weaponScale)
	{
	case STRETCH_SMALL:
		sfSprite_rotate(weaponData.spriteArray[LOAD], LOAD_SPEED_SMALL);
		break;
	case STRETCH_MEDIUM:
		sfSprite_rotate(weaponData.spriteArray[LOAD], LOAD_SPEED_MEDIUM);
		break;
	case STRETCH_BIG:
		sfSprite_rotate(weaponData.spriteArray[LOAD], LOAD_SPEED_BIG);
		break;
	}
}

void SetLoadHitbox(void)
{
	for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
	{
		//Set the size of the hitbox
		switch (weaponData.weaponScale)
		{
		case STRETCH_SMALL:
			for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
			{
				sfRectangleShape_setSize(weaponData.hitbox[i], (sfVector2f) { LOAD_HITBOX_SIZE* LOAD_SCALE_SMALL - (i * 4), LOAD_HITBOX_SIZE* LOAD_SCALE_SMALL - (i * 4) });
			}
			break;
		case STRETCH_MEDIUM:
			for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
			{
				sfRectangleShape_setSize(weaponData.hitbox[i], (sfVector2f) { (float)LOAD_HITBOX_SIZE* LOAD_SCALE_MEDIUM - (i * 10), (float)LOAD_HITBOX_SIZE* LOAD_SCALE_MEDIUM - (i * 10) });
			}
			break;
		case STRETCH_BIG:
			for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
			{
				sfRectangleShape_setSize(weaponData.hitbox[i], (sfVector2f) {
					(float)LOAD_HITBOX_SIZE* LOAD_SCALE_BIG - (i * 20), (float)LOAD_HITBOX_SIZE* LOAD_SCALE_BIG - LOAD_SCALE_BIG - (i * 20)
				});
			}
			break;
		}

		//Set the origin of the hitbox
		sfFloatRect rectangle = sfRectangleShape_getGlobalBounds(weaponData.hitbox[i]);
		sfVector2f origin = { rectangle.width / 2, rectangle.height / 2 };
		sfRectangleShape_setOrigin(weaponData.hitbox[i], origin);
	}
}

void SetLoadHitboxPosition(void)
{
	//Set the position of the hitboxes
	for (unsigned i = 0; i < LOAD_HITBOX_NB; i++)
	{
		sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
		sfVector2f playerPosition = { playerHitbox.left + playerHitbox.width / 2, playerHitbox.top + playerHitbox.height / 2 };
		float angle = ConvertDegToRad(sfSprite_getRotation(weaponData.spriteArray[LOAD]) - 90 - (i * LOAD_HITBOX_ANGLE_DIFFERENCE));

		sfVector2f hitboxPosition = { 0,0 };
		switch (weaponData.weaponScale)
		{
		case STRETCH_SMALL:
			hitboxPosition.x = playerPosition.x + LOAD_RADIUS * LOAD_SCALE_SMALL * cosf(angle);
			hitboxPosition.y = playerPosition.y + LOAD_RADIUS * LOAD_SCALE_SMALL * sinf(angle);
			break;
		case STRETCH_MEDIUM:
			hitboxPosition.x = playerPosition.x + LOAD_RADIUS * LOAD_SCALE_MEDIUM * cosf(angle);
			hitboxPosition.y = playerPosition.y + LOAD_RADIUS * LOAD_SCALE_MEDIUM * sinf(angle);
			break;
		case STRETCH_BIG:
			hitboxPosition.x = playerPosition.x + 60 * LOAD_SCALE_BIG * cosf(angle);
			hitboxPosition.y = playerPosition.y + 60 * LOAD_SCALE_BIG * sinf(angle);
			break;
		}
		sfRectangleShape_setPosition(weaponData.hitbox[i], hitboxPosition);
	}
}

void ResetAttackHitbox(void)
{
	sfRectangleShape_setSize(weaponData.hitbox[0], (sfVector2f) { 1, 1 });
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
	sfRectangleShape_setOrigin(weaponData.hitbox[0], (sfVector2f) { 0, 0 });
	sfRectangleShape_setPosition(weaponData.hitbox[0], (sfVector2f) { playerHitbox.left + (playerHitbox.width / 2), playerHitbox.top + (playerHitbox.height / 2) });
}

void SetWeaponScale(WeaponScale _scale)
{
	//Set the scale information
	switch (_scale)
	{
	case STRETCH_SMALL:
		weaponData.weaponScale = STRETCH_SMALL;
		break;
	case STRETCH_MEDIUM:
		weaponData.weaponScale = STRETCH_MEDIUM;
		break;
	case STRETCH_BIG:
		weaponData.weaponScale = STRETCH_BIG;
		break;
	}

	////Resize the Load Sprite
	//if (weaponData.weaponType == LOAD)
	//{
	//	switch (weaponData.weaponScale)
	//	{
	//	case STRETCH_SMALL:
	//		sfSprite_setScale(weaponData.spriteArray[LOAD], (sfVector2f) { LOAD_SCALE_SMALL, LOAD_SCALE_SMALL });
	//		break;
	//	case STRETCH_MEDIUM:
	//		sfSprite_setScale(weaponData.spriteArray[LOAD], (sfVector2f) { LOAD_SCALE_MEDIUM, LOAD_SCALE_MEDIUM });
	//		break;
	//	case STRETCH_BIG:
	//		sfSprite_setScale(weaponData.spriteArray[LOAD], (sfVector2f) { LOAD_SCALE_BIG, LOAD_SCALE_BIG });
	//		break;
	//	}
	//}
}

void SetWeaponPosition(sfSprite* _sprite, sfVector2f _position)
{
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
	sfBool isPlayerRight = GetPlayer()->direction;
	sfVector2f position = { 0 };

	if (!weaponData.isAttacking)
	{
		//sfFloatRect weapon = sfSprite_getGlobalBounds(weaponData.spriteArray[SWORD]);
		//sfSprite_setOrigin(weaponData.spriteArray[SWORD], (sfVector2f) { weapon.width / 2, weapon.height / 2 });

		//Set the position and the direction of the Idle sprite
		if (isPlayerRight == sfTrue)
		{
			position.x = playerHitbox.left - _position.x;
			position.y = playerHitbox.top + /*playerHitbox.height/2*/ -_position.y;

			sfSprite_setScale(_sprite, (sfVector2f) { SCALE, SCALE });
			sfSprite_setRotation(weaponData.spriteArray[SWORD], 205);
		}
		else
		{
			position.x = playerHitbox.left + playerHitbox.width + _position.x;
			position.y = playerHitbox.top + /*playerHitbox.height/2*/ -_position.y;

			sfSprite_setScale(_sprite, (sfVector2f) { -SCALE, SCALE });
			sfSprite_setRotation(weaponData.spriteArray[SWORD], 155);
		}
	}
	else
	{
		//Set the position and the direction of the Attack sprite
		if (isPlayerRight == sfTrue)
		{
			position.x = playerHitbox.left + playerHitbox.width;
			position.y = playerHitbox.top - (_position.y / 2);

			sfSprite_setScale(_sprite, (sfVector2f) { SCALE, SCALE });
		}
		else
		{
			position.x = playerHitbox.left;
			position.y = playerHitbox.top - (_position.y / 2);

			sfSprite_setScale(_sprite, (sfVector2f) { -SCALE, SCALE });
		}
	}
	sfSprite_setPosition(_sprite, position);
}

void SetLoadPosition(void)
{
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
	sfVector2f position = { 0 };

	position.x = playerHitbox.left + (playerHitbox.width / 2);
	position.y = playerHitbox.top + (playerHitbox.height / 2);

	sfSprite_setPosition(weaponData.spriteArray[LOAD], position);
}

void UpdateWeaponPosition(void)
{
	switch (weaponData.weaponType)
	{
	case SWORD:
		SetWeaponPosition(weaponData.spriteArray[SWORD], (sfVector2f) { SWORD_POS_X, SWORD_POS_Y });
		break;
	case LOAD:
		SetLoadPosition();
		break;
	default:
		break;
	}
}

void PlayWeaponAttack(WeaponAnimation _attack)
{
	weaponData.currentAnimation = &weaponData.animationArray[_attack];
	LoadAnimation(weaponData.currentAnimation);
}

void CollisionWeaponEnemy(void)
{
	//for (unsigned i = 0; i < GetEnemyData()->list->count; i++)
	//{
	//	Element* enemyElement = GetElement(GetEnemyData()->list, i);
	//	EnemyObj* enemyObj = enemyElement->data;

	//	//SWORD
	//	if (weaponData.weaponType == SWORD)
	//	{
	//		sfFloatRect weaponhitbox = sfRectangleShape_getGlobalBounds(weaponData.hitbox[0]);
	//		sfFloatRect enemyRectangleHitbox = sfRectangleShape_getGlobalBounds(enemyObj->rectangleHitbox);

	//		switch (enemyObj->type)
	//		{
	//		case TYPE_01:
	//			if (IsColliding2Rectangles(weaponhitbox, enemyRectangleHitbox) && !enemyObj->hasBeenHit)
	//			{
	//				enemyObj->hasBeenHit = sfTrue;

	//				if (enemyObj->lifePoints > 0)
	//				{
	//					enemyObj->lifePoints--;
	//				}
	//			}
	//			break;
	//		case TYPE_02:
	//			if (IsColliding2Rectangles(weaponhitbox, enemyRectangleHitbox) && !enemyObj->hasBeenHit)
	//			{
	//				enemyObj->hasBeenHit = sfTrue;

	//				if (enemyObj->lifePoints > 0)
	//				{
	//					enemyObj->lifePoints--;

	//				}
	//			}
	//			break;
	//		case TYPE_03:
	//			weaponhitbox.width *= 2;
	//			if (CollisionCircleRectangleWeapon(enemyObj->circleHitbox, weaponData.hitbox[0], weaponhitbox) && !enemyObj->hasBeenHit)
	//			{
	//				enemyObj->hasBeenHit = sfTrue;

	//				if (enemyObj->lifePoints > 0)
	//				{
	//					enemyObj->lifePoints--;
	//				}
	//			}
	//		}
	//	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	//SHIELD
		//	else if (weaponData.weaponType == LOAD)
		//	{
		//		for (unsigned shield = 0; shield < LOAD_HITBOX_NB; shield++)
		//		{
		//			sfFloatRect weaponhitbox = sfRectangleShape_getGlobalBounds(weaponData.hitbox[shield]);
		//			sfFloatRect enemyRectangleHitbox = sfRectangleShape_getGlobalBounds(enemyObj->rectangleHitbox);

		//			switch (enemyObj->type)
		//			{
		//			case TYPE_01:
		//				if (IsColliding2Rectangles(weaponhitbox, enemyRectangleHitbox))
		//				{
		//					enemyObj->isDamaged = sfTrue;
		//				}
		//				break;
		//			case TYPE_02:
		//				if (CollisionCircleRectangle(enemyObj->circleHitbox, weaponData.hitbox[shield]))
		//				{
		//					enemyObj->isDamaged = sfTrue;
		//				}
		//				break;
		//			case TYPE_03:
		//				if (IsColliding2Rectangles(weaponhitbox, enemyRectangleHitbox))
		//				{
		//					enemyObj->isDamaged = sfTrue;
		//				}
		//			default:
		//				break;
		//			}
		//		}
		//	}
	//}
}

void IncreaseSizeWeapon()
{
	weaponData.weaponScale += 1;
	weaponData.weaponScale %= 3;
	SetWeaponScale(weaponData.weaponScale);
	SetIdleAnimation();
	weaponData.isSwapingWeapon = sfTrue;
}

WeaponScale GetSizeWeapon()
{
	return weaponData.weaponScale;
}

void SetSizeWeapon(int _size)
{
	weaponData.weaponScale = _size % 3;
	SetWeaponScale(weaponData.weaponScale);
	SetIdleAnimation();
}

void SetAttackHitbox(void)
{
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
	sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
	sfBool isPlayerRight = GetPlayer()->direction;
	sfVector2f position = { 0 };

	float sizeX = 0;
	float sizeY = 0;

	//Set the hitbox Size
	if (weaponData.weaponType == SWORD)
	{
		switch (weaponData.weaponScale)
		{
		case STRETCH_SMALL:
			//sfRectangleShape_setSize(weaponData.hitbox[0], (sfVector2f) { SWORD_NORMAL_SIZE_X, SWORD_NORMAL_SIZE_Y });
			//position.x += SWORD_HORIZONTAL_POS_X;
			//position.y += SWORD_HORIZONTAL_POS_Y;
			sizeX = SWORD_NORMAL_SIZE_X;
			sizeY = SWORD_NORMAL_SIZE_Y;
			break;
		case STRETCH_MEDIUM:
			//sfRectangleShape_setSize(weaponData.hitbox[0], (sfVector2f) { SWORD_HORIZONTAL_SIZE_X, SWORD_HORIZONTAL_SIZE_Y });
			//position.x += SWORD_MEDIUM_POS_X;
			//position.y += SWORD_MEDIUM_POS_Y;
			sizeX = SWORD_HORIZONTAL_SIZE_X;
			sizeY = SWORD_HORIZONTAL_SIZE_Y;

			break;
		case STRETCH_BIG:
			//sfRectangleShape_setSize(weaponData.hitbox[0], (sfVector2f) { SWORD_VERTICAL_SIZE_X, SWORD_VERTICAL_SIZE_Y });
			sizeX = SWORD_VERTICAL_SIZE_X;
			sizeY = SWORD_VERTICAL_SIZE_Y;
			break;
		default:
			sizeX = 0;
			sizeY = 0;
			break;
		}
	}


	sfRectangleShape_setSize(weaponData.hitbox[0], (sfVector2f) { sizeX, sizeY });
	sfRectangleShape_setOrigin(weaponData.hitbox[0], (sfVector2f) { 0, sizeY });
	//sfRectangleShape_setPosition(weaponData.hitbox[0], playerPos);

	//Set the hitbox position
	if (isPlayerRight)
	{
		sfRectangleShape_setPosition(weaponData.hitbox[0], (sfVector2f) { playerHitbox.left + playerHitbox.width / 2, playerHitbox.top + +playerHitbox.height });
		sfRectangleShape_setScale(weaponData.hitbox[0], (sfVector2f) { 1, 1 });
	}
	else
	{
		sfRectangleShape_setPosition(weaponData.hitbox[0], (sfVector2f) { playerHitbox.left + playerHitbox.width / 2, playerHitbox.top + playerHitbox.height });
		sfRectangleShape_setScale(weaponData.hitbox[0], (sfVector2f) { -1, 1 });
	}
	//sfRectangleShape_setPosition(weaponData.hitbox[0], position);
}



void SetAttackWeapon(void)
{
	weaponData.isAttacking = sfTrue;
}

void ResetWeapon(void)
{
	SetIdleAnimation();
	ResetAttackHitbox();
	weaponData.isAttacking = sfFalse;

	//for (int i = 0; i < GetEnemyData()->list->count; i++)
	//{
	//	EnemyObj* enemyObj = GetElement(GetEnemyData()->list, i)->data;

	//	enemyObj->hasBeenHit = sfFalse;

	//}
}

const WeaponData* GetWeapon(void)
{
	return &weaponData;
}

sfBool IsWeaponAttacking()
{
	return weaponData.isAttacking;
}