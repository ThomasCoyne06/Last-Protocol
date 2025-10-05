#include "Player.h"

#pragma region Statement
void LoadPlayerAnimation(void);
void LoadPlayerHitbox(void);


void UpdatePlayerGravity(float _dt);
void UpdatePlayerSpriteAndHitbox(void);
void UpdatePlayerState(void);
void UpdatePlayerTurnAround(void);
void UpdateWeaponSize();
void PlayerMoveSide(float _dt);
void PlayerMoveSideLeft(float _dt);
void PlayerMoveSideRight(float _dt);
void PlayerSlowInAir(float _dt);
void UpdatePlayerJump(float _dt);
void UpdatePlayerCooldown(float _dt);
void UpdateBulletTime(float _dt);
void UpdateEnergy(float _dt);
void CollisionPlayerLevelBorders(void);
void UpdateSlidingCondition();

void PlayerDash(float _dt);
void PlayerSliding(float _dt);
void PlayerSlow(float _dt);
void PlayerAcceleration(float _dt);
void DrawPlayerHitbox(sfRenderWindow* _window);

void CollisionPlayerPickup(void);
void CollisionPlayerDoor(void);
void CollisionPlayerText(void);
void CollisionPlayerLaser();
void CollisionPlayerBumper();
void CollisionPlayerLevelVerti(void);
void CollisionPlayerLevelHoriz(void);
void CollisionPlayerSecretZone(void);

void UpdateOnGroundTimer(float);

sfBool IsRunning(void);
void SetPlayerChooseAttack(void);
void SetPlayerStateAttack(int _index);

void LoadThunder();
void UpdateThunder();
void DrawThunder(sfRenderWindow* _window);
sfRectangleShape* placeHolder;

#pragma endregion

PlayerData player;

void LoadPlayer(void)
{
	player.texture = sfTexture_createFromFile("Assets/Textures/Game/Player/Spritesheet-PP-V11.png", NULL);

	sfVector2f position = GetLevel()->entityList[PLAYER_SPAWN].positionList[0];
	//sfVector2f position = { SCREEN_WIDTH / 2, -SCREEN_HEIGHT * 2 };
	player.sprite = CreateSprite(player.texture, position, sfFalse, SCALE);

	player.nbDash = 1;
	player.distanceDash = 0;
	player.timerSlide = SPEED_SLIDE;
	player.direction = PLAYER_RIGHT;
	player.IsAttackingPressed = sfFalse;
	player.HealthPoint = MAX_HEALTH_POINTS;
	player.energy = ENERGY_MAX;
	player.timeBulletTime = BULLET_MAX_TIMER;
	player.isActif = sfTrue;
	player.isAlive = sfTrue;
	player.cheatMod = sfFalse;

	LoadPlayerAnimation();
	LoadPlayerHitbox();


	//LoadThunder();

}
void LoadPlayerAnimation(void)
{

	// 
	sfIntRect firstFrame = { -5, IDLE_ANIM_START,PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[IDLE] = CreateAnimation(player.sprite, 13, 10, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ -5, RUN_ANIM_START,PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[RUN] = CreateAnimation(player.sprite, 11, FRAME_RATE, firstFrame, sfTrue);

	firstFrame = (sfIntRect){ -5, JUMP_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[JUMP] = CreateAnimation(player.sprite, 6, 18, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ -5, SLIDING_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[SLIDING] = CreateAnimation(player.sprite, 4, FRAME_RATE, firstFrame, sfFalse);
	//firstFrame = (sfIntRect){ 0, DEAD_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	//player.animation[DAMAGE]= CreateAnimation(player.sprite, 9, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ -5, SLIDING_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[SLIDING] = CreateAnimation(player.sprite, 4, FRAME_RATE, firstFrame, sfFalse);
	/*for (int i = 4; i < NB_STATE - 3; i++)
	{
		player.animation[i] = player.animation[IDLE];
	}*/
	player.state = IDLE;
	firstFrame = (sfIntRect){ 0, DEAD_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[DAMAGE] = CreateAnimation(player.sprite, 9, 15, firstFrame, sfFalse);


	firstFrame = (sfIntRect){ 0, DASH_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[DASHING] = CreateAnimation(player.sprite, 8, FRAME_RATE * 3 /** (DASH_SPEED / TRAVELLED_DISTANCE)*/, firstFrame, sfTrue);

	firstFrame = (sfIntRect){ 0, DEAD_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[DEAD] = CreateAnimation(player.sprite, 7, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ -5, JUMP_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animation[KNOCKBACK] = CreateAnimation(player.sprite, 6, 18, firstFrame, sfFalse);





	firstFrame = (sfIntRect){ 0,  STATIC_ATTACK_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animationAttack[0] = CreateAnimation(player.sprite, 4, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ 0, MOVING_ATTACK_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animationAttack[1] = CreateAnimation(player.sprite, 4, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ 0, JUMPING_ATTACK_ANIM_START , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animationAttack[2] = CreateAnimation(player.sprite, 3, FRAME_RATE, firstFrame, sfFalse);

	//Medium

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 4,  STATIC_ATTACK_MEDIUM , PLAYER_WIDTH ,PLAYER_HEIGHT };
	player.animationAttack[3] = CreateAnimation(player.sprite, 5, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 6,MOVING_ATTACK_MEDIUM,PLAYER_WIDTH,PLAYER_HEIGHT };
	player.animationAttack[4] = CreateAnimation(player.sprite, 8, FRAME_RATE * 2, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 1,JUMPING_ATTACK_MEDIUM,PLAYER_WIDTH,PLAYER_HEIGHT };
	player.animationAttack[5] = CreateAnimation(player.sprite, 3, FRAME_RATE, firstFrame, sfFalse);

	//Big

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 4,STATIC_ATTACK_BIG,PLAYER_WIDTH,PLAYER_HEIGHT };
	player.animationAttack[6] = CreateAnimation(player.sprite, 5, FRAME_RATE, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ PLAYER_WIDTH * 5,MOVING_ATTACK_BIG,PLAYER_WIDTH,PLAYER_HEIGHT };
	player.animationAttack[7] = CreateAnimation(player.sprite, 9, FRAME_RATE * 2, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ 0,JUMPING_ATTACK_BIG,PLAYER_WIDTH,PLAYER_HEIGHT };
	player.animationAttack[8] = CreateAnimation(player.sprite, 4, FRAME_RATE, firstFrame, sfFalse);

	player.currentAnimation = &player.animation[player.state];




	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
}

void LoadPlayerHitbox(void)
{
	sfVector2f position = GetLevel()->entityList[PLAYER_SPAWN].positionList[0];

	sfFloatRect rect = { position.x, position.y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT };
	player.hitbox = CreateHitbox(rect, sfFalse, sfRed, SCALE);
	sfRectangleShape_setOrigin(player.hitbox, (sfVector2f) { rect.width / 2, rect.height });
}

void UpdatePlayer(sfRenderWindow* _window, float _dt)
{

	if (player.isActif)
	{

		UpdatePlayerGravity(_dt);
		sfRectangleShape_move(player.hitbox, (sfVector2f) { 0, player.velocity.y* _dt });
		UpdateOnGroundTimer(_dt);
		CollisionPlayerLevelVerti();

		sfRectangleShape_move(player.hitbox, (sfVector2f) { player.velocity.x* _dt, 0 });
		CollisionPlayerLevelHoriz();

		UpdatePlayerSpriteAndHitbox();

		sfJoystick_update();

		UpdateAnimation(player.currentAnimation, _dt);

		UpdatePlayerState();

		switch (player.state)
		{
		case IDLE:
			PlayerMoveSide(_dt);
			UpdatePlayerTurnAround();
			break;

		case RUN:
			PlayerMoveSide(_dt);
			UpdatePlayerTurnAround();
			break;

		case JUMP:
			PlayerMoveSide(_dt);
			UpdatePlayerTurnAround();
			UpdatePlayerJump(_dt);
			break;

		case FALL:
			PlayerMoveSide(_dt);
			UpdatePlayerJump(_dt);
			UpdatePlayerTurnAround();
			break;

		case STATIC_ATTACK:
			PlayerSlow(_dt);
			UpdatePlayerJump(_dt);
			break;

		case MOVING_ATTACK:
			PlayerMoveSide(_dt);
			UpdatePlayerJump(_dt);
			break;

		case JUMPING_ATTACK:
			PlayerMoveSide(_dt);
			UpdatePlayerJump(_dt);
			break;

		case SLIDING:
			PlayerSliding(_dt);
			break;

		case DASHING:
			PlayerDash(_dt);
			UpdatePlayerJump(_dt);
			break;

		case CRAWLING:
			PlayerMoveSide(_dt);
			break;
		case DAMAGE:
			PlayerMoveSide(_dt);
			UpdatePlayerTurnAround();
			UpdatePlayerJump(_dt);

		case DEAD:
			//PlayerMoveSide(_dt);
			//UpdatePlayerTurnAround();
			//UpdatePlayerJump(_dt);
			break;
		}


		CollisionPlayerLevelBorders();
		CollisionPlayerPickup();
		CollisionPlayerText();
		CollisionPlayerLaser();
		CollisionPlayerDoor();
		CollisionPlayerBumper();
		CollisionPlayerSecretZone();

	}
	UpdateSlidingCondition();
	UpdatePlayerCooldown(_dt);
	UpdateBulletTime(_dt);

	UpdateWeaponSize();
	UpdateEnergy(_dt);

	if (player.HealthPoint <= 0 && player.cheatMod == sfFalse)
	{
		player.isAlive = sfFalse;
	}
	if (IsPlayerAlive() == sfFalse && AnimationIsFinished(&player.animation[DEAD]))
	{
		if (GetTransitionState(READY))
		{
			StartTransitionPicture(sfWhite, 2, 10);
		}
		else if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(GAME_OVER);
		}
	}
}

void UpdatePlayerGravity(float _dt)
{
	if (player.state != DASHING && player.enableGravity)
	{
		if (player.velocity.y < GRAVITY_LIMIT)
		{
			// the first "if" is used to increase the size of the jump when you keep pressed the jump 
			if (player.isJumping && player.velocity.y < 0)
			{
				player.velocity.y += (GRAVITY * _dt) / GRAVITY_REDUCE;
			}
			else
			{
				player.velocity.y += (GRAVITY * _dt);
			}

		}
		else
		{
			player.velocity.y = GRAVITY_LIMIT;
		}
	}
}

void UpdatePlayerSpriteAndHitbox(void)
{
	sfVector2f position = sfRectangleShape_getPosition(player.hitbox);
	sfSprite_setPosition(player.sprite, position);

	sfVector2f hitboxSize = sfRectangleShape_getSize(player.hitbox);
	if (player.state != DASHING)
	{
		//player.hitbox.width = PLAYER_HITBOX_WIDTH;
		//player.hitbox.height = PLAYER_HITBOX_HEIGHT;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
		//sfVector2f hitboxSize = sfRectangleShape_getSize(player.hitbox);
	}

	switch (player.state)
	{
	case IDLE:
		hitboxSize.x = PLAYER_HITBOX_WIDTH;
		hitboxSize.y = PLAYER_HITBOX_HEIGHT;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
		break;

	case RUN:
		hitboxSize.x = PLAYER_HITBOX_WIDTH;
		hitboxSize.y = PLAYER_HITBOX_HEIGHT;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
		break;

	case JUMP:
		hitboxSize.x = PLAYER_HITBOX_WIDTH;
		hitboxSize.y = HITBOX_JUMP_HEIGHT;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
		break;

	case JUMPING_ATTACK:
		hitboxSize.x = PLAYER_HITBOX_WIDTH;
		hitboxSize.y = HITBOX_JUMP_HEIGHT;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y });
		break;

	case SLIDING:
		hitboxSize.x = PLAYER_HITBOX_WIDTH * 2;
		hitboxSize.y = TILE_HEIGHT - 5;
		sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, ORIGIN_Y /*+ 1*/ });
		break;
		/*case DASHING:
			if (!player.onGround)
			{
				player.hitbox.width = PLAYER_HITBOX_WIDTH;
				player.hitbox.height = HITBOX_JUMP_HEIGHT;
				sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, 62 });
			}
			else
			{
				player.hitbox.width = PLAYER_HITBOX_WIDTH * 2;
				player.hitbox.height = CELL_HEIGHT - 5;
				sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, 71 });
			}
			break;*/
	}
	sfRectangleShape_setOrigin(player.hitbox, (sfVector2f) { hitboxSize.x / 2, hitboxSize.y });
	sfRectangleShape_setSize(player.hitbox, hitboxSize);
}

void UpdatePlayerState(void)
{
	if (player.isAlive)
	{

		switch (player.state)
		{
		case IDLE:
			if (player.canDash && CheckMapping(GetMapping()->dashing))
			{
				SetPlayerState(DASHING);
			}
			else if (CheckMapping(GetMapping()->jump) && player.canJump || player.IsBumped == sfTrue)
			{
				SetPlayerState(JUMP);
			}
			else if (IsRunning() && !player.collisionHoriz)
			{
				SetPlayerState(RUN);
			}

			else if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
			{
				SetPlayerChooseAttack();
			}
			break;
		case RUN:

			if (player.canDash && CheckMapping(GetMapping()->dashing))
			{
				SetPlayerState(DASHING);
			}
			else if (CheckMapping(GetMapping()->down) && player.canSlide)
			{
				SetPlayerState(SLIDING);
			}
			else if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
			{
				SetPlayerChooseAttack();
			}
			else if (CheckMapping(GetMapping()->jump) && player.canJump || player.IsBumped == sfTrue)
			{
				SetPlayerState(JUMP);
			}
			else if (!IsRunning() || player.collisionHoriz)
			{
				SetPlayerState(IDLE);
			}
			break;
		case JUMP:
			if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
			{
				SetPlayerChooseAttack();
			}
			else if (CheckMapping(GetMapping()->down) && player.canSlide)
			{
				SetPlayerState(SLIDING);
			}
			else if (player.canDash && CheckMapping(GetMapping()->dashing))
			{
				SetPlayerState(DASHING);
			}
			else if ((CheckMapping(GetMapping()->jump) && player.canJump) || player.IsBumped == sfTrue)
			{

				SetPlayerState(JUMP);


			}
			else if (player.onGround && IsRunning())
			{
				SetPlayerState(RUN);
			}
			else if (player.onGround && !IsRunning())
			{
				SetPlayerState(IDLE);
			}

			break;
		case FALL:
			if (player.onGround)
			{
				SetPlayerState(IDLE);
			}
			else if (CheckMapping(GetMapping()->jump) && player.canJump)
			{
				SetPlayerState(JUMP);
			}
			else if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
			{
				SetPlayerChooseAttack();
			}
			else if (CheckMapping(GetMapping()->down))
			{
				SetPlayerState(SLIDING);
			}
			else if (player.canDash && CheckMapping(GetMapping()->dashing))
			{
				SetPlayerState(DASHING);
			}
			break;
		case STATIC_ATTACK:
			if (AnimationIsFinished(player.currentAnimation))
			{
				ResetWeapon();
				if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
				{
					SetPlayerChooseAttack();
				}
				else if (!player.onGround)
				{
					SetPlayerState(JUMP);
				}
				else if (IsRunning())
				{
					SetPlayerState(RUN);
				}
				else
				{
					SetPlayerState(IDLE);
				}
			}
			break;
		case MOVING_ATTACK:
			if (AnimationIsFinished(player.currentAnimation))
			{

				ResetWeapon();
				if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
				{
					SetPlayerChooseAttack();
				}
				else if (!player.onGround)
				{
					SetPlayerState(JUMP);
				}
				else if (CheckMapping(GetMapping()->down) && player.canSlide)
				{
					SetPlayerState(SLIDING);
				}

				else if (IsRunning())
				{
					SetPlayerState(RUN);
				}
				else
				{
					SetPlayerState(IDLE);
				}
			}
		case JUMPING_ATTACK:
			if (AnimationIsFinished(player.currentAnimation))
			{
				ResetWeapon();
				if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
				{
					SetPlayerChooseAttack();
				}
				else if (CheckMapping(GetMapping()->down) && player.canSlide)
				{
					SetPlayerState(SLIDING);
				}
				else if (!player.onGround)
				{
					SetPlayerState(JUMP);
				}
				else if (IsRunning())
				{
					SetPlayerState(RUN);
				}
				else
				{
					SetPlayerState(IDLE);
				}
			}
			break;

		case DEAD:
			break;
		case SLIDING:
			if ((abs((int)player.velocity.x) <= PLAYER_SPEED_MAX && !CheckMapping(GetMapping()->down)) && player.canStand)
			{
				player.timerSlide = SPEED_SLIDE;
				if (CheckMapping(GetMapping()->jump) && player.canJump || player.IsBumped == sfTrue)
				{
					SetPlayerState(JUMP);
				}
				else if (player.canDash && CheckMapping(GetMapping()->dashing))
				{
					SetPlayerState(DASHING);
				}
				else if (IsRunning)
				{
					SetPlayerState(RUN);
				}
				else if (!IsRunning)
				{
					SetPlayerState(IDLE);
				}
			}
			break;
		case DASHING:

			if (CheckMapping(GetMapping()->attack))
			{

				SetPlayerChooseAttack();
			}
			else if (CheckMapping(GetMapping()->down) && player.canSlide)
			{
				SetPlayerState(SLIDING);
			}
			if (player.distanceDash >= TRAVELLED_DISTANCE)
			{
				if (player.onGround == sfFalse)
				{
					SetPlayerState(JUMP);

				}
				else if (!IsRunning)
				{
					SetPlayerState(IDLE);
				}
				else if (IsRunning)
				{
					SetPlayerState(RUN);
				}
			}
			break;

		case CRAWLING:
			if (!CheckMapping(GetMapping()->down))
			{
				if (CheckMapping(GetMapping()->jump) && player.canJump)
				{
					SetPlayerState(JUMP);
				}
				else if (player.canDash && CheckMapping(GetMapping()->dashing))
				{
					SetPlayerState(DASHING);
				}
				else if (IsRunning())
				{
					SetPlayerState(RUN);
				}
				else if (!IsRunning())
				{
					SetPlayerState(IDLE);
				}
			}

			break;
		case DAMAGE:
			if (AnimationIsFinished(player.currentAnimation))
			{

				if (player.canDash && CheckMapping(GetMapping()->dashing))
				{
					SetPlayerState(DASHING);
				}
				else if (CheckMapping(GetMapping()->jump) && player.canJump || player.IsBumped == sfTrue)
				{
					SetPlayerState(JUMP);
				}
				else if (IsRunning())
				{
					SetPlayerState(RUN);
				}

				else if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
				{
					SetPlayerChooseAttack();
				}
				else
				{
					SetPlayerState(IDLE);
				}
				break;
			}
		case KNOCKBACK:
			if (AnimationIsFinished(player.currentAnimation))
			{

				/*if (CheckMapping(GetMapping()->attack) && player.IsAttackingPressed == sfFalse)
				{
					SetPlayerChooseAttack();
				}
				else if (CheckMapping(GetMapping()->down) && player.canSlide)
				{
					SetPlayerState(SLIDING);
				}
				else if (player.canDash && CheckMapping(GetMapping()->dashing))
				{
					SetPlayerState(DASHING);
				}
				else if ((CheckMapping(GetMapping()->jump) && player.canJump) || player.IsBumped == sfTrue)
				{
					SetPlayerState(JUMP);
				}
				else if (player.onGround && IsRunning())
				{
					SetPlayerState(RUN);
				}
				else if (player.onGround && !IsRunning())
				{
					SetPlayerState(IDLE);
				}
				break;*/
				if (!player.onGround)
				{
					SetPlayerState(JUMP);
					// put to the last frame of jump
					SetAnimationFrame(&player.animation[JUMP], GetAnimationNBFrame(&player.animation[JUMP]) - 1);


				}
				else if (IsRunning)
				{
					SetPlayerState(RUN);
				}
				else
				{
					SetPlayerState(IDLE);
				}


			}

		}
	}

	// is used to not reset everyframe the setPlayerState
	else if (!player.isAlive && player.state != DEAD)
	{
		SetPlayerState(DEAD);
	}



}

void UpdatePlayerTurnAround(void)
{
	float JoystickPosX = sfJoystick_getAxisPosition(0, 0);

	if (CheckMapping(GetMapping()->right) || JoystickPosX > MIN_DETECTION_CONTROLLER)
	{
		sfVector2f scale = sfSprite_getScale(player.sprite);
		sfSprite_setScale(player.sprite, (sfVector2f) { (float)abs((int)scale.x), scale.y });
		player.direction = PLAYER_RIGHT;
	}
	else if (CheckMapping(GetMapping()->left) || JoystickPosX < -MIN_DETECTION_CONTROLLER)
	{
		sfVector2f scale = sfSprite_getScale(player.sprite);
		sfSprite_setScale(player.sprite, (sfVector2f) { (float)-abs((int)scale.x), scale.y });
		player.direction = PLAYER_LEFT;
	}
}
void PlayerMoveSide(float _dt)
{
	player.enableGravity = sfTrue;

	// Give a small acceleration over time
	PlayerAcceleration(_dt);


	if (CheckMapping(GetMapping()->right))
	{
		PlayerMoveSideRight(_dt);
		PlayerSlowInAir(_dt);
	}
	else if (CheckMapping(GetMapping()->left))
	{
		PlayerMoveSideLeft(_dt);
		PlayerSlowInAir(_dt);
	}
	else
	{
		PlayerSlow(_dt);
	}
}
void PlayerMoveSideLeft(float _dt)
{

	player.velocity.x = -(PLAYER_SPEED_MIN + ((PLAYER_SPEED_MAX - PLAYER_SPEED_MIN) * (player.acceleration / ACCELERATION_TIME))); //* GetAxisValue(GetMapping()->left));
	player.slow = 1;



}
void PlayerMoveSideRight(float _dt)
{
	player.velocity.x = (PLAYER_SPEED_MIN + ((PLAYER_SPEED_MAX - PLAYER_SPEED_MIN) * (player.acceleration / ACCELERATION_TIME)));// *GetAxisValue(GetMapping()->right));
	player.slow = 1;

}
void PlayerSlowInAir(float _dt)
{
	if (!player.onGround && player.dirB4Jump != player.direction)
	{
		player.velocity.x *= JUMP_DIRECTION_CHANGE;
	}
	player.lastSpeedReached = player.velocity.x;
}
void UpdatePlayerJump(float _dt)
{

	if (CheckMapping(GetMapping()->jump))
	{
		if (player.canJump)
		{
			//if (player.velocity.y > JUMP_MOMENTUM)
			//{
			player.canJump = sfFalse;

			player.cooldownJump = 0;
			player.velocity.y = PLAYER_IMPULSE;
			player.isJumping = sfTrue;
			player.dirB4Jump = player.direction;
			PlaySound(JUMP_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfTrue);
			AddMisc(MISC_JUMP, sfRectangleShape_getPosition(player.hitbox));

			if (!player.onGround)
			{
				player.nbJump--;
			}
			//}

		}
		// the character is not jumping anymore if he fall even when the key is pressed
		else if (player.isJumping && player.velocity.y > 0)
		{
			player.isJumping = sfFalse;
		}
	}
	else
	{
		player.isJumping = sfFalse;
	}
}

void PlayerSliding(float _dt)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);
	CellCoordinates cell[2] = { 0 };
	cell[0] = GetLevelColumnLineWithPosition((sfVector2f) { hitbox.left, hitbox.top - CELL_HEIGHT });
	cell[1] = GetLevelColumnLineWithPosition((sfVector2f) { hitbox.left + hitbox.width, hitbox.top - CELL_HEIGHT });
	if (player.timerSlide > 0 && (!GetLevelCollide(cell[0]) && !GetLevelCollide(cell[1])))
	{
		player.timerSlide -= _dt / SLOW_SLIDE;
		player.velocity.x = player.lastSpeedReached * player.timerSlide;
	}
	else if (GetLevelCollide(cell[0]) || GetLevelCollide(cell[1]))
	{
		player.velocity.x = player.lastSpeedReached * player.timerSlide;

	}
	else
	{
		player.slow = 0;
		player.velocity.x = 0;
		player.lastSpeedReached = 0;

	}
}


void PlayerDash(float _dt)
{
	if (CheckMapping(GetMapping()->dashing) && player.canDash)
	{
		if (player.direction == PLAYER_LEFT)
		{
			player.velocity.x = -DASH_SPEED;
			//player.lastSpeedReached = -PLAYER_SPEED_MAX;
			player.lastSpeedReached = -DASH_SPEED / SPEED_SLIDE;

		}
		if (player.direction == PLAYER_RIGHT)
		{

			player.velocity.x = DASH_SPEED;
			//player.lastSpeedReached = PLAYER_SPEED_MAX;
			player.lastSpeedReached = DASH_SPEED / SPEED_SLIDE;

		}
		player.distanceDash = 0;
		player.velocity.y = 0;
		player.canDash = sfFalse;
		player.nbDash--;
		player.cooldownDash = 0;
		//CheckCollisionDash();



	}

	player.distanceDash += (float)abs((int)(_dt * player.velocity.x));

	if (player.distanceDash >= (float)TRAVELLED_DISTANCE)
	{
		float var = player.direction == PLAYER_LEFT ? -1.f : 1.f;
		player.velocity.x = (TRAVELLED_DISTANCE - player.distanceDash) * var;
	}
}
void UpdatePlayerCooldown(float _dt)
{
	player.cooldownJump += _dt;
	if (player.nbDash < NB_DASH)
	{
		player.cooldownDash += _dt;
		if (player.cooldownDash >= COOLDOWN_DASH)
		{
			player.nbDash++;
			if (player.nbDash < NB_DASH)
			{
				player.cooldownDash -= COOLDOWN_DASH;
			}
			else
			{
				player.cooldownDash = 0;
			}

		}
	}
	// NOT SPAMMING DASH
	if (player.nbDash && !CheckMapping(GetMapping()->dashing))
	{
		player.canDash = sfTrue;
	}
	// checkMapping here is used to not make a double jump when the button is kept pressed
	if (player.nbJump > 0 && !CheckMapping(GetMapping()->jump))
	{

		player.canJump = sfTrue;
		// the player velocity is lower only if the player touched a bumper,
		// it means it cant cancel a bump from a bumper
		if (player.velocity.y <= PLAYER_IMPULSE)
		{
			player.canJump = sfFalse;
		}
	}
	// NOT SPAMMING ATTACK
	if (!CheckMapping(GetMapping()->attack))
	{
		player.IsAttackingPressed = sfFalse;
	}

	// INVINSIBLE TIMER,
	if (player.timerInvinsible > 0)
	{
		player.timerInvinsible -= _dt;
	}
	else
	{
		player.timerInvinsible = 0;
	}
	// blinking when he take dmg
	if (player.timerInvinsible > 0)
	{
		int timer = (int)(player.timerInvinsible * 4);
		if (timer % 2)
		{
			sfSprite_setColor(player.sprite, sfColor_fromRGBA(150, 0, 0, 150));
		}
		else
		{
			sfSprite_setColor(player.sprite, sfColor_fromRGBA(255, 255, 255, 255));
		}

	}
	else
	{
		sfSprite_setColor(player.sprite, sfColor_fromRGBA(255, 255, 255, 255));
	}
	player.IsBumped = sfFalse;

	//UpdateEnergy(_dt);

	//CheatMod
	if (sfKeyboard_isKeyPressed(sfKeyNumpad9) && player.isPressingCheatMod == sfFalse)
	{
		player.cheatMod = !player.cheatMod;
		player.isPressingCheatMod = sfTrue;

	}
	else if (!sfKeyboard_isKeyPressed(sfKeyNumpad9))
	{
		player.isPressingCheatMod = sfFalse;
	}

	// 
	if (player.cheatMod)
	{
		sfSprite_setColor(player.sprite, sfGreen);
	}
	else if (player.timerInvinsible <= 0)
	{
		sfSprite_setColor(player.sprite, sfWhite);
	}
}

void CollisionPlayerLevelBorders(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);
	sfFloatRect levelHitbox = GetLevelHitbox();
	if (hitbox.left < levelHitbox.left)
	{
		sfRectangleShape_move(player.hitbox, (sfVector2f) { levelHitbox.left - hitbox.left, 0 });
	}
	else if (hitbox.left + hitbox.width > levelHitbox.left + levelHitbox.width)
	{
		//SetGameState(GAME_OVER);
		sfRectangleShape_move(player.sprite, (sfVector2f) { levelHitbox.left + levelHitbox.width - (hitbox.left + hitbox.width), 0 });
	}

	if (hitbox.top < levelHitbox.top)
	{
		sfRectangleShape_move(player.hitbox, (sfVector2f) { 0, levelHitbox.top - hitbox.top });
	}
	else if (hitbox.top + hitbox.height > levelHitbox.top + levelHitbox.height)
	{
		sfRectangleShape_move(player.hitbox, (sfVector2f) { 0, levelHitbox.top + levelHitbox.height - (hitbox.top + hitbox.height) });
	}
}

void PlayerSlow(float _dt)
{
	if (player.slow > 0)
	{
		player.slow -= _dt / SLOW;
		player.velocity.x = player.lastSpeedReached * player.slow;
	}
	else
	{
		player.slow = 0;
		player.velocity.x = 0;
		player.lastSpeedReached = 0;

	}
	player.acceleration = 0;
}

void PlayerAcceleration(float _dt)
{
	player.acceleration += _dt;
	if (player.acceleration > ACCELERATION_TIME)
	{
		player.acceleration = ACCELERATION_TIME;
	}
}

void DrawPlayer(sfRenderWindow* _window)
{


	//DrawThunder(_window);

	DrawAnimation(player.currentAnimation, _window);
}

void DrawPlayerHitbox(sfRenderWindow* _window)
{
	sfRenderWindow_drawRectangleShape(_window, player.hitbox, NULL);
}

void CollisionPlayerPickup(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	for (unsigned i = 0; i < GetPickupNb(); i++)
	{
		sfFloatRect pickupHitbox = GetPickup(i);
		if (IsColliding2Rectangles(hitbox, pickupHitbox))
		{
			ObtainPickup(i);
		}
	}
}

void CollisionPlayerDoor(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	for (unsigned i = 0; i < GetDoorNb(); i++)
	{
		sfFloatRect doorHitbox = GetDoorHitbox(i);
		if (IsColliding2Rectangles(hitbox, doorHitbox))
		{
			DoorIsOpen();
			//GetAllFiles();
		}
	}
}

void CollisionPlayerText(void)
{

	unsigned textNb = GetSignNb();
	if (textNb > 0)
	{
		sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

		for (unsigned i = 0; i < textNb; i++)
		{
			if (IsColliding2Rectangles(hitbox, GetSignHitbox(i)))
			{
				if (!IsReading(i))
				{
					SetReading(i, sfTrue);
				}
			}
			else
			{
				SetReading(i, sfFalse);
			}
		}
	}
}

void CollisionPlayerLaser(void)
{
	if (player.timerInvinsible == 0)
	{
		unsigned laserNb = GetLaserNb();
		if (GetLaserActivity())
		{
			sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

			for (unsigned i = 0; i < laserNb; i++)
			{
				sfFloatRect laserHb = GetLaser(i);
				if (IsColliding2Rectangles(hitbox, laserHb))
				{
					EditPlayerHP(-1);

					sfFloatRect playerHb = sfRectangleShape_getGlobalBounds(player.hitbox);
					sfVector2f laserPos = { laserHb.left + laserHb.width / 2, laserHb.top + laserHb.height / 2 };
					sfVector2f playerPos = { playerHb.left + playerHb.width / 2, playerHb.top + playerHb.height / 2 };
					PlayerKnockback(KNOCKBACK_POWER, GetAngle(laserPos, playerPos));
					SetRandomCamShake(5, 10, 0.2f);

				}
			}
		}
	}
}

void CollisionPlayerBumper()
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	unsigned bumperNb = GetBumperNb();
	for (unsigned i = 0; i < bumperNb; i++)
	{
		if (IsColliding2Rectangles(hitbox, GetBumperHitbox(i)))
		{
			if (!GetBumperActivity(i))
			{
				if (player.onGround && player.state != DASHING)
				{
					player.canJump = sfFalse;
					player.velocity.y = PLAYER_IMPULSE_BUMPER;
					player.isJumping = sfFalse;//sfTrue;
					player.dirB4Jump = player.direction;
					player.IsBumped = sfTrue;
					ActivateBumper(i);
					UpdatePlayerState();
				}
			}
		}
	}
}

void DrawPlayerHitboxOrigin(sfRenderWindow* _window)
{
	sfVector2f position = sfRectangleShape_getPosition(player.hitbox);
	sfCircleShape* origin = CreateCircle(position, 4, sfTrue, sfRed, 0, sfTransparent);
	sfRenderWindow_drawCircleShape(_window, origin, NULL);

	sfCircleShape_destroy(origin);
	origin = NULL;

	sfVector2f position2 = sfSprite_getPosition(player.sprite);
	sfCircleShape* origin2 = CreateCircle(position2, 4, sfTrue, sfBlue, 0, sfTransparent);
	sfRenderWindow_drawCircleShape(_window, origin2, NULL);

	sfCircleShape_destroy(origin2);
	origin2 = NULL;
}

void CleanupPlayer(void)
{
	sfSprite_destroy(player.sprite);
	sfRectangleShape_destroy(player.hitbox);
	player.sprite = NULL;
	player.hitbox = NULL;
	sfTexture_destroy(player.texture);
	player.texture = NULL;
	player = (PlayerData){ 0 };
}

void KeypressedPlayer(sfKeyEvent _key)
{

}

void SetPlayerState(PlayerState _state)
{
	//sounds
	switch (_state)
	{
	case IDLE:
		break;
	case RUN:
		break;
	case JUMP:
		if (player.IsBumped)
		{
			PlaySound(BUMPER_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfTrue);
		}
		break;
	case SLIDING:
		PlaySound(SLIDE_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case FALL:
		break;
	case DAMAGE:
		PlaySound(HIT_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case DEAD:
		break;
	case DASHING:
		PlaySound(FALL_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		PlaySound(DASH_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		//SetRandomCamShake(3, 1, 0.4f);

		break;
	case CRAWLING:
		break;
	case STATIC_ATTACK:
		PlaySound(SWORD_SLASH_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case MOVING_ATTACK:
		PlaySound(SWORD_SLASH_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case JUMPING_ATTACK:
		PlaySound(SWORD_SLASH_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	default:
		break;
	}
	//player.distanceDash = 0;
	player.state = _state;
	player.currentAnimation = &player.animation[player.state];
	LoadAnimation(player.currentAnimation);
}

sfBool IsPlayerIsFalling(float _playerVelocityY)
{
	return _playerVelocityY > 0;
}

void PlayerJump(float _playerVelocityY)
{
	_playerVelocityY = PLAYER_IMPULSE;
}

const PlayerData* GetPlayer(void)
{
	return &player;
}

PlayerData* SetPlayer(void)
{
	return &player;
}


sfBool IsRunning(void)
{
	if (CheckMapping(GetMapping()->right) ||
		CheckMapping(GetMapping()->left))
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

void UpdateOnGroundTimer(float _dt)
{
	if (player.onGround)
	{
		player.onGroundTimer += _dt;
		if (player.onGroundTimer >= ON_GROUND_TIMER_MAX || player.velocity.y < 0)
		{
			player.onGroundTimer -= ON_GROUND_TIMER_MAX;
			player.onGround = sfFalse;
		}
	}
}

void CollisionPlayerLevelHoriz(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	//if (player.velocity.x != 0)
	//{
	LevelCollid col = CollisionTilesHoriz(hitbox);
	if (col.isColliding)
	{
		sfRectangleShape_move(player.hitbox, (sfVector2f) { col.offset, 0 });
		player.velocity.x = 0;
		player.distanceDash = TRAVELLED_DISTANCE;
	}
	//}
}

void CollisionPlayerLevelVerti(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	if (player.velocity.y != 0)
	{
		LevelCollid col = CollisionTilesVerti(hitbox);
		if (col.isColliding)
		{
			if (col.direction == COLLISION_DOWN)
			{
				player.onGround = sfTrue;
				player.canJump = sfTrue;
				player.nbJump = NB_JUMP;
				col.offset += OFFSET_COL; //avoid sprite vibration
				if (player.velocity.y > LANDING_SOUND && player.state != SLIDING)
				{
					PlaySound(LAND_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
				}
			}
			sfRectangleShape_move(player.hitbox, (sfVector2f) { 0, col.offset });
			player.velocity.y = 0;
		}
	}
}

void CollisionPlayerSecretZone(void)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);
	sfVector2f position = { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 };
	CellCoordinates coordinates = GetLevelColumnLineWithPosition(position);
	if (GetLevelCollideSecret(coordinates))
	{
		SetLevel()->layer[SECRET].vanish = sfTrue;
	}
	else
	{
		SetLevel()->layer[SECRET].vanish = sfFalse;
	}
}

void UpdateBulletTime(float _dt)
{
	if (!player.IsPressingBulletTime)
	{
		if (player.IsBulletTime == sfTrue)
		{
			player.energy -= ENERGY_LOST_BULLET_TIME * _dt;
			//player.timeBulletTime -= _dt * BULLET_TIME_SLOW;
			if (CheckMapping(GetMapping()->bulletTime))
			{
				player.IsBulletTime = sfFalse;
				player.IsPressingBulletTime = sfTrue;
			}
		}
		else if (player.IsBulletTime == sfFalse)
		{
			/*player.timeBulletTime += _dt;
			if (player.timeBulletTime >= BULLET_TIME_DURATION)
			{
				player.timeBulletTime = BULLET_TIME_DURATION;
			}*/

			// To press it 1 time to activate it
			if (CheckMapping(GetMapping()->bulletTime))
			{
				player.IsBulletTime = sfTrue;
				player.IsPressingBulletTime = sfTrue;
			}
		}
	}

	if (player.energy <= 0)
	{
		player.IsBulletTime = sfFalse;
	}
	if (!CheckMapping(GetMapping()->bulletTime))
	{
		player.IsPressingBulletTime = sfFalse;
	}

}

sfBool GetPlayerBulletTime()
{
	return player.IsBulletTime;
}

float GetEnergy()
{
	return player.energy;
}

void IncreaseEnergy(float _energy)
{
	player.energy += _energy;
}

void UpdateEnergy(float _dt)
{
	int size = GetSizeWeapon();
	if (player.energy <= 0)
	{
		SetSizeWeapon(0);
		player.energy = 0;
	}
	else if (player.energy >= ENERGY_MAX)
	{
		player.energy = ENERGY_MAX;
	}


}

void UpdateSlidingCondition()
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	// check if he can stand up
	if (player.state == SLIDING || player.state == CRAWLING)
	{

		CellCoordinates cell[2] = { 0 };
		cell[0] = GetLevelColumnLineWithPosition((sfVector2f) { hitbox.left + 1, hitbox.top - CELL_HEIGHT });
		cell[1] = GetLevelColumnLineWithPosition((sfVector2f) { hitbox.left + hitbox.width - 1, hitbox.top - CELL_HEIGHT });
		if (GetLevelCollide(cell[0]) || GetLevelCollide(cell[1]))
		{
			player.canStand = sfFalse;

		}
		else
		{
			player.canStand = sfTrue;
		}

	}
	else
	{
		player.canStand = sfTrue;
	}

	// check if he can slide

	//float var = player.direction == PLAYER_LEFT ? -1.f : 1.f;
	//CellCoordinates cell = GetLevelColumnLineWithPosition((sfVector2f) { (hitbox.left + hitbox.width / 2) + CELL_WIDTH * var, hitbox.top + hitbox.height / 2 });
	float var = player.direction == PLAYER_LEFT ? 0.f : 1.f;
	CellCoordinates cell = GetLevelColumnLineWithPosition((sfVector2f) { (hitbox.left + hitbox.width * var), hitbox.top + hitbox.height / 2 });
	if (GetLevelCollide(cell))
	{
		player.canSlide = sfFalse;
	}
	else
	{
		player.canSlide = sfTrue;
	}
}

void SetPlayerChooseAttack(void)
{
	SetAttackWeapon();
	player.IsAttackingPressed = sfTrue;
	int size = (int)GetSizeWeapon();
	int typeAttack = 0;
	if (size == 0)
	{
		PlaySound(SWORD_SLASH_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfTrue);
	}
	else if (size == 1)
	{
		PlaySound(SWORD_SLASH_2_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfTrue);
		player.energy -= ENERGY_LOST_1ST_SIZE;
	}
	else if (size == 2)
	{
		PlaySound(SWORD_SLASH_3_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfTrue);
		player.energy -= ENERGY_LOST_2ND_SIZE;
	}

	if (!player.onGround)
	{
		player.state = JUMPING_ATTACK;
		typeAttack = 2;
	}
	else if (IsRunning())
	{
		player.state = MOVING_ATTACK;
		typeAttack = 1;
	}
	else
	{
		player.state = STATIC_ATTACK;
		typeAttack = 0;

	}
	int state = (size * 3 + typeAttack);
	SetPlayerStateAttack(state);
}


void EditPlayerHP(int _editHP)
{
	if ((player.timerInvinsible == 0 && _editHP < 0) && !player.cheatMod)
	{
		player.HealthPoint += _editHP;
		SetLifeHUD(player.HealthPoint);
		player.timerInvinsible = INVINCIBLE_TIMER;
		PlaySound(HIT_PLAYER, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		//SetPlayerState(DAMAGE);
		lifeLost += -_editHP;

		if (player.HealthPoint <= 0)
		{
			//AddNewDeath();
			//LooseAllFiles();
		}
	}
	else if (_editHP > 0)
	{
		player.HealthPoint += _editHP;
		SetLifeHUD(player.HealthPoint);
		if (player.HealthPoint > MAX_HEALTH_POINTS)
		{
			player.HealthPoint = MAX_HEALTH_POINTS;
		}
	}
}

void SetPlayerStateAttack(int _index)
{


	player.currentAnimation = &player.animationAttack[_index];
	LoadAnimation(player.currentAnimation);
}

void LoadThunder()
{
	sfTexture* thunderTexture = sfTexture_createFromFile("Assets/Textures/Game/Player/FxPlayer/Player_Run.png", NULL);
	player.ThunderSprite = CreateSprite(thunderTexture, (sfVector2f) { 0, 0 }, sfFalse, SCALE);

	sfIntRect firstFrame = { 145,0,70,20 };
	player.ThunderAnimation = CreateAnimation(player.ThunderSprite, 12, FRAME_RATE * 2, firstFrame, sfTrue);

	sfSprite_setOrigin(player.ThunderSprite, (sfVector2f) { 70, 10 });
}

void UpdateThunder()
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);
	/*sfVector2f position = sfSprite_getPosition(player.sprite);
	sfSprite_setPosition(player.ThunderSprite, position);*/


	//sfFloatRect rect = player.hitbox;
	sfSprite_setPosition(player.ThunderSprite, (sfVector2f) { hitbox.left + hitbox.width / 2 + 15, hitbox.top + hitbox.height / 2 - 35 });

	if (player.direction == PLAYER_RIGHT)
	{
		sfSprite_setScale(player.ThunderSprite, (sfVector2f) { SCALE, SCALE });
	}
	else if (player.direction == PLAYER_LEFT)
	{
		sfSprite_setScale(player.ThunderSprite, (sfVector2f) { -SCALE, SCALE });
	}


}

void DrawThunder(sfRenderWindow* _window)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(player.hitbox);

	for (int i = 0; i < 5; i++)
	{

		sfSprite_setPosition(player.ThunderSprite, (sfVector2f) { hitbox.left + hitbox.width / 2 + 15, hitbox.top + (hitbox.height / 1) - 35 - 20 * i });

		if (player.direction == PLAYER_RIGHT)
		{
			sfSprite_setScale(player.ThunderSprite, (sfVector2f) { SCALE, SCALE });
		}
		else if (player.direction == PLAYER_LEFT)
		{
			sfSprite_setScale(player.ThunderSprite, (sfVector2f) { -SCALE, SCALE });
		}

		sfRenderWindow_drawSprite(_window, player.ThunderSprite, NULL);
	}
}

void SetPlayerActivity(sfBool _activity)
{

	if (_activity == sfFalse)
	{
		ResetWeapon();
	}
	player.isActif = _activity;
}

sfBool IsPlayerAlive()
{
	return player.isAlive;
}

void UpdateWeaponSize()
{

	if (CheckMapping(GetMapping()->resize) && player.isSwapingWeapon == sfFalse && !IsWeaponAttacking())
	{
		IncreaseSizeWeapon();
		player.isSwapingWeapon = sfTrue;
	}
	else if (!CheckMapping(GetMapping()->resize))
	{
		player.isSwapingWeapon = sfFalse;
	}
}

void PlayerKnockback(float _knockbackPower, float _angle)
{
	player.velocity.x = _knockbackPower * cosf(_angle);
	player.velocity.y = _knockbackPower * sinf(_angle);
	SetPlayerState(KNOCKBACK);
}
