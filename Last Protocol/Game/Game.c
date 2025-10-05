#include "Game.h"

void DrawGameHitbox(sfRenderWindow* _window);
void CollisionEnemyPlayer(void);
void CollisionEnemyWeapon(void);
void CheckMiscBasedOnWeapon(sfVector2f _startPosition);
void DrawPropsUpfront(sfRenderWindow* _windows);
void CollisionPlayerCrossedDoor(sfRenderWindow* _window);
void CollisionPlayerProps(float _dt);

void CollisionPlayerBullet(void);
void CollisionBulletEnvironment(void);

void LoadTextLevelIntro(void);

void UpdateTextLevelIntro(sfRenderWindow* _window, float _dt);

void DrawTextLevelIntro(sfRenderWindow* _window);

void CleanupTextIntro(void);

GameData game;

void LoadGame(LevelNumber _levelNumber)
{
	fileSave += fileCollected;
	fileCollected = 0;
	game.actualLevel = _levelNumber;
	LoadLevel(_levelNumber);
	LoadPickup();
	LoadBumper();
	LoadLaser();
	LoadDoor();
	LoadSign();

	CombineTextBoxWithText();

	LoadBulletTime();
	LoadPlayer();
	LoadEnemy();
	LoadMisc();
	LoadCamera();
	LoadBackground(_levelNumber);
	LoadBullet();
	LoadHud();
	LoadWeapon();
	LoadAllSound();
	LoadProps(_levelNumber);
	LoadPauseMenu();

	LoadBoss();
	LoadHUDboss();
	//AddFilesAllOfLvl();

	LoadTextLevelIntro();

	game.isPausing = sfFalse;
}

void MouseMovedGame(sfMouseMoveEvent _mouseMove)
{
	game.mousePosition = (sfVector2f){ (float)_mouseMove.x, (float)_mouseMove.y };
}

void UpdateGame(float _dt, sfRenderWindow* _window)
{
	if (!game.pause)
	{
		crono += _dt;
		game.textTimer += _dt;
		UpdateLevel(_dt);
		if (GetPlayerBulletTime())
		{
			_dt /= BULLET_TIME_SLOW;
		}
		UpdatePlayer(_window, _dt);
		UpdateBulletTimeAnimation(_dt, _window);

		sfVector2f playerPos = sfRectangleShape_getPosition(GetPlayer()->hitbox);

		UpdateCamera(_window, playerPos, _dt);
		UpdateBackground(_window, _dt);
		UpdatePickup(_dt);
		UpdateLaser(_dt);
		UpdateBumper(_dt);
		UpdateDoor(_dt);
		UpdateEnemy(_dt);
		UpdateBullet(_dt);
		UpdateWeapon(_dt);
		UpdateSign(_dt);

		//UpdateTime(_dt);
		UpdateTextLevelIntro(_window, _dt);


		CollisionEnemyPlayer();
		UpdateHud(_dt, _window);
		CollisionEnemyWeapon();

		CollisionPlayerProps(_dt);
		UpdateProps(_dt);

		CollisionPlayerBullet();
		CollisionBulletEnvironment();
		UpdateMisc(_dt);

		if (game.actualLevel == LEVEL_3)
		{
			UpdateBoss(_dt);
			UpdateHUDboss(_dt);
		}
	}
	if (game.pause)
	{
		UpdatePauseMenu(_dt, _window);
		switch (GetButtonClicked())
		{
		case CROSS_PAUSE:
			game.pause = !game.pause;
			ResetClickedButton();
			break;
		case CONTINUE_BUTTON_PAUSE:
			game.pause = !game.pause;
			ResetClickedButton();
			break;
		case QUIT_PAUSE:
			game.pause = !game.pause;
			ResetClickedButton();
			PlayMusic(MAIN_MENU1, sfTrue, sfFalse);
			LoadMenu();
			SetGameState(MENU);
			break;
		default:
			break;
		}
	}

	if (CheckMapping(GetMapping()->pause) && game.isPausing == sfFalse)
	{
		LoadPauseMenu();
		game.pause = !game.pause;
	}

	game.isPausing = CheckMapping(GetMapping()->pause);
	UpdateMusic(_dt);
	UpdateSound(_dt);
	CollisionPlayerCrossedDoor(_window);
}

void DrawGame(sfRenderWindow* _window)
{
	//sfFloatRect backgroundRect = sfSprite_getGlobalBounds(game.spriteBackground);

	//if (playerPos.x - SCREEN_WIDTH / 2 >= 0 && playerPos.x + SCREEN_WIDTH / 2 <= backgroundRect.width)
	//{

	//}

	//sfRenderWindow_drawSprite(_window, game.spriteBackground, NULL);

	DrawBackground(_window);

	DrawLevelBehind(_window);

	if (GetLevel()->number == 3)
	{
		DrawBoss(_window);
	}
	//DrawLine(_window);
	//DrawMiscGame(_window);
	DrawProps(_window);
	DrawEnemy(_window);
	DrawBulletTimeAnimation(_window);
	DrawBullet(_window);
	DrawBumper(_window);
	DrawLaser(_window);
	DrawDoor(_window);
	DrawPlayer(_window);
	DrawSignPedestral(_window);
	DrawPickup(_window);
	DrawPropsUpfront(_window);
	DrawSign(_window);
	DrawLevelBefore(_window);
	DrawWeapon(_window);

	//DrawCamera(_window);
	DrawMisc(_window);


	DrawHud(_window);

	DrawObtainedPickup(_window);
	DrawGameHitbox(_window);


	if (GetLevel()->number == 3)
	{
		DrawHUDboss(_window);
	}

	if (game.pause)
	{
		DrawPauseMenu(_window);
	}

	DrawTextLevelIntro( _window);

	DrawSettingsMenu(_window);


}

void DrawGameHitbox(sfRenderWindow* _window)
{
	if (game.drawHitbox)
	{
		DrawPlayerHitbox(_window);
		DrawEnemyHitbox(_window);
		DrawPropsHitbox(_window);
		DrawBulletHitbox(_window);

	}
	if (game.drawOrigin)
	{
		DrawPlayerHitboxOrigin(_window);
		DrawEnemyHitboxOrigin(_window);
		DrawMiscOrigin(_window);

	}
}

void CleanupGame(sfRenderWindow* _window)
{
	fileSave += fileCollected;

	CleanupCamera(_window);
	CleanupBackground();
	//CleanupLine();
	CleanupPlayer();
	CleanupSign();
	CleanupLaser();
	CleanupBumper();
	CleanupDoor();
	CleanupBullet();
	CleanupMisc();
	CleanupProps();
	CleanupLevel();
	CleanupPauseMenu();
	CleanupBoss();
	CleanupWeapon();
	CleanupHUDBoss();

	CleanupSound();
	CleanupTextIntro();
	LevelNumber temp = game.actualLevel;
	game = (GameData){ 0 };
	game.actualLevel = temp;
}

void KeypressedGame(sfRenderWindow* _window, sfKeyEvent _key)
{
	KeypressedPlayer(_key);

	if (_key.code == sfKeyH)
	{
		game.drawHitbox = game.drawHitbox ? sfFalse : sfTrue;
	}
	else if (_key.code == sfKeyO)
	{
		game.drawOrigin = game.drawOrigin ? sfFalse : sfTrue;
	}
	else if (_key.code == sfKeyT)
	{
		sfVector2f position = sfSprite_getPosition(GetPlayer()->sprite);
		AddBullet(BULLET_BASIC, position, game.mousePosition);
	}
	else if (_key.code == sfKeyEnter)
	{
		CleanupGame(_window);
		LoadGame(game.actualLevel);
	}
	else if (_key.code == sfKeyG)
	{
		//CleanupGame();
		SetGameState(GAME_OVER);
	}
	else if (_key.code == sfKeyNumpad7)
	{
		//CleanupGame();
		SetGameState(WIN);
	}
	else if (_key.code == sfKeyNumpad1)
	{
		CleanupGame(_window);
		LoadGame(LEVEL_1);
	}
	else if (_key.code == sfKeyNumpad2)
	{
		CleanupGame(_window);
		LoadGame(LEVEL_2);
	}
	else if (_key.code == sfKeyNumpad3)
	{
		CleanupGame(_window);
		LoadGame(LEVEL_3);
	}
	else if (_key.code == sfKeyB)
	{
		SetLevel()->layer[ENTITY].invisible = GetLevel()->layer[ENTITY].invisible ? sfFalse : sfTrue;
	}
	else if (_key.code == sfKeyNumpad5)
	{
		sfVector2f position = sfSprite_getPosition(GetPlayer()->sprite);
		SetRandomCamShake(10, 1, 2);
	}
	//else if (_key.code == sfKeyEscape)
	//{
	//	if (game.isPausing)
	//	{
	//		//SetGameState(MENU);
	//	}
	//}
}

void CollisionEnemyPlayer(void)
{
	if (GetPlayer()->timerInvinsible == 0)
	{
		List* enemyList = GetEnemy()->list;

		for (unsigned i = 0; i < enemyList->count; i++)
		{
			EnemyObj* enemy = GetElement(enemyList, i)->data;
			if (enemy->state != ENEMY_DEAD)
			{
				sfFloatRect enemyHb = sfRectangleShape_getGlobalBounds(enemy->hitbox);
				sfFloatRect playerHb = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);

				CollisionData col = CollisionRectanglesOffset(enemyHb, playerHb);
				EnemyBehavior* enemyBehavior = &GetEnemy()->behaviorList[enemy->type];

				if (col.direction == COLLISION_UP && !enemyBehavior->explode)
				{
					if (!enemy->isInvincible)
					{
						AddMisc(MISC_JUMP_ENEMY, sfRectangleShape_getPosition(enemy->hitbox));
						PlaySound(ENEMY_JUMP_ON, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
						SetRandomCamShake(10, 10, 0.2f);
					}
					EditEnemyHP(enemy, -1);
					if (enemy->life <= 0)
					{
						PlayEnemyDeathSound(enemy->type);
					}
					SetPlayer()->nbJump = NB_JUMP;
					SetPlayer()->velocity.y = PLAYER_IMPULSE;
					//AddMisc(MISC_COLLISION_ENEMY, sfRectangleShape_getPosition(enemy->hitbox));
				}
				else if (col.direction != COLLISION_NONE)
				{
					EnemyStatistic* enemyStat = &GetEnemy()->statsList[enemy->type];
					EditPlayerHP(-enemyStat->herCrashDamage);
					EditEnemyHP(enemy, -enemyStat->myCrashDamage);
					AddMisc(MISC_JUMP_ENEMY, sfRectangleShape_getPosition(enemy->hitbox));
					sfVector2f enemyPos = { enemyHb.left + enemyHb.width / 2, enemyHb.top + enemyHb.height / 2 };
					sfVector2f playerPos = { playerHb.left + playerHb.width / 2, playerHb.top + playerHb.height / 2 };
					PlayerKnockback(KNOCKBACK_POWER, GetAngle(enemyPos, playerPos));
					SetRandomCamShake(5, 10, 0.2f);
				}
			}
		}
	}
}

void CollisionEnemyWeapon(void)
{
	List* enemyList = GetEnemy()->list;
	for (unsigned i = 0; i < enemyList->count; i++)
	{
		EnemyObj* enemy = GetElement(enemyList, i)->data;

		if (!enemy->isInvincible)
		{

			sfFloatRect enemyHb = sfRectangleShape_getGlobalBounds(enemy->hitbox);
			sfFloatRect weaponHb = sfRectangleShape_getGlobalBounds(GetWeaponHitbox(0));

			if (IsColliding2Rectangles(enemyHb, weaponHb) && !enemy->isInvincible && GetWeapon()->isAttacking)
			{
				sfVector2f enemyPos = sfRectangleShape_getPosition(enemy->hitbox);

				int weaponSize = GetSizeWeapon();

				sfVector2f enemyCenterPos = { enemyHb.left + enemyHb.width / 2, enemyHb.top + enemyHb.height / 2 };
				sfVector2f weaponPos = { weaponHb.left + weaponHb.width / 2, weaponHb.top + weaponHb.height / 2 };

				switch (weaponSize)
				{
				case STRETCH_SMALL: 
					EditEnemyHP(enemy, -SWORD_SMALL_DAMAGE);
					EnemyKnockback(enemy, KNOCKBACK_SMALL, GetAngle(weaponPos, enemyCenterPos));
					SetRandomCamShake(5, 1, 0.2f);

					break;
				case STRETCH_MEDIUM: 
					EditEnemyHP(enemy, -SWORD_MEDIUM_DAMAGE);
					EnemyKnockback(enemy, KNOCKBACK_MEDIUM, GetAngle(weaponPos, enemyCenterPos));
					SetRandomCamShake(10, 1, 0.3f);

					break;
				case STRETCH_BIG: 
					EditEnemyHP(enemy, -SWORD_BIG_DAMAGE);
					EnemyKnockback(enemy, KNOCKBACK_BIG, GetAngle(weaponPos, enemyCenterPos));
					SetRandomCamShake(15, 1, 0.3f);
					break;
				default:
					break;
				}

				if (enemy->life <= 0)
				{
					PlayEnemyDeathSound(enemy->type);
				}
				else
				{
					PlayEnemyHitSound(enemy->type);
				}

				CheckMiscBasedOnWeapon(enemyPos);
				if (enemy->turn == ENEMY_TURN_RIGHT)
				{
					FlipMisc();
				}
			}
		}
	}
}

void CheckMiscBasedOnWeapon(sfVector2f _startPosition)
{
	WeaponData* weapon = GetWeapon();
	if (weapon->weaponScale == STRETCH_SMALL)
	{
		AddMisc(MISC_ATTACK_BASIC, _startPosition);
	}
	else if (weapon->weaponScale == STRETCH_MEDIUM)
	{
		AddMisc(MISC_ATTACK_MEDIUM, _startPosition);
	}
	else if (weapon->weaponScale == STRETCH_BIG)
	{
		AddMisc(MISC_ATTACK_ULTIMATE, _startPosition);
	}
}

void DrawPropsUpfront(sfRenderWindow* _windows)
{
	for (unsigned i = 0; i < GetProps()->list->count; i++)
	{
		PropsObj* props = GetElement(GetProps()->list, i)->data;

		if (props->isUpFront)
		{
			DrawAnimation(&props->animationList[props->type], _windows);
		}
	}
}

void CollisionPlayerCrossedDoor(sfRenderWindow* _window)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);

	for (unsigned i = 0; i < GetDoorNb(); i++)
	{
		sfFloatRect doorCrossedHitbox = GetDoorCrossedHitbox(i);
		if (IsColliding2Rectangles(hitbox, doorCrossedHitbox))
		{
			SetPlayer()->velocity = (sfVector2f){ 0 };
			if (GetTransitionState(READY))
			{
				StartTransitionPicture(sfWhite, 2, 10);
			}
			else if (GetTransitionState(COVER_SCREEN))
			{
				CleanupGame(_window);
				LoadGame(game.actualLevel + 1);
			}
		}
	}
}

void CollisionPlayerProps(float _dt)
{
	sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);

	List* propList = GetProps()->list;
	for (unsigned i = 0; i < propList->count; i++)
	{
		PropsObj* prop = GetElement(propList, i)->data;

		sfVector2f playerVelocity = GetPlayer()->velocity;

		if (prop->isInteractible && (playerVelocity.x != 0 || (playerVelocity.y > 0 && playerVelocity.y < 20 )))
		{
			sfFloatRect propHitbox = sfRectangleShape_getGlobalBounds(prop->hitbox);
			if (IsColliding2Rectangles(playerHitbox, propHitbox))
			{

				if (prop->animationList[prop->type].isFinished)
				{
					sfVector2f pos = sfSprite_getPosition(GetPlayer()->sprite);
					if (prop->type == PROPS_GRASS_1 || prop->type == PROPS_GRASS_2)
					{
						PlaySound(GRASS_SOUND, pos, sfFalse, sfFalse);
					}
					else if (prop->type == PROPS_VINE_1 || prop->type == PROPS_VINE_2 || prop->type == PROPS_VINE_3)
					{
						PlaySound(LIANA_SOUND, pos, sfFalse, sfFalse);
					}
					LoadAnimation(&prop->animationList[prop->type]);
				}
			}
		}
	}
}

void CollisionPlayerBullet(void)
{
	if (GetPlayer()->timerInvinsible == 0)
	{
		sfFloatRect playerHitbox = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
		List* bulletList = GetBullet()->list;

		for (unsigned i = 0; i < bulletList->count; i++)
		{
			BulletObj* bullet = GetElement(bulletList, i)->data;

			sfFloatRect bulletHb = sfRectangleShape_getGlobalBounds(bullet->hitbox);
			sfVector2f bulletPosition = sfRectangleShape_getPosition(bullet->hitbox);

			if (IsCollidingPointRectangle(bulletPosition, playerHitbox))
			{
				if (!bullet->isExploded)
				{
					EditPlayerHP(-1);
					bullet->isExploded = sfTrue;

					if (bullet->type == BULLET_BOSS)
					{
						AddMisc(MISC_EXPLOSION_BOSS, bulletPosition);
					}
					else
					{
						AddMisc(MISC_EXPLOSION, bulletPosition);
						RotateMisc(bullet->sprite);
					}

					sfFloatRect bulletHb = sfRectangleShape_getGlobalBounds(bullet->hitbox);
					sfFloatRect playerHb = sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox);
					sfVector2f bulletPos = { bulletHb.left + bulletHb.width / 2, bulletHb.top + bulletHb.height / 2 };
					sfVector2f playerPos = { playerHb.left + playerHb.width / 2, playerHb.top + playerHb.height / 2 };
					PlayerKnockback(KNOCKBACK_POWER, GetAngle(bulletPos, playerPos));
					DestroyBullet(GetBullet()->list, i);
					SetRandomCamShake(5, 10, 0.2f);

				}
			}
			sfRectangleShape_setRotation(bullet->hitbox, sfSprite_getRotation(bullet->sprite));
		}
	}
}

void CollisionBulletEnvironment(void)
{
	List* bulletList = GetBullet()->list;

	for (unsigned i = 0; i < bulletList->count; i++)
	{
		BulletObj* bullet = GetElement(bulletList, i)->data;

		sfVector2f bulletPos = sfRectangleShape_getPosition(bullet->hitbox);
		CellCoordinates bulletCoordinates = GetLevelColumnLineWithPosition((sfVector2f)
		{
			bulletPos.x, bulletPos.y
		});

		sfBool isCollding = GetLevelCollide(bulletCoordinates);

		if (isCollding)
		{
			if (!bullet->isExploded)
			{
				sfVector2f bulletPosition = sfRectangleShape_getPosition(bullet->hitbox);
				bullet->isExploded = sfTrue;

				if (bullet->type == BULLET_BOSS)
				{
					AddMisc(MISC_EXPLOSION_BOSS, sfSprite_getPosition(bullet->sprite));
				}
				else
				{
					AddMisc(MISC_EXPLOSION, sfSprite_getPosition(bullet->sprite));
					RotateMisc(bullet->sprite);
				}

				DestroyBullet(GetBullet()->list, i);
			}
		}
		sfRectangleShape_setRotation(bullet->hitbox, sfSprite_getRotation(bullet->sprite));
	}
}

void LoadTextLevelIntro(void)
{
	game.levelFont = sfFont_createFromFile("Assets/Fonts/GlobalFont.ttf", NULL);
	game.levelText[TEXT_LEVEL_1] = CreateText(game.levelFont, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, sfWhite, sfTrue, "\tLEVEL 1\nDISK DRIVE");
	game.levelText[TEXT_LEVEL_2] = CreateText(game.levelFont, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, sfWhite, sfTrue, "\tLEVEL 2\n PROCESSOR");
	game.levelText[TEXT_LEVEL_3] = CreateText(game.levelFont, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, sfWhite, sfTrue, "LEVEL ERROR\n\tUNKNOWN");
}

void UpdateTextLevelIntro(sfRenderWindow* _window, float _dt)
{

	for (unsigned i = 0; i < NB_TEXT_LEVEL; i++)
	{
		LinkTextToCamera(game.levelText[i], _window, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	}
}

void DrawTextLevelIntro(sfRenderWindow* _window)
{
	if (game.textTimer < TEXT_TIME)
	{
		switch (GetLevel()->number)
		{
		case 1:
			sfRenderWindow_drawText(_window, game.levelText[TEXT_LEVEL_1], NULL);
			break;
		case 2:
			sfRenderWindow_drawText(_window, game.levelText[TEXT_LEVEL_2], NULL);
			break;
		case 3:
			sfRenderWindow_drawText(_window, game.levelText[TEXT_LEVEL_3], NULL);
			break;
		}

	}
}

void CleanupTextIntro(void)
{
	for (unsigned i = 0; i < NB_TEXT_LEVEL; i++)
	{
		sfText_destroy(game.levelText[i]);
		game.levelText[i] = NULL;
	}
}


