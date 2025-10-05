#include "Boss.h"

BossStruct bossStruct;

sfVector2f playerPos = { 0,0 };
sfVector2f handPos = { 0,0 };

void ArmMovement(float _dt);
void Shifumi(float _dt);
void UpdateStateHand(int _hand, AttackHand _state);
void PrepareForSlap(sfSprite* _spriteHand);
void UpdateHitboxLocation(void);
sfBool DoesPlayerCollideWithHitbox(void);

void PrepareHandForEvent(float _dt);
void CreateActionHand(float _dt);
void ResetEverything(void);
void ArmAnimation(float _dt);

void ShowArmInPosition(float _dt);
void MakeTheAttack(float _dt);

void GunFire(HandStruct _hand, float _dt);

void DamageHand(float _dt);
void PlayerGetGrabbed(float _dt);

void DamageHead(float _dt);

int CalculDamageBoss(void);

void UpdateBossBlink(BossStruct* _enemy, int _bodyPart, float _dt);
void UpdateInvincibility(float _dt);

void LoadBoss(void)
{
	


	//head
	sfIntRect firstFrame = { 0, 2445, 131, 85 };

	bossStruct.texture = sfTexture_createFromFile("Assets/Textures/Boss/Sprite_sheet_boss_1.png", NULL);
	bossStruct.head.sprite = CreateSpriteFromSpriteSheet(bossStruct.texture, firstFrame, sfTrue, SCALE);
	sfSprite_setPosition(bossStruct.head.sprite, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	//his right hand
	firstFrame = (sfIntRect){ 0, 422, 65, 115 };
	bossStruct.handRight.sprite = CreateSpriteFromSpriteSheet(bossStruct.texture, firstFrame, sfTrue, SCALE);
	sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 });
	sfSprite_setScale(bossStruct.handRight.sprite, (sfVector2f) { -SCALE, SCALE });
	bossStruct.handRight.idleAnimation = CreateAnimation(bossStruct.handRight.sprite, 8, 8, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 259, 84, 69 };
	bossStruct.handRight.clapVerticalAnimation = CreateAnimation(bossStruct.handRight.sprite, 1, 1, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 1117, 112, 109 };
	bossStruct.handRight.shifumiAnim = CreateAnimation(bossStruct.handRight.sprite, 10, 10, firstFrame, sfFalse);
	bossStruct.handRight.rockAnim = CreateAnimation(bossStruct.handRight.sprite, 1, 1, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ 0, 1441, 114, 65 };
	bossStruct.handRight.paperAnim = CreateAnimation(bossStruct.handRight.sprite, 1, 1, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ 0, 1293, 126, 56 };
	bossStruct.handRight.scissorsAnim = CreateAnimation(bossStruct.handRight.sprite, 1, 1, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 96, 117, 64 };
	bossStruct.handRight.gunAnim = CreateAnimation(bossStruct.handRight.sprite, 16, GUN_SPEED, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 1597, 187, 187 };
	bossStruct.handRight.deathAnim = CreateAnimation(bossStruct.handRight.sprite, 7, 7, firstFrame, sfFalse);


	firstFrame = (sfIntRect){ 0, 669, 114, 65 };
	bossStruct.handRight.grabIdle = CreateAnimation(bossStruct.handRight.sprite, 1, 4, firstFrame, sfFalse);
	bossStruct.handRight.grabing = CreateAnimation(bossStruct.handRight.sprite, 4, 4, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ 342, 669, 114, 65 };
	bossStruct.handRight.grab = CreateAnimation(bossStruct.handRight.sprite, 1, 4, firstFrame, sfFalse);

	firstFrame = (sfIntRect){0 , 3326, 108, 65};
	bossStruct.handRight.loseAnim = CreateAnimation(bossStruct.handRight.sprite, 4, 9, firstFrame, sfFalse);
	
	


	


	//his left hand
	firstFrame = (sfIntRect){ 0, 422, 65, 115 };
	bossStruct.handLeft.sprite = CreateSpriteFromSpriteSheet(bossStruct.texture, firstFrame, sfTrue, SCALE);
	sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { SCREEN_WIDTH / 2 + 250, SCREEN_HEIGHT / 2 });
	bossStruct.handLeft.idleAnimation = CreateAnimation(bossStruct.handLeft.sprite, 8, 8, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 259, 84, 69 };
	bossStruct.handLeft.clapVerticalAnimation = CreateAnimation(bossStruct.handLeft.sprite, 1, 1, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 1117, 112, 109 };
	bossStruct.handLeft.shifumiAnim = CreateAnimation(bossStruct.handLeft.sprite, 10, 10, firstFrame, sfFalse);
	
	bossStruct.handLeft.rockAnim = CreateAnimation(bossStruct.handLeft.sprite, 1, 1, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ 0, 1293, 126, 56 };
	bossStruct.handLeft.scissorsAnim = CreateAnimation(bossStruct.handLeft.sprite, 1, 1, firstFrame, sfTrue);

	firstFrame = (sfIntRect){ 0, 1441, 114, 65 };
	bossStruct.handLeft.paperAnim = CreateAnimation(bossStruct.handLeft.sprite, 1, 1, firstFrame, sfFalse);
	
	firstFrame = (sfIntRect){ 0, 96, 117, 64 };
	bossStruct.handLeft.gunAnim = CreateAnimation(bossStruct.handLeft.sprite, 16, GUN_SPEED, firstFrame, sfTrue);
	firstFrame = (sfIntRect){ 0, 1597, 187, 187 };
	bossStruct.handLeft.deathAnim = CreateAnimation(bossStruct.handLeft.sprite, 7, 7, firstFrame, sfFalse);

	firstFrame = (sfIntRect){ 0, 669, 114, 65 };
	bossStruct.handLeft.grabIdle = CreateAnimation(bossStruct.handLeft.sprite, 1, 4, firstFrame, sfFalse);
	bossStruct.handLeft.grabing = CreateAnimation(bossStruct.handLeft.sprite, 4, 4, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ 342, 669, 114, 65 };
	bossStruct.handLeft.grab = CreateAnimation(bossStruct.handLeft.sprite, 1, 4, firstFrame, sfFalse);


	firstFrame = (sfIntRect){ 0 , 3326, 108, 65 };
	bossStruct.handLeft.loseAnim = CreateAnimation(bossStruct.handLeft.sprite, 4, 9, firstFrame, sfFalse);
	//bossStruct.handLeft.scissorsAnim = bossStruct.handLeft.deathAnim; 
	//bossStruct.handLeft.loseAnim = bossStruct.handLeft.deathAnim;






	//head
	firstFrame = (sfIntRect){ 0, 2446, 131, 85 };
	bossStruct.head.beginRelease = CreateAnimation(bossStruct.head.sprite, 4, 4, firstFrame, sfFalse);
	firstFrame = (sfIntRect){ 0, 2570, 131, 85 };
	bossStruct.head.endRelease = CreateAnimation(bossStruct.head.sprite, 4, 4, firstFrame, sfFalse);


	firstFrame = (sfIntRect){ 0, 2996, 193, 159 };
	bossStruct.head.death = CreateAnimation(bossStruct.head.sprite, 6, 4, firstFrame, sfFalse);



	//sfFloatRect hitbox = sfSprite_getGlobalBounds(bossStruct.handRight.sprite);
	//sfVector2f pos;
	//pos.x = hitbox.left + hitbox.width / 2;
	//pos.y = hitbox.top + hitbox.height / 2;
	//sfSprite_setOrigin(bossStruct.handRight.sprite, pos);

	//hitbox = sfSprite_getGlobalBounds(bossStruct.handLeft.sprite);
	//pos;
	//pos.x = hitbox.left + hitbox.width / 2;
	//pos.y = hitbox.top + hitbox.height / 2;
	//sfSprite_setOrigin(bossStruct.handLeft.sprite, pos);

	//hitbox = sfSprite_getGlobalBounds(bossStruct.head.sprite);
	//pos;
	//pos.x = hitbox.left + hitbox.width / 2;
	//pos.y = hitbox.top + hitbox.height / 2;
	//sfSprite_setOrigin(bossStruct.head.sprite, pos);


	//define position where boss go
	bossStruct.position[0] = (sfVector2f){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + DISTANCE_POSITION_FROM_CENTER / 2 };
	bossStruct.position[1] = (sfVector2f){ SCREEN_WIDTH / 2 - DISTANCE_POSITION_FROM_CENTER, SCREEN_HEIGHT / 2 };
	bossStruct.position[2] = (sfVector2f){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - DISTANCE_POSITION_FROM_CENTER / 2 };
	bossStruct.position[3] = (sfVector2f){ SCREEN_WIDTH / 2 + DISTANCE_POSITION_FROM_CENTER, SCREEN_HEIGHT / 2 };

	//hitbox
	bossStruct.hitbox.leftHandSlapHitbox = CreateRectangle((sfFloatRect) { 0, 0, 150, 50 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.rightHandSlapHitbox = CreateRectangle((sfFloatRect) { 0, 0, 150, 50 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.leftHandDamageHitbox = CreateRectangle((sfFloatRect) { 0, 0, 120, 180 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.rightHandDamageHitbox = CreateRectangle((sfFloatRect) { 0, 0, 120, 180 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.leftGrabHitbox = CreateRectangle((sfFloatRect) { 0, 0, 80, 80 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.rightGrabHitbox = CreateRectangle((sfFloatRect) { 0, 0, 80, 80 }, sfTrue, sfTransparent, 3.0f, sfRed);
	bossStruct.hitbox.headHitbox = CreateRectangle((sfFloatRect) { 0, 0, 160, 160 }, sfTrue, sfTransparent, 3.0f, sfRed);

	//movement var
	bossStruct.positionGoal = 0;
	bossStruct.head.hp = HP_HEAD;
	bossStruct.handRight.hp = HP_HAND;
	bossStruct.handLeft.hp = HP_HAND;
	bossStruct.handEndShifumi = SHIFUMI_TIMER;
	bossStruct.prepareHand = PREPARE_HAND;
	bossStruct.waiting = WAITING_TIME;
	bossStruct.startAction = sfFalse;
	bossStruct.shifumiOver = sfFalse;
	bossStruct.handReady = sfFalse;
	bossStruct.actionOver = sfFalse;

	bossStruct.handLeft.canDamage = DAMAGE_AGAIN_TIMER;
	bossStruct.handRight.canDamage = DAMAGE_AGAIN_TIMER;
	bossStruct.canGrab = GRABBING_TIMER;
	bossStruct.blink[0].alpha = BLINK_ALPHA_FULL;
	bossStruct.blink[1].alpha = BLINK_ALPHA_FULL;
	bossStruct.blink[2].alpha = BLINK_ALPHA_FULL;

	bossStruct.blink[0].blinkTimer = 0.0f;
	bossStruct.blink[1].blinkTimer = 0.0f;
	bossStruct.blink[2].blinkTimer = 0.0f;

	bossStruct.isArrivedX = sfFalse;
	bossStruct.isArrivedY = sfFalse;
	bossStruct.phase = PHASE_HAND;
}

void UpdateBoss(float _dt)
{
	if (bossStruct.phase != BOSS_DEFEATED)
	{
		if (bossStruct.timeStop <= 0.0f)
		{
			HeadMovement(_dt);

			ArmMovement(_dt);

			bossStruct.actionOver = sfFalse;
			SetPlayerActivity(sfTrue);
		}
		else
		{
			if (!bossStruct.shifumiOver)
			{
				if (bossStruct.handLeft.hp > 0 && bossStruct.handRight.hp > 0)
				{
					//shifumi


					Shifumi(_dt);

				}
				else if (bossStruct.handLeft.hp > 0 || bossStruct.handRight.hp > 0)
				{

					AttackHand temp = NO_ATTACK;
					if (rand() % 2 == 0)
					{
						temp = GUN_PREPARING;
					}
					else
					{
						temp = SLAP_PREPARING;
					}
					bossStruct.handLeft.state = temp;
					bossStruct.handRight.state = temp;
					bossStruct.shifumiOver = sfTrue;
				}
				else
				{
					bossStruct.phase = PHASE_HEAD;
				}
			}

			if (bossStruct.phase == PHASE_HAND)
			{
				//shifumi over ? prepare hands for actions
				if (bossStruct.shifumiOver)
				{
					PrepareHandForEvent(_dt);
					ArmMovement(_dt);

					if (bossStruct.handReady)
					{
						CreateActionHand(_dt);
						bossStruct.timeStop = STOP_TIME;
					}


				}
				if (bossStruct.actionOver)
				{

					//reset EVERYTHINGS at default value (except for life)
					ResetEverything();

					//it's over so start timer stop ?

					bossStruct.timeStop -= _dt;
				}
			}
			else
			{
				if (!bossStruct.actionOver)
				{
					UpdateAnimation(&bossStruct.head.beginRelease, _dt);
					if (bossStruct.head.beginRelease.frame == 3)
					{
						bossStruct.head.endRelease.frame = 0;
						AddEnemy(ENEMY_EYE, sfSprite_getPosition(bossStruct.head.sprite));
						bossStruct.actionOver = sfTrue;
					}
				}
				else
				{
					UpdateAnimation(&bossStruct.head.endRelease, _dt);
					if (bossStruct.head.endRelease.frame == 3)
					{
						bossStruct.head.beginRelease.frame = 0;
						ResetEverything();
						bossStruct.timeStop -= _dt;
					}

				}

			}
		}

		if (bossStruct.phase == PHASE_HEAD)
		{
			DamageHead(_dt);
			UpdateBossBlink(&bossStruct, HEAD_PART, _dt);
		}
		else
		{
			DamageHand(_dt);

			UpdateBossBlink(&bossStruct, LEFT_HAND, _dt);
			UpdateBossBlink(&bossStruct, RIGHT_HAND, _dt);
		}

		UpdateHitboxLocation();
		ArmAnimation(_dt);
		UpdateInvincibility(_dt);
	}
	else
	{

		UpdateAnimation(&bossStruct.head.death, _dt);
		if (GetTransitionState(READY))
		{
			StartTransitionPicture(sfWhite, 2, 10);
		}
		else if (GetTransitionState(COVER_SCREEN))
		{
			SetGameState(WIN);
		}

		if (bossStruct.counter <= 15)
		{

			bossStruct.counter++;
		}
	}
}

void DrawBoss(sfRenderWindow* _window)
{

	sfSprite_setColor(bossStruct.handLeft.sprite, sfColor_fromRGBA(255, 255, 255, bossStruct.blink[0].alpha));
	sfSprite_setColor(bossStruct.handRight.sprite, sfColor_fromRGBA(255, 255, 255, bossStruct.blink[1].alpha));
	sfSprite_setColor(bossStruct.head.sprite, sfColor_fromRGBA(255, 255, 255, bossStruct.blink[2].alpha));

	sfRenderWindow_drawSprite(_window, bossStruct.head.sprite, NULL);
	sfRenderWindow_drawSprite(_window, bossStruct.handRight.sprite, NULL);
	sfRenderWindow_drawSprite(_window, bossStruct.handLeft.sprite, NULL);

	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.leftHandSlapHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.rightHandSlapHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.rightHandDamageHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.leftHandDamageHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.rightGrabHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.leftGrabHitbox, NULL);
	//sfRenderWindow_drawRectangleShape(_window, bossStruct.hitbox.headHitbox, NULL);
}

void CleanupBoss(void)
{
}

void HeadMovement(float _dt)
{
	sfVector2f bossPos = sfSprite_getPosition(bossStruct.head.sprite);

	//movement X of head

	if (abs(bossPos.x - bossStruct.position[bossStruct.positionGoal].x) <= ACCEPTANCE_RADIUS)
	{
		bossStruct.movement.x = 0;
		bossStruct.isArrivedX = sfTrue;
	}

	else if (bossPos.x >= bossStruct.position[bossStruct.positionGoal].x)
	{
		bossStruct.movement.x = -HEAD_SPEED * _dt;
	}
	else
	{
		bossStruct.movement.x = HEAD_SPEED * _dt;
	}

	//movement Y of head

	if (abs(bossPos.y - bossStruct.position[bossStruct.positionGoal].y) <= ACCEPTANCE_RADIUS / 2)
	{
		bossStruct.movement.y = 0;
		bossStruct.isArrivedY = sfTrue;
	}

	else if (bossPos.y >= bossStruct.position[bossStruct.positionGoal].y)
	{
		bossStruct.movement.y = -HEAD_SPEED * _dt;
	}
	else
	{
		bossStruct.movement.y = HEAD_SPEED * _dt;
	}

	if (bossStruct.isArrivedX && bossStruct.isArrivedY)
	{
		// 
		bossStruct.isArrivedY = sfFalse;
		bossStruct.isArrivedX = sfFalse;
		bossStruct.positionGoal = (bossStruct.positionGoal + 1) % 4;
		bossStruct.timeStop = STOP_TIME;
		bossStruct.handLeft.shifumiAnim.frame = 0;
		bossStruct.handRight.shifumiAnim.frame = 0;
		bossStruct.handLeft.loseAnim.frame = 0;
		bossStruct.handRight.loseAnim.frame = 0;


	}


	//head
	sfSprite_move(bossStruct.head.sprite, bossStruct.movement);
}

void ArmMovement(float _dt)
{
	if (bossStruct.handLeft.hp > 0)
	{
		switch (bossStruct.handLeft.state)
		{
		case NO_ATTACK:
			sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { sfSprite_getPosition(bossStruct.head.sprite).x + DISTANCE_HAND_HEAD, sfSprite_getPosition(bossStruct.head.sprite).y });
			break;
		case VERTICAL_SLAP:
			break;
		case GRAB:
			playerPos = sfSprite_getPosition(GetPlayer()->sprite);
			handPos = sfSprite_getPosition(bossStruct.handLeft.sprite);

			if (abs(playerPos.x - handPos.x) >= ACCEPTANCE_RADIUS)
			{
				if (playerPos.x > handPos.x)
				{
					sfSprite_move(bossStruct.handLeft.sprite, (sfVector2f) { ARM_SPEED_GRAB* _dt, 0 });
				}
				else
				{
					sfSprite_move(bossStruct.handLeft.sprite, (sfVector2f) { -ARM_SPEED_GRAB * _dt, 0 });
				}
			}

			if (abs(playerPos.y - handPos.y) >= ACCEPTANCE_RADIUS)
			{
				if (playerPos.y > handPos.y)
				{
					sfSprite_move(bossStruct.handLeft.sprite, (sfVector2f) { 0, ARM_SPEED_GRAB* _dt });
				}
				else
				{
					sfSprite_move(bossStruct.handLeft.sprite, (sfVector2f) { 0, -ARM_SPEED_GRAB * _dt });
				}
			}

			break;
		case GUN:
			break;
		default:
			break;
		}
	}

	if (bossStruct.handRight.hp > 0)
	{
		switch (bossStruct.handRight.state)
		{
		case NO_ATTACK:
			sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { sfSprite_getPosition(bossStruct.head.sprite).x - DISTANCE_HAND_HEAD, sfSprite_getPosition(bossStruct.head.sprite).y });
			break;
		case VERTICAL_SLAP:
			break;
		case GRAB:
			playerPos = sfSprite_getPosition(GetPlayer()->sprite);
			handPos = sfSprite_getPosition(bossStruct.handRight.sprite);

			if (abs(playerPos.x - handPos.x) >= ACCEPTANCE_RADIUS)
			{
				if (playerPos.x > handPos.x)
				{
					sfSprite_move(bossStruct.handRight.sprite, (sfVector2f) { ARM_SPEED_GRAB* _dt, 0 });
				}
				else
				{
					sfSprite_move(bossStruct.handRight.sprite, (sfVector2f) { -ARM_SPEED_GRAB * _dt, 0 });
				}
			}

			if (abs(playerPos.y - handPos.y) >= ACCEPTANCE_RADIUS)
			{
				if (playerPos.y > handPos.y)
				{
					sfSprite_move(bossStruct.handRight.sprite, (sfVector2f) { 0, ARM_SPEED_GRAB* _dt });
				}
				else
				{
					sfSprite_move(bossStruct.handRight.sprite, (sfVector2f) { 0, -ARM_SPEED_GRAB * _dt });
				}
			}
			break;
		case GUN:
			break;
		default:
			break;
		}
	}

}


void Shifumi(float _dt)
{
	bossStruct.handLeft.state = SHIFUMI_STATE;
	bossStruct.handRight.state = SHIFUMI_STATE;

	if (bossStruct.handLeft.shifumiAnim.frame == SHIFUMI_NBR_FRAME || bossStruct.handRight.shifumiAnim.frame == SHIFUMI_NBR_FRAME)
	{
		bossStruct.handLeft.shifumiAnim.frame = 0;
		bossStruct.handRight.shifumiAnim.frame = 0;

		bossStruct.shifumiOver = sfTrue;


		UpdateStateHand(LEFT_HAND, NO_ATTACK);
		UpdateStateHand(RIGHT_HAND, NO_ATTACK);


		int handWinner = -1;

		//bossStruct.handLeft.shifumiValue = 1 + rand() % SCISSORS;
		//bossStruct.handRight.shifumiValue = 1 + rand() % SCISSORS;
		//if (bossStruct.handLeft.shifumiValue == bossStruct.handRight.shifumiValue)
		//{
		//	handWinner = 2;
		//}
		//if (bossStruct.handLeft.shifumiValue != ROCK &&
		//	bossStruct.handRight.shifumiValue != ROCK)
		//{
		//	if (bossStruct.handLeft.shifumiValue > bossStruct.handRight.shifumiValue)
		//	{
		//		handWinner = LEFT_HAND;
		//	}
		//	else if (bossStruct.handLeft.shifumiValue < bossStruct.handRight.shifumiValue)
		//	{
		//		handWinner = RIGHT_HAND;
		//	}
		//}
		//else
		//{
		//	if (bossStruct.handLeft.shifumiValue % SCISSORS > bossStruct.handRight.shifumiValue % SCISSORS)
		//	{
		//		handWinner = LEFT_HAND;
		//	}
		//	else if (bossStruct.handLeft.shifumiValue % SCISSORS < bossStruct.handRight.shifumiValue % SCISSORS)
		//	{
		//		handWinner = RIGHT_HAND;
		//	}
		//}

		int value = rand() % 3;
		if (value == 0)
		{
			handWinner = LEFT_HAND;
			bossStruct.handLeft.shifumiValue = 1 + (rand() % SCISSORS);
			bossStruct.handRight.shifumiValue = 0;
		}
		else if (value == 1)
		{
			handWinner = RIGHT_HAND;
			bossStruct.handRight.shifumiValue = 1 + (rand() % SCISSORS);
			bossStruct.handLeft.shifumiValue = 0;
		}
		else
		{
			handWinner = EQUALITY_SHIFUMI_VALUE;
			bossStruct.handRight.shifumiValue = 1 + (rand() % SCISSORS);
			
			bossStruct.handLeft.shifumiValue = bossStruct.handRight.shifumiValue;
		}
		if (handWinner == LEFT_HAND)
		{
			switch (bossStruct.handLeft.shifumiValue)
			{
			case PAPER:
				UpdateStateHand(LEFT_HAND, SLAP_PREPARING);
				UpdateStateHand(RIGHT_HAND, SAD_HAND);
				break;
			case ROCK:
				UpdateStateHand(LEFT_HAND, GRAB_PREPARING);
				UpdateStateHand(RIGHT_HAND, SAD_HAND);
				break;
			case SCISSORS:
				UpdateStateHand(LEFT_HAND, GUN_PREPARING);
				UpdateStateHand(RIGHT_HAND, SAD_HAND);
				break;
			default:
				//UpdateStateHand(LEFT_HAND, SLAP_PREPARING);
				//UpdateStateHand(RIGHT_HAND, SAD_HAND);
				break;
			}
		}
		else if (handWinner == RIGHT_HAND)
		{
			switch (bossStruct.handRight.shifumiValue)
			{
			case PAPER:
				UpdateStateHand(RIGHT_HAND, SLAP_PREPARING);
				UpdateStateHand(LEFT_HAND, SAD_HAND);
				break;
			case ROCK:
				UpdateStateHand(RIGHT_HAND, GRAB_PREPARING);
				UpdateStateHand(LEFT_HAND, SAD_HAND);
				
				break;
			case SCISSORS:
				UpdateStateHand(RIGHT_HAND, GUN_PREPARING);
				UpdateStateHand(LEFT_HAND, SAD_HAND);
				break;
			default:
				//UpdateStateHand(RIGHT_HAND, SLAP_PREPARING);
				//UpdateStateHand(LEFT_HAND, SAD_HAND);
				break;
			}
		}

		
		//equality
		else if (handWinner == EQUALITY_SHIFUMI_VALUE)
		{
			switch (bossStruct.handRight.shifumiValue)
			{
			case PAPER:
				UpdateStateHand(RIGHT_HAND, SLAP_PREPARING);
				UpdateStateHand(LEFT_HAND, SLAP_PREPARING);
				break;
			case ROCK:
				UpdateStateHand(RIGHT_HAND, GRAB_PREPARING);
				//UpdateStateHand(LEFT_HAND, NO_ATTACK);
				UpdateStateHand(LEFT_HAND, GRAB_PREPARING);
				break;
			case SCISSORS:
				UpdateStateHand(RIGHT_HAND, GUN_PREPARING);
				UpdateStateHand(LEFT_HAND, GUN_PREPARING);
				break;
			default:
				break;
			}
		}
	}
}


void UpdateStateHand(int _hand, AttackHand _state)
{
	if (_hand == LEFT_HAND)
	{
		bossStruct.handLeft.state = _state;
	}
	else
	{
		bossStruct.handRight.state = _state;
	}
}

void UpdateHitboxLocation(void)
{
	sfRectangleShape_setPosition(bossStruct.hitbox.leftHandSlapHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handLeft.sprite).x, sfSprite_getPosition(bossStruct.handLeft.sprite).y });
	sfRectangleShape_setPosition(bossStruct.hitbox.rightHandSlapHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handRight.sprite).x, sfSprite_getPosition(bossStruct.handRight.sprite).y });
	sfRectangleShape_setPosition(bossStruct.hitbox.rightHandDamageHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handRight.sprite).x, sfSprite_getPosition(bossStruct.handRight.sprite).y });
	sfRectangleShape_setPosition(bossStruct.hitbox.leftHandDamageHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handLeft.sprite).x, sfSprite_getPosition(bossStruct.handLeft.sprite).y });
	sfRectangleShape_setPosition(bossStruct.hitbox.leftGrabHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handLeft.sprite).x + OFFSET_GRAB_X, sfSprite_getPosition(bossStruct.handLeft.sprite).y - OFFSET_GRAB_Y });
	sfRectangleShape_setPosition(bossStruct.hitbox.rightGrabHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.handRight.sprite).x - OFFSET_GRAB_X, sfSprite_getPosition(bossStruct.handRight.sprite).y - OFFSET_GRAB_Y });
	sfRectangleShape_setPosition(bossStruct.hitbox.headHitbox, (sfVector2f) { sfSprite_getPosition(bossStruct.head.sprite).x - HEAD_HITBOX_OFFSET_X, sfSprite_getPosition(bossStruct.head.sprite).y - HEAD_HITBOX_OFFSET_Y });
}

sfBool DoesPlayerCollideWithHitbox(void)
{
	return (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.leftHandSlapHitbox)) && bossStruct.handLeft.state == VERTICAL_SLAP) ||
		(IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.rightHandSlapHitbox)) && bossStruct.handRight.state == VERTICAL_SLAP);
}

void PrepareHandForEvent(float _dt)
{
	if (bossStruct.handEndShifumi <= 0.0f)
	{
		bossStruct.handReady = sfTrue;
	}
	else
	{
		bossStruct.handEndShifumi -= _dt;
	}
}

void CreateActionHand(float _dt)
{
	if (bossStruct.prepareHand <= 0.0f)
	{
		if (bossStruct.startAction == sfFalse)
		{
			ShowArmInPosition(_dt);
		}
		else
		{
			MakeTheAttack(_dt);
		}
	}
	else
	{
		bossStruct.prepareHand -= _dt;
	}

}


void ResetEverything(void)
{
	bossStruct.handEndShifumi = SHIFUMI_TIMER;
	bossStruct.prepareHand = PREPARE_HAND;
	bossStruct.waiting = WAITING_TIME;
	bossStruct.startAction = sfFalse;
	bossStruct.shifumiOver = sfFalse;
	bossStruct.handReady = sfFalse;
	bossStruct.bulletAmount = 0;

	bossStruct.handLeft.state = NO_ATTACK;
	bossStruct.handRight.state = NO_ATTACK;
	SetPlayerActivity(sfTrue);
}

void ArmAnimation(float _dt)
{
	if (bossStruct.handLeft.hp <= 0)
	{
		UpdateAnimation(&bossStruct.handLeft.deathAnim, _dt);
		if (bossStruct.handLeft.deathAnim.frame == bossStruct.handLeft.deathAnim.nbFrame)
		{
			sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { DEATH_POSITION, DEATH_POSITION });
		}
	}
	else
	{
		switch (bossStruct.handLeft.state)
		{
		case HAND_DEAD:
			UpdateAnimation(&bossStruct.handLeft.deathAnim, _dt);

			sfVector2f pos;
			pos.x = bossStruct.handLeft.deathAnim.firstFrame.left + bossStruct.handLeft.deathAnim.firstFrame.width / 2;
			pos.y = bossStruct.handLeft.deathAnim.firstFrame.top + bossStruct.handLeft.deathAnim.firstFrame.height / 2;
			sfSprite_setOrigin(bossStruct.handLeft.sprite, pos);
			break;
		case NO_ATTACK:
			UpdateAnimation(&bossStruct.handLeft.idleAnimation, _dt);
			break;
		case SLAP_PREPARING:
			UpdateAnimation(&bossStruct.handLeft.paperAnim, _dt);
			break;
		case VERTICAL_SLAP:
			UpdateAnimation(&bossStruct.handLeft.clapVerticalAnimation, _dt);
			if (bossStruct.startAction == sfFalse)
			{
				PrepareForSlap(bossStruct.handLeft.sprite);
			}
			break;
		case GUN_PREPARING:
			
			UpdateAnimation(&bossStruct.handLeft.scissorsAnim, _dt);
			break;
		case GRAB_PREPARING:
			
			UpdateAnimation(&bossStruct.handLeft.rockAnim, _dt);
			break;
		case GRAB:
			UpdateAnimation(&bossStruct.handLeft.rockAnim, _dt);
			if (bossStruct.handRight.state != VERTICAL_SLAP)
			{
				PrepareForSlap(bossStruct.handRight.sprite);
			}
			PlayerGetGrabbed(_dt);
			break;
		case GUN:
			if (bossStruct.startAction == sfFalse)
			{
				UpdateAnimation(&bossStruct.handLeft.scissorsAnim, _dt);

			}
			else
			{
				UpdateAnimation(&bossStruct.handLeft.gunAnim, _dt);

			}
			break;
		case SHIFUMI_STATE:
			UpdateAnimation(&bossStruct.handLeft.shifumiAnim, _dt);
			break;
		case SAD_HAND:
			UpdateAnimation(&bossStruct.handLeft.loseAnim, _dt);
			break;
		default:
			break;
		}
	}
	if (bossStruct.handRight.hp <= 0)
	{
		UpdateAnimation(&bossStruct.handRight.deathAnim, _dt);
		if (bossStruct.handRight.deathAnim.frame == bossStruct.handRight.deathAnim.nbFrame)
		{
			sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { DEATH_POSITION, DEATH_POSITION });
		}
	}
	else
	{
		switch (bossStruct.handRight.state)
		{
		case HAND_DEAD:
			UpdateAnimation(&bossStruct.handRight.deathAnim, _dt);

			break;
		case NO_ATTACK:
			UpdateAnimation(&bossStruct.handRight.idleAnimation, _dt);
			break;
		case SLAP_PREPARING:
			UpdateAnimation(&bossStruct.handRight.paperAnim, _dt);
			break;
		case VERTICAL_SLAP:
			UpdateAnimation(&bossStruct.handRight.clapVerticalAnimation, _dt);
			if (bossStruct.startAction == sfFalse)
			{
				PrepareForSlap(bossStruct.handRight.sprite);
			}
			break;
		case GUN_PREPARING:
		
			UpdateAnimation(&bossStruct.handRight.scissorsAnim, _dt);
			break;
		case GUN:
			if (bossStruct.startAction == sfFalse)
			{
				UpdateAnimation(&bossStruct.handRight.scissorsAnim, _dt);
			}
			else
			{
				UpdateAnimation(&bossStruct.handRight.gunAnim, _dt);
			}
			break;
		case GRAB_PREPARING:
			UpdateAnimation(&bossStruct.handRight.rockAnim, _dt);
			break;
		case GRAB:
			UpdateAnimation(&bossStruct.handRight.rockAnim, _dt);
			if (bossStruct.handLeft.state != VERTICAL_SLAP)
			{
				PrepareForSlap(bossStruct.handLeft.sprite);
			}
			PlayerGetGrabbed(_dt);
			break;
		case SHIFUMI_STATE:
			UpdateAnimation(&bossStruct.handRight.shifumiAnim, _dt);
			break;
		case SAD_HAND:
			UpdateAnimation(&bossStruct.handRight.loseAnim, _dt);
			
			break;
		default:
			break;
		}
	}
}

void ShowArmInPosition(float _dt)
{
	switch (bossStruct.handLeft.state)
	{
	case SLAP_PREPARING:

		UpdateStateHand(LEFT_HAND, VERTICAL_SLAP);

		break;
	case GUN_PREPARING:

		UpdateStateHand(LEFT_HAND, GUN);

		break;
	case GRAB_PREPARING:
		if (bossStruct.handLeft.state == bossStruct.handRight.state)
		{

		}
		else
		{
			UpdateStateHand(RIGHT_HAND, GRAB);
		}
		break;
	default:
		break;
	}

	switch (bossStruct.handRight.state)
	{
	case SLAP_PREPARING:

		UpdateStateHand(RIGHT_HAND, VERTICAL_SLAP);

		break;
	case GUN_PREPARING:

		UpdateStateHand(RIGHT_HAND, GUN);

		break;
	case GRAB_PREPARING:

		UpdateStateHand(RIGHT_HAND, GRAB);

		break;
	default:
		break;
	}

	if (bossStruct.waiting <= 0.0f)
	{
		bossStruct.startAction = sfTrue;
	}
	else
	{
		bossStruct.waiting -= _dt;
	}
}

void MakeTheAttack(float _dt)
{
	if (bossStruct.handLeft.hp > 0)
	{
		switch (bossStruct.handLeft.state)
		{
		case VERTICAL_SLAP:
			SlapVertical(_dt, bossStruct.handLeft.sprite);
			break;
		case GUN:
			GunFire(bossStruct.handLeft, _dt);
			break;
		case GRAB:
			if (bossStruct.canGrab > 0.0f)
			{
				bossStruct.canGrab -= _dt;
			}
			else
			{
				bossStruct.actionOver = sfTrue;
				bossStruct.canGrab = GRABBING_TIMER;
			}
			break;
		default:
			break;
		}
	}

	if (bossStruct.handRight.hp > 0)
	{
		switch (bossStruct.handRight.state)
		{
		case VERTICAL_SLAP:
			SlapVertical(_dt, bossStruct.handRight.sprite);
			break;
		case GUN:
			GunFire(bossStruct.handRight, _dt);
			break;
		case GRAB:
			if (bossStruct.canGrab > 0.0f)
			{
				bossStruct.canGrab -= _dt;
			}
			else
			{
				bossStruct.actionOver = sfTrue;
				bossStruct.canGrab = GRABBING_TIMER;
			}
			break;
		default:
			break;
		}
	}



}

void PrepareForSlap(sfSprite* _spriteHand)
{
	if (bossStruct.handLeft.state == bossStruct.handRight.state)
	{
		sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { sfSprite_getPosition(GetPlayer()->sprite).x + SLAP_HAND_PREP_OFFSET, SCREEN_HEIGHT / 2 - DISTANCE_POSITION_FROM_CENTER });
		sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { sfSprite_getPosition(GetPlayer()->sprite).x - SLAP_HAND_PREP_OFFSET, SCREEN_HEIGHT / 2 - DISTANCE_POSITION_FROM_CENTER });

	}
	else
	{
		sfSprite_setPosition(_spriteHand, (sfVector2f) { sfSprite_getPosition(GetPlayer()->sprite).x, SCREEN_HEIGHT / 2 - DISTANCE_POSITION_FROM_CENTER });
	}
}


void SlapVertical(float _dt, sfSprite* _handSprite)
{
	if (sfSprite_getPosition(_handSprite).y <= SCREEN_HEIGHT - SLAP_HAND_BOTT_OFFSET)
	{
		sfSprite_move(_handSprite, (sfVector2f) { 0, VERTICAL_SLAP_SPEED* _dt });

		//player loose HP
		if (DoesPlayerCollideWithHitbox())
		{
			EditPlayerHP(-1);


		}
	}
	else
	{

		sfVector2f handPos = sfSprite_getPosition(_handSprite);
		handPos.y += OFFSET_BOSS_SMOKE;

		AddMisc(MISC_SMOKE, handPos);
		bossStruct.actionOver = sfTrue;
	}
}

PhaseEnum GetActualPhase(void)
{
	return bossStruct.phase;
}

int GetLifeOfHand(int _hand)
{
	if (_hand == LEFT_HAND)
	{
		return bossStruct.handLeft.hp;
	}
	else
	{
		return bossStruct.handRight.hp;
	}
}

int GetLifeOfHead(void)
{
	return bossStruct.head.hp;
}

void GunFire(HandStruct _hand, float _dt)
{
	if (bossStruct.handLeft.state == bossStruct.handRight.state)
	{
		sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { SCREEN_WIDTH - GUN_POSITION_X_DECAL, sfSprite_getPosition(GetPlayer()->sprite).y + GUN_POSITION_Y_DECAL });
		sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { GUN_POSITION_X_DECAL, sfSprite_getPosition(GetPlayer()->sprite).y + GUN_POSITION_Y_DECAL });
	}
	else if (bossStruct.handLeft.state == GUN)
	{
		sfSprite_setPosition(bossStruct.handLeft.sprite, (sfVector2f) { SCREEN_WIDTH - GUN_POSITION_X_DECAL, sfSprite_getPosition(GetPlayer()->sprite).y + GUN_POSITION_Y_DECAL });
	}
	else if (bossStruct.handRight.state == GUN)
	{
		sfSprite_setPosition(bossStruct.handRight.sprite, (sfVector2f) { GUN_POSITION_X_DECAL, sfSprite_getPosition(GetPlayer()->sprite).y + GUN_POSITION_Y_DECAL });
	}


	if (_hand.gunAnim.frame == 10)
	{
		bossStruct.bulletAmount++;
		sfVector2f bossPos = sfSprite_getPosition(_hand.sprite);
		bossPos.y += BULLET_Y_POSITION;
		sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
		playerPos.y += BULLET_Y_POSITION;
		AddBullet(BULLET_BOSS, bossPos, playerPos);
	}

	if (bossStruct.bulletAmount == BULLET_MAX_FIRE)
	{
		bossStruct.actionOver = sfTrue;
	}
}

void DamageHand(float _dt)
{
	if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetWeaponHitbox(0)), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.leftHandDamageHitbox))
		&& IsWeaponAttacking()
		&& bossStruct.handLeft.canDamage <= 0.0f
		&& bossStruct.handLeft.hp > 0
		&& bossStruct.blink[LEFT_HAND].isInvincible == sfFalse)
	{
		bossStruct.handLeft.hp -= CalculDamageBoss();
		AddPickup(P_FILE, (sfVector2f) { CELL_WIDTH * 5 + rand() % SCREEN_WIDTH / 2, CELL_HEIGHT * 5 + rand() % SCREEN_HEIGHT / 2 });
		bossStruct.blink[LEFT_HAND].isInvincible = sfTrue;
		if (bossStruct.handLeft.hp <= 0)
		{
			UpdateStateHand(LEFT_HAND, HAND_DEAD);

			ResetEverything();
			bossStruct.timeStop = -1.0f;
			AddPickup(P_LIFE, sfSprite_getPosition(bossStruct.handLeft.sprite));


		}
		bossStruct.handLeft.canDamage = DAMAGE_AGAIN_TIMER;
		CheckMiscBasedOnWeapon(sfSprite_getPosition(bossStruct.handLeft.sprite));

	}

	else
	{
		bossStruct.handLeft.canDamage -= _dt;
	}


	if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetWeaponHitbox(0)), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.rightHandDamageHitbox))
		&& IsWeaponAttacking()
		&& bossStruct.handRight.canDamage <= 0.0f
		&& bossStruct.handRight.hp > 0
		&& bossStruct.blink[RIGHT_HAND].isInvincible == sfFalse)
	{
		bossStruct.handRight.hp -= CalculDamageBoss();
		AddPickup(P_FILE, (sfVector2f) { CELL_WIDTH * 5 + rand() % SCREEN_WIDTH / 2, CELL_HEIGHT * 5 + rand() % SCREEN_HEIGHT / 2 });
		bossStruct.blink[RIGHT_HAND].isInvincible = sfTrue;
		if (bossStruct.handRight.hp <= 0)
		{
			UpdateStateHand(RIGHT_HAND, HAND_DEAD);

			ResetEverything();
			bossStruct.timeStop = -1.0f;
			AddPickup(P_LIFE, sfSprite_getPosition(bossStruct.handRight.sprite));


		}
		bossStruct.handRight.canDamage = DAMAGE_AGAIN_TIMER;
		CheckMiscBasedOnWeapon(sfSprite_getPosition(bossStruct.handRight.sprite));
	}

	else
	{
		bossStruct.handRight.canDamage -= _dt;
	}

}

void UpdateBossBlink(BossStruct* _enemy, int _bodyPart, float _dt)
{
	AttackHand state = NO_ATTACK;
	int index = 0;

	switch (_bodyPart)
	{
	case LEFT_HAND:
		state = bossStruct.handLeft.state;
		index = LEFT_HAND;
		break;
	case RIGHT_HAND:
		state = bossStruct.handRight.state;
		index = RIGHT_HAND;
		break;
	case HEAD_PART:
		if (bossStruct.head.hp <= 0)
		{
			state = HAND_DEAD;
		}
		index = HEAD_PART - 1;
		break;
	default:
		break;
	}

	if (_enemy->blink[index].isInvincible && state != HAND_DEAD)
	{
		_enemy->blink[index].blinkTimer += _dt;
		if (_enemy->blink[index].blinkTimer < BLINK_SPEED)
		{
			_enemy->blink[index].alpha = BLINK_ALPHA_MIN;
		}
		else if (_enemy->blink[index].blinkTimer < BLINK_SPEED * 2)
		{
			_enemy->blink[index].alpha = BLINK_ALPHA_FULL;
		}
		else
		{
			_enemy->blink[index].blinkTimer = 0;
		}
	}
	else
	{
		_enemy->blink[index].alpha = BLINK_ALPHA_FULL;
	}
}

void UpdateInvincibility(float _dt)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		if (bossStruct.blink[i].isInvincible == sfTrue)
		{
			bossStruct.blink[i].timer += _dt;
			if (bossStruct.blink[i].timer >= INVISIBILY_TIMER)
			{
				bossStruct.blink[i].timer = 0.0f,
					bossStruct.blink[i].blinkTimer = 0.0f;
				bossStruct.blink[i].isInvincible = sfFalse;
			}
		}
	}
}


void PlayerGetGrabbed(float _dt)
{
	if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.leftGrabHitbox))
		&& bossStruct.handLeft.state == GRAB)
	{
		UpdateAnimation(&bossStruct.handLeft.grabing, _dt);
		SetPlayerState(IDLE);
		SetPlayerActivity(sfFalse);

		if (bossStruct.handRight.state != VERTICAL_SLAP)
		{
			bossStruct.handLeft.grabing.frame = 0;
			PrepareForSlap(bossStruct.handRight.sprite);
			UpdateStateHand(RIGHT_HAND, VERTICAL_SLAP);
		}

	}

	else if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetPlayer()->hitbox), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.rightGrabHitbox))
		&& bossStruct.handRight.state == GRAB)
	{
		UpdateAnimation(&bossStruct.handRight.grabing, _dt);
		SetPlayerState(IDLE);
		SetPlayerActivity(sfFalse);

		if (bossStruct.handLeft.state != VERTICAL_SLAP)
		{
			bossStruct.handRight.grabing.frame = 0;
			PrepareForSlap(bossStruct.handLeft.sprite);
			UpdateStateHand(LEFT_HAND, VERTICAL_SLAP);
		}
	}
}

void DamageHead(float _dt)
{
	if (IsColliding2Rectangles(sfRectangleShape_getGlobalBounds(GetWeaponHitbox(0)), sfRectangleShape_getGlobalBounds(bossStruct.hitbox.headHitbox))
		&& IsWeaponAttacking()
		&& bossStruct.head.canDamage <= 0.0f && 
		bossStruct.blink[HEAD_PART - 1].isInvincible == sfFalse)
	{
		bossStruct.head.hp -= CalculDamageBoss();
		bossStruct.head.canDamage = DAMAGE_AGAIN_TIMER;
		bossStruct.blink[HEAD_PART - 1].isInvincible = sfTrue;
	}
	if (bossStruct.head.hp <= 0)
	{
		bossStruct.phase = BOSS_DEFEATED;
	}
	else
	{
		bossStruct.head.canDamage -= _dt;
	}
}

int CalculDamageBoss(void)
{
	if (GetSizeWeapon() == STRETCH_SMALL)
	{
		SetRandomCamShake(5, 1, 0.2f);
		return DAMAGE_TO_BOSE_SMALL;
	}
	else if (GetSizeWeapon() == STRETCH_MEDIUM)
	{
		SetRandomCamShake(10, 1, 0.3f);
		return DAMAGE_TO_BOSE_MEDIUM;
	}
	else if (GetSizeWeapon() == STRETCH_BIG)
	{
		SetRandomCamShake(15, 1, 0.3f);
		return DAMAGE_TO_BOSE_BIG;
	}
}
