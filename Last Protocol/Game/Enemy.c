#include "Enemy.h"
#include "../Stats.h"

#pragma region DECLARATION OF FUNCTIONS

//       :::::::::       ::::::::::       ::::::::       :::            :::        :::::::::
//      :+:    :+:      :+:             :+:    :+:      :+:          :+: :+:      :+:    :+:
//     +:+    +:+      +:+             +:+             +:+         +:+   +:+     +:+    +:+
//    +#+    +:+      +#++:++#        +#+             +#+        +#++:++#++:    +#++:++#:
//   +#+    +#+      +#+             +#+             +#+        +#+     +#+    +#+    +#+
//  #+#    #+#      #+#             #+#    #+#      #+#        #+#     #+#    #+#    #+#
// #########       ##########       ########       ########## ###     ###    ###    ###

void LoadEnemySettingsFile(void);
void LoadEnemyHitbox(void);
void LoadEnemySpawnInMap(void);

void UpdateEnemyType(EnemyObj* _enemy, float _dt);
void UpdateEnemyGravity(EnemyObj* _enemy, float _dt);
void UpdateEnemyState(EnemyObj* _enemy, float _dt);
void UpdateEnemyFollow(EnemyObj* _enemy, sfVector2f _targetPos, float _dt);
void UpdateEnemyTurn(EnemyObj* _enemy);
void UpdateEnemyRoam(EnemyObj* _enemy);
void UpdateEnemyExplosionTimer(EnemyObj* _enemy, float _dt);
void UpdateEnemyFlying(EnemyObj* _enemy, float _dt);
void UpdateEnemyShoot(EnemyObj* _enemy);
void UpdateEnemyShootCooldown(EnemyObj* _enemy, float _dt);
void UpdateEnemyLanding(EnemyObj* _enemy, float _dt);
void UpdateEnemyLife(EnemyObj* _enemy);
void UpdateEnemyDestroy(EnemyObj* _enemy, int _index);
void UpdateEnemyInvulnerability(EnemyObj* _enemy, float _dt);
void UpdateEnemyBlink(EnemyObj* _enemy, float _dt);
void UpdateEnemySprite(EnemyObj* _enemy);
void UpdateEnemyMoveHoriz(EnemyObj* _enemy, float _dt);
void UpdateEnemyMoveVerti(EnemyObj* _enemy, float _dt);

void UpdateEnemyJump(EnemyObj* _enemy);

void UpdateEnemyComeDown(EnemyObj* _enemy);

void UpdateEnemyKnockback(EnemyObj* _enemy, float _dt);

void CollisionEnemyLevelBorder(EnemyObj* _enemy);
void CollisionEnemyLevelHoriz(EnemyObj* _enemy);
void CollisionEnemyLevelVerti(EnemyObj* _enemy);

sfBool CheckEnemyDetectPlayer(EnemyObj* _enemy, float _detectRange);
void SettingsEnemyStats(FILE* const _file, EnemyType _typeID);
EnemyState ScanState(FILE* const _file, const char* const _format);
void SettingsEnemyBehavior(FILE* const _file, EnemyType _typeID);
void SettingsEnemyAnimation(FILE* const _file, EnemyType _typeID);

#pragma endregion

EnemyData enemyData;

#pragma region ADD

//           :::        :::::::::       :::::::::
//        :+: :+:      :+:    :+:      :+:    :+:
//      +:+   +:+     +:+    +:+      +:+    +:+
//    +#++:++#++:    +#+    +:+      +#+    +:+
//   +#+     +#+    +#+    +#+      +#+    +#+
//  #+#     #+#    #+#    #+#      #+#    #+#
// ###     ###    #########       #########

void AddEnemy(EnemyType _type, sfVector2f _position)
{
	EnemyObj* newEnemy = (EnemyObj*)calloc(1, sizeof(EnemyObj));
	if (newEnemy != NULL)
	{
		EnemyStatistic* stats = &enemyData.statsList[_type];

		newEnemy->hitbox = sfRectangleShape_copy(enemyData.hitboxList[_type]);
		sfRectangleShape_setPosition(newEnemy->hitbox, _position);

		newEnemy->state = stats->startState;
		newEnemy->turn = stats->startTurn;
		newEnemy->life = stats->maxLife;
		newEnemy->alpha = BLINK_ALPHA_FULL;
		newEnemy->type = _type;
		newEnemy->speed = stats->speed;

		AnimationData* anim = &enemyData.animationList[_type][newEnemy->state];
		newEnemy->currentAnimation = *anim;
		newEnemy->currentAnimation.sprite = sfSprite_copy(anim->sprite);
		sfSprite_setPosition(newEnemy->currentAnimation.sprite, _position);

		Element* element = CreateElement((void*)newEnemy);
		InsertElement(enemyData.list, element, 0);
	}
	else
	{
		printf("calloc failed in AddEnemy");
	}
}

#pragma endregion

#pragma region LOAD FUNCTIONS

//       :::        ::::::::           :::        :::::::::
//      :+:       :+:    :+:        :+: :+:      :+:    :+:
//     +:+       +:+    +:+       +:+   +:+     +:+    +:+
//    +#+       +#+    +:+      +#++:++#++:    +#+    +:+
//   +#+       +#+    +#+      +#+     +#+    +#+    +#+
//  #+#       #+#    #+#      #+#     #+#    #+#    #+#
// ########## ########       ###     ###    #########

void LoadEnemy(void)
{
	enemyData.list = CreateList();
	LoadEnemySettingsFile();
	LoadEnemyHitbox();
	LoadEnemySpawnInMap();
}

void LoadEnemySettingsFile(void)
{
	char enemyNames[][10] = { "Basic", "TV", "Eye", "Jumper", "Mini" };

	for (unsigned typeID = 0; typeID < NB_ENEMY_TYPE; typeID++)
	{
		char fileName[50];
		sprintf_s(fileName, sizeof(fileName), "Assets/Settings/Enemy_%s_Settings.txt", enemyNames[typeID]);

		FILE* file;
		if (fopen_s(&file, fileName, "r") == 0)
		{
			SettingsEnemyStats(file, typeID);
			SettingsEnemyBehavior(file, typeID);
			SettingsEnemyAnimation(file, typeID);

			fclose(file);
		}
		else
		{
			printf("Open file error !!!\n");
		}
	}
}

void LoadEnemyHitbox(void)
{
	for (unsigned typeID = 0; typeID < NB_ENEMY_TYPE; typeID++)
	{
		EnemyStatistic* stats = &enemyData.statsList[typeID];
		sfFloatRect rect = { 0, 0, stats->hitboxSize.x, stats->hitboxSize.y };
		enemyData.hitboxList[typeID] = CreateHitbox(rect, sfFalse, sfRed, SCALE);
		sfRectangleShape_setOrigin(enemyData.hitboxList[typeID], (sfVector2f) {
			stats->hitboxSize.x / 2, stats->hitboxSize.y
		});
	}
}

void LoadEnemySpawnInMap(void)
{
	EntityData entity = GetLevel()->entityList[ENEMY_BASIC_SPAWN];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2, entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddEnemy(ENEMY_BASIC, position);
	}

	entity = GetLevel()->entityList[ENEMY_TV_SPAWN];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2, entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddEnemy(ENEMY_TV, entity.positionList[i]);
	}

	entity = GetLevel()->entityList[ENEMY_EYE_SPAWN];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2 , entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddEnemy(ENEMY_EYE, entity.positionList[i]);
	}

	entity = GetLevel()->entityList[ENEMY_JUMPER_SPAWN];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2, entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddEnemy(ENEMY_JUMPER, entity.positionList[i]);
	}

	entity = GetLevel()->entityList[ENEMY_MINI_SPAWN];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2, entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddEnemy(ENEMY_MINI, entity.positionList[i]);
	}
}

#pragma endregion

#pragma region UPDATE FUNCTIONS

//      :::    :::       :::::::::       :::::::::           :::    :::::::::::       ::::::::::
//     :+:    :+:       :+:    :+:      :+:    :+:        :+: :+:      :+:           :+:
//    +:+    +:+       +:+    +:+      +:+    +:+       +:+   +:+     +:+           +:+
//   +#+    +:+       +#++:++#+       +#+    +:+      +#++:++#++:    +#+           +#++:++#
//  +#+    +#+       +#+             +#+    +#+      +#+     +#+    +#+           +#+
// #+#    #+#       #+#             #+#    #+#      #+#     #+#    #+#           #+#
// ########        ###             #########       ###     ###    ###           ##########

void UpdateEnemy(float _dt)
{
	Element* actualElement = enemyData.list->last;
	int index = enemyData.list->count - 1;

	while (actualElement != NULL && enemyData.list->count > 0)
	{
		EnemyObj* obj = actualElement->data;

		UpdateEnemyState(obj, _dt);

		UpdateEnemyMoveHoriz(obj, _dt);
		CollisionEnemyLevelHoriz(obj);

		UpdateEnemyMoveVerti(obj, _dt);
		CollisionEnemyLevelVerti(obj);

		UpdateEnemySprite(obj);
		UpdateEnemyInvulnerability(obj, _dt);
		UpdateEnemyBlink(obj, _dt);
		UpdateEnemyLife(obj);
		UpdateEnemyGravity(obj, _dt);
		UpdateAnimation(&obj->currentAnimation, _dt);
		UpdateEnemyKnockback(obj, _dt);

		CollisionEnemyLevelBorder(obj);

		obj->enableGravity = sfTrue;
		actualElement = actualElement->prev;
		//DELET ZONE
		UpdateEnemyDestroy(obj, index);
		////////////
		index--;
	}
}

void UpdateEnemyGravity(EnemyObj* _enemy, float _dt)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];
	if (behavior->gravityAffect && _enemy->enableGravity)
	{
		if (_enemy->velocity.y < GRAVITY_LIMIT)
		{
			_enemy->velocity.y += (GRAVITY * _dt);
		}
	}
}

void UpdateEnemyState(EnemyObj* _enemy, float _dt)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	switch (_enemy->state)
	{
	case ENEMY_IDLE:
		UpdateEnemyTurn(_enemy);
		if (CheckEnemyDetectPlayer(_enemy, stats->detectRange))
		{
			SetEnemyState(_enemy, ENEMY_MOVE);
		}
		break;

	case ENEMY_MOVE:
	{
		sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
		if (!_enemy->isKnockback)
		{
			UpdateEnemyFollow(_enemy, playerPos, _dt);
			UpdateEnemyRoam(_enemy);
			UpdateEnemyFlying(_enemy, _dt);
			UpdateEnemyJump(_enemy);
			UpdateEnemyShootCooldown(_enemy, _dt);
		}
		UpdateEnemyTurn(_enemy);
		UpdateEnemyExplosionTimer(_enemy, _dt);
		UpdateEnemyComeDown(_enemy);
		if (!CheckEnemyDetectPlayer(_enemy, stats->agressiveRange) && behavior->unDetectPlayer)
		{
			SetEnemyState(_enemy, ENEMY_IDLE);
			_enemy->velocity.x = 0.f;
			UpdateEnemyLanding(_enemy, _dt);
		}
	}
	break;

	case ENEMY_DEAD:
		if (AnimationIsFinished(&_enemy->currentAnimation))
		{
			_enemy->isDead = sfTrue;
		}
		break;

	case ENEMY_SHOOT:
		if (!_enemy->isKnockback)
		{
			sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
			UpdateEnemyFollow(_enemy, playerPos, _dt);
			UpdateEnemyFlying(_enemy, _dt);
		}
		UpdateEnemyTurn(_enemy);
		UpdateEnemyShoot(_enemy);

		if (AnimationIsFinished(&_enemy->currentAnimation))
		{
			SetEnemyState(_enemy, ENEMY_MOVE);
		}
		break;
	case ENEMY_JUMP:
		if (AnimationIsFinished(&_enemy->currentAnimation))
		{
			if (_enemy->isJump)
			{
				_enemy->isJump = sfFalse;
				_enemy->velocity.y = -JUMP_FORCE;
				_enemy->speed += JUMP_BONUS_SPEED;
				_enemy->jumpTarget = sfSprite_getPosition(GetPlayer()->sprite);
			}
			if (!_enemy->isKnockback)
			{
				UpdateEnemyFollow(_enemy, _enemy->jumpTarget, _dt);
			}
			UpdateEnemyTurn(_enemy);
			if (_enemy->colDir.y == COLLISION_DOWN && _enemy->velocity.y >= 0)
			{
				EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
				_enemy->speed = stats->speed;
				SetEnemyState(_enemy, ENEMY_MOVE);
			}
		}
		break;
	default:
		break;
	}
}

void UpdateEnemyFollow(EnemyObj* _enemy, sfVector2f _targetPos, float _dt)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->followPlayerHoriz || behavior->followPlayerVerti)
	{
		sfVector2f enemyPos = sfRectangleShape_getPosition(_enemy->hitbox);

		float angle = GetAngle(enemyPos, _targetPos);

		if (behavior->followPlayerHoriz)
		{
			_enemy->velocity.x = _enemy->speed * cosf(angle);
		}

		if (behavior->followPlayerVerti)
		{
			_enemy->velocity.y = _enemy->speed * sinf(angle);
		}
	}
}

void UpdateEnemyTurn(EnemyObj* _enemy)
{
	if (!_enemy->isKnockback)
	{
		if (_enemy->velocity.x < 0)
		{
			_enemy->turn = ENEMY_TURN_LEFT;
		}
		else if (_enemy->velocity.x > 0)
		{
			_enemy->turn = ENEMY_TURN_RIGHT;
		}
		sfSprite_setScale(_enemy->currentAnimation.sprite, (sfVector2f) { (float)SCALE * -_enemy->turn, (float)SCALE });
	}
}

void UpdateEnemyRoam(EnemyObj* _enemy)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->roam)
	{
		if (_enemy->colDir.x != 0)
		{
			_enemy->turn = -_enemy->turn;
		}

		_enemy->velocity.x = _enemy->speed * _enemy->turn;
	}
}
void UpdateEnemyExplosionTimer(EnemyObj* _enemy, float _dt)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->explode)
	{
		_enemy->explosionCountdown += _dt;
		_enemy->currentAnimation.frameRate = expf(_enemy->explosionCountdown + 0.5f);
		if (_enemy->explosionCountdown > stats->explosionCountdownMax)
		{
			PlaySound(EYE_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
			SetEnemyState(_enemy, ENEMY_DEAD);
			_enemy->velocity = (sfVector2f){ 0 };
		}
	}

}
void UpdateEnemyFlying(EnemyObj* _enemy, float _dt)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->fly)
	{
		_enemy->altitudeTimer += _dt;
		if (_enemy->altitudeTimer >= stats->altitudeMax)
		{
			_enemy->altitudeTimer = stats->altitudeMax;
			//_enemy->velocity.y = -GRAVITY * _dt;
			_enemy->velocity.y = 0;
			_enemy->enableGravity = sfFalse;
		}
		else
		{
			_enemy->velocity.y -= (GRAVITY * stats->speedTakeoff) * _dt;
		}
	}

}

void UpdateEnemyShoot(EnemyObj* _enemy)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->shoot)
	{

		if (_enemy->currentAnimation.isFinished)
		{
			sfVector2f playerPos = sfRectangleShape_getPosition(GetPlayer()->hitbox);
			sfVector2f enemyPos = sfRectangleShape_getPosition(_enemy->hitbox);
			enemyPos.y -= OFFSET_SHOOT_Y;

			if (_enemy->turn == ENEMY_TURN_RIGHT)
			{
				enemyPos.x += OFFSET_SHOOT_X;
			}
			else
			{
				enemyPos.x -= OFFSET_SHOOT_X;
			}


			AddBullet(BULLET_BASIC, enemyPos, playerPos);
		}
	}
}

void UpdateEnemyShootCooldown(EnemyObj* _enemy, float _dt)
{
	EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->shoot)
	{
		_enemy->shootTimer += _dt;

		if (_enemy->shootTimer > stats->shootCooldown)
		{
			_enemy->shootTimer = 0.f;
			SetEnemyState(_enemy, ENEMY_SHOOT);
		}
	}
}

void UpdateEnemyLanding(EnemyObj* _enemy, float _dt)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];

	if (behavior->landing)
	{
		_enemy->altitudeTimer = 0.f;
	}
}

void UpdateEnemyLife(EnemyObj* _enemy)
{
	if (_enemy->life <= 0 && _enemy->state != ENEMY_DEAD)
	{
		EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];
		if (behavior->explode && _enemy->isKnockback) {}
		else
		{
			_enemy->velocity = (sfVector2f){ 0 };
			SetEnemyState(_enemy, ENEMY_DEAD);
		}
	}
}

void UpdateEnemyDestroy(EnemyObj* _enemy, int _index)
{
	if (_enemy->isDead)
	{
		//AddEnemyKilled();
		DestroyEnemy(_enemy, _index);
	}
}

void UpdateEnemyInvulnerability(EnemyObj* _enemy, float _dt)
{
	if (_enemy->isInvincible && _enemy->state != ENEMY_DEAD)
	{
		EnemyStatistic* stats = &enemyData.statsList[_enemy->type];
		_enemy->invulnerabilityTimer += _dt;
		if (_enemy->invulnerabilityTimer >= stats->invulnerabilityCooldown)
		{
			_enemy->invulnerabilityTimer -= stats->invulnerabilityCooldown;
			_enemy->isInvincible = sfFalse;
		}
	}
}

void UpdateEnemyBlink(EnemyObj* _enemy, float _dt)
{
	if (_enemy->isInvincible && _enemy->state != ENEMY_DEAD)
	{
		_enemy->blinkTimer += _dt;
		if (_enemy->blinkTimer < BLINK_SPEED)
		{
			_enemy->alpha = BLINK_ALPHA_MIN;
		}
		else if (_enemy->blinkTimer < BLINK_SPEED * 2)
		{
			_enemy->alpha = BLINK_ALPHA_FULL;
		}
		else
		{
			_enemy->blinkTimer = 0;
		}
	}
	else
	{
		_enemy->alpha = BLINK_ALPHA_FULL;
	}
}

void UpdateEnemySprite(EnemyObj* _enemy)
{
	sfVector2f position = sfRectangleShape_getPosition(_enemy->hitbox);
	sfSprite_setPosition(_enemy->currentAnimation.sprite, position);
}

void UpdateEnemyMoveHoriz(EnemyObj* _enemy, float _dt)
{
	sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { _enemy->velocity.x* _dt, 0 });
}

void UpdateEnemyMoveVerti(EnemyObj* _enemy, float _dt)
{
	sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { 0, _enemy->velocity.y* _dt });
}

void UpdateEnemyJump(EnemyObj* _enemy)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];
	if (behavior->jump)
	{
		if (_enemy->colDir.y == COLLISION_DOWN)
		{

			sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(_enemy->hitbox);

			sfVector2f southWest = { hitbox.left, hitbox.top + hitbox.height - 1 };
			sfVector2f southEast = { hitbox.left + hitbox.width, hitbox.top + hitbox.height - 1 };

			CellCoordinates cellSouthWest = GetLevelColumnLineWithPosition(southWest);
			CellCoordinates cellSouthEast = GetLevelColumnLineWithPosition(southEast);
			cellSouthWest.column -= JUMP_DETECT_RANGE;
			cellSouthEast.column += JUMP_DETECT_RANGE;
			if ((_enemy->velocity.x < JUMP_DETECT_VELOCITY && _enemy->velocity.x > -JUMP_DETECT_VELOCITY) || GetLevelCollide(cellSouthWest) || GetLevelCollide(cellSouthEast))
			{
				_enemy->velocity.x = 0;
				_enemy->isJump = sfTrue;
				SetEnemyState(_enemy, ENEMY_JUMP);
			}

			cellSouthWest.column += JUMP_DETECT_RANGE;
			cellSouthEast.column -= JUMP_DETECT_RANGE;
			cellSouthWest.line++;
			cellSouthEast.line++;

			if (_enemy->colDir.x != COLLISION_NONE || !GetLevelCollide(cellSouthWest) || !GetLevelCollide(cellSouthEast))
			{
				_enemy->velocity.x = 0;
				_enemy->isJump = sfTrue;
				SetEnemyState(_enemy, ENEMY_JUMP);
			}
		}
	}
}

void UpdateEnemyComeDown(EnemyObj* _enemy)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];
	if (!behavior->comeDown)
	{
		if (_enemy->colDir.y == COLLISION_DOWN)
		{

			sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(_enemy->hitbox);

			sfVector2f southWest = { hitbox.left, hitbox.top + hitbox.height - 1 };
			sfVector2f southEast = { hitbox.left + hitbox.width, hitbox.top + hitbox.height - 1 };

			CellCoordinates cellSouthWest = GetLevelColumnLineWithPosition(southWest);
			CellCoordinates cellSouthEast = GetLevelColumnLineWithPosition(southEast);

			cellSouthWest.line++;
			cellSouthEast.line++;
			if (!GetLevelCollide(cellSouthWest))
			{
				_enemy->turn = ENEMY_TURN_RIGHT;
				_enemy->velocity.x = _enemy->speed * _enemy->turn;
				sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { (float)_enemy->turn * 10, 0 });
			}
			else if (!GetLevelCollide(cellSouthEast))
			{
				_enemy->turn = ENEMY_TURN_LEFT;
				_enemy->velocity.x = _enemy->speed * _enemy->turn;
				sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { (float)_enemy->turn * 10, 0 });
			}
		}
	}
}
void UpdateEnemyKnockback(EnemyObj* _enemy, float _dt)
{
	if (_enemy->isKnockback)
	{
		_enemy->knockbackDuration += _dt;
		if (_enemy->knockbackDuration >= KNOCKBACK_ENEMY_DURATION)
		{
			_enemy->knockbackDuration = 0;
			_enemy->isKnockback = sfFalse;
			_enemy->velocity = (sfVector2f){ 0 };
		}
	}
}

#pragma endregion

#pragma region COLLISION FUNCTIONS

//       ::::::::       ::::::::       :::        :::        :::::::::::       :::::::::
//     :+:    :+:     :+:    :+:      :+:        :+:            :+:           :+:    :+:
//    +:+            +:+    +:+      +:+        +:+            +:+           +:+    +:+
//   +#+            +#+    +:+      +#+        +#+            +#+           +#+    +:+
//  +#+            +#+    +#+      +#+        +#+            +#+           +#+    +#+
// #+#    #+#     #+#    #+#      #+#        #+#            #+#           #+#    #+#
// ########       ########       ########## ########## ###########       #########

void CollisionEnemyLevelBorder(EnemyObj* _enemy)
{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(_enemy->hitbox);
	sfFloatRect levelBorder = GetLevelHitbox();

	if (hitbox.left < levelBorder.left)
	{
		_enemy->colDir.x = COLLISION_LEFT;
		_enemy->velocity.x = 0;
		sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { -hitbox.left + levelBorder.left, 0 });
	}
	else if (hitbox.left + hitbox.width > levelBorder.left + levelBorder.width)
	{
		_enemy->colDir.x = COLLISION_RIGHT;
		_enemy->velocity.x = 0;
		sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { levelBorder.left + levelBorder.width - (hitbox.left + hitbox.width), 0 });
	}

	if (hitbox.top < levelBorder.top)
	{
		_enemy->colDir.y = COLLISION_UP;
		_enemy->velocity.y = 0;
		sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { 0, -hitbox.top + levelBorder.top });
	}
	else if (hitbox.top + hitbox.height > levelBorder.top + levelBorder.height)
	{
		_enemy->colDir.y = COLLISION_DOWN;
		_enemy->velocity.y = 0;
		sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { 0, levelBorder.top + levelBorder.height - (hitbox.top + hitbox.height) });
	}
}

void CollisionEnemyLevelHoriz(EnemyObj* _enemy)
{
	//if (_enemy->velocity.x != 0)
	//{
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(_enemy->hitbox);
	LevelCollid col = CollisionTilesHoriz(hitbox);
	if (col.isColliding)
	{
		sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { col.offset, 0 });
		_enemy->velocity.x = 0;
	}
	_enemy->colDir.x = col.direction;
	//}
}

void CollisionEnemyLevelVerti(EnemyObj* _enemy)
{
	if (_enemy->velocity.y != 0)
	{
		_enemy->onGround = sfFalse;

		sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(_enemy->hitbox);
		LevelCollid col = CollisionTilesVerti(hitbox);
		if (col.isColliding)
		{
			if (col.direction == COLLISION_DOWN)
			{
				_enemy->onGround = sfTrue;
				col.offset += OFFSET_COL; //avoid sprite vibration
			}
			sfRectangleShape_move(_enemy->hitbox, (sfVector2f) { 0, col.offset });
			_enemy->velocity.y = 0;
		}
		_enemy->colDir.y = col.direction;
	}
}
#pragma endregion

#pragma region DRAW FUNCTIONS

//       :::::::::       :::::::::           :::      :::       :::
//      :+:    :+:      :+:    :+:        :+: :+:    :+:       :+:
//     +:+    +:+      +:+    +:+       +:+   +:+   +:+       +:+
//    +#+    +:+      +#++:++#:       +#++:++#++:  +#+  +:+  +#+
//   +#+    +#+      +#+    +#+      +#+     +#+  +#+ +#+#+ +#+
//  #+#    #+#      #+#    #+#      #+#     #+#   #+#+# #+#+#
// #########       ###    ###      ###     ###    ###   ###

void DrawEnemy(sfRenderWindow* _window)
{
	Element* actualElement = enemyData.list->first;

	while (actualElement != NULL && enemyData.list->count > 0)
	{
		EnemyObj* obj = actualElement->data;
		if (obj->state != ENEMY_DEAD)
		{
			sfSprite_setColor(obj->currentAnimation.sprite, sfColor_fromRGBA(255, 255, 255, obj->alpha));
			if (obj->isInvincible)
			{
				sfSprite_setColor(obj->currentAnimation.sprite, sfColor_fromRGBA(255, 0, 0, obj->alpha));
			}
		}
		DrawAnimation(&obj->currentAnimation, _window);
		actualElement = actualElement->next;
	}
}

void DrawEnemyHitbox(sfRenderWindow* _windows)
{
	Element* actualElement = enemyData.list->first;

	while (actualElement != NULL && enemyData.list->count > 0)
	{
		EnemyObj* obj = actualElement->data;
		sfRenderWindow_drawRectangleShape(_windows, obj->hitbox, NULL);
		actualElement = actualElement->next;
	}
}
void DrawEnemyHitboxOrigin(sfRenderWindow* _window)
{

	Element* actualElement = enemyData.list->first;

	while (actualElement != NULL && enemyData.list->count > 0)
	{
		EnemyObj* obj = actualElement->data;
		sfVector2f position = sfRectangleShape_getPosition(obj->hitbox);
		sfCircleShape* origin = CreateCircle(position, 4, sfTrue, sfRed, 0, sfTransparent);
		sfRenderWindow_drawCircleShape(_window, origin, NULL);

		sfCircleShape_destroy(origin);
		origin = NULL;

		sfVector2f position2 = sfRectangleShape_getPosition(obj->hitbox);
		sfCircleShape* origin2 = CreateCircle(position2, 4, sfTrue, sfBlue, 0, sfTransparent);
		sfRenderWindow_drawCircleShape(_window, origin2, NULL);

		sfCircleShape_destroy(origin2);
		origin2 = NULL;
		actualElement = actualElement->next;
	}

}
#pragma endregion

#pragma region CLEANUP FUNCTIONS

//       ::::::::       :::        ::::::::::           :::        ::::    :::      :::    :::       :::::::::
//     :+:    :+:      :+:        :+:                :+: :+:      :+:+:   :+:      :+:    :+:       :+:    :+:
//    +:+             +:+        +:+               +:+   +:+     :+:+:+  +:+      +:+    +:+       +:+    +:+
//   +#+             +#+        +#++:++#         +#++:++#++:    +#+ +:+ +#+      +#+    +:+       +#++:++#+
//  +#+             +#+        +#+              +#+     +#+    +#+  +#+#+#      +#+    +#+       +#+
// #+#    #+#      #+#        #+#              #+#     #+#    #+#   #+#+#      #+#    #+#       #+#
// ########       ########## ##########       ###     ###    ###    ####       ########        ###

void CleanupEnemy(void)
{
	for (unsigned i = 0; i < NB_ENEMY_TYPE; i++)
	{
		for (unsigned j = 0; j < NB_ENEMY_STATE; j++)
		{
			sfSprite_destroy(enemyData.animationList[i][j].sprite);
		}
	}

	for (unsigned i = 0; i < NB_ENEMY_TYPE; i++)
	{
		sfRectangleShape_destroy(enemyData.hitboxList[i]);
	}

	Element* actualElement = enemyData.list->first;
	while (actualElement != NULL)
	{
		EnemyObj* obj = actualElement->data;
		sfRectangleShape_destroy(obj->hitbox);
		sfSprite_destroy(obj->currentAnimation.sprite);
		actualElement = actualElement->next;
	}
	DeleteList(enemyData.list);

	sfTexture_destroy(enemyData.texture);

	enemyData = (EnemyData){ 0 };
}
#pragma endregion

#pragma region OTHER FUNCTIONS

//       ::::::::   :::::::::::       :::    :::       ::::::::::       :::::::::
//     :+:    :+:      :+:           :+:    :+:       :+:              :+:    :+:
//    +:+    +:+      +:+           +:+    +:+       +:+              +:+    +:+
//   +#+    +:+      +#+           +#++:++#++       +#++:++#         +#++:++#:
//  +#+    +#+      +#+           +#+    +#+       +#+              +#+    +#+
// #+#    #+#      #+#           #+#    #+#       #+#              #+#    #+#
// ########       ###           ###    ###       ##########       ###    ###

sfBool CheckEnemyDetectPlayer(EnemyObj* _enemy, float _detectRange)
{
	EnemyBehavior* behavior = &enemyData.behaviorList[_enemy->type];
	if (behavior->detectPlayer)
	{
		sfVector2f enemyPos = sfRectangleShape_getPosition(_enemy->hitbox);
		sfVector2f playerPos = sfSprite_getPosition(GetPlayer()->sprite);
		float distance = GetDistanceBetweenPoints(enemyPos, playerPos);

		return distance <= _detectRange;
	}
	return sfFalse;
}

void DestroyEnemy(EnemyObj* _enemy, unsigned _index)
{
	sfSprite_destroy(_enemy->currentAnimation.sprite);
	sfRectangleShape_destroy(_enemy->hitbox);

	RemoveElement(enemyData.list, _index);
}

void EditEnemyHP(EnemyObj* _enemy, int _editHP)
{
	if (!_enemy->isInvincible)
	{
		_enemy->life += _editHP;
		_enemy->isInvincible = sfTrue;
	}
}

EnemyState ScanState(FILE* const _file, const char* const _format)
{
	char stateName[15] = { '\0' };
	char text[50] = { '\0' };
	EnemyState stateID = EOF;

	strcpy_s(text, sizeof(text), _format);
	strcat_s(text, sizeof(text), " = %14s\n");
	fscanf_s(_file, text, stateName, (unsigned)sizeof(stateName));
	stateID = strcmp(stateName, "IDLE") == 0 ? ENEMY_IDLE : stateID;
	stateID = strcmp(stateName, "MOVE") == 0 ? ENEMY_MOVE : stateID;
	stateID = strcmp(stateName, "DEAD") == 0 ? ENEMY_DEAD : stateID;
	stateID = strcmp(stateName, "SHOOT") == 0 ? ENEMY_SHOOT : stateID;
	stateID = strcmp(stateName, "LANDING") == 0 ? ENEMY_LANDING : stateID;
	stateID = strcmp(stateName, "JUMP") == 0 ? ENEMY_JUMP : stateID;
	return stateID;
}

// .-. .-. .-. .-. .-.
// `-.  |  |-|  |  `-.
// `-'  '  ` '  '  `-'

void SettingsEnemyStats(FILE* const _file, EnemyType _typeID)
{
	//SkipUselessLine(_file, '=');
	SkipLinesUntilCharacter(_file, '=');

	EnemyStatistic* stats = &enemyData.statsList[_typeID];
	fscanf_s(_file, "life = %d\n", &stats->maxLife);
	fscanf_s(_file, "my crash damage = %d\n", &stats->myCrashDamage);
	fscanf_s(_file, "her crash damage = %d\n", &stats->herCrashDamage);
	fscanf_s(_file, "shoot damage = %d\n", &stats->shootDamage);
	fscanf_s(_file, "shoot cooldown = %f\n", &stats->shootCooldown);
	fscanf_s(_file, "invulnerability cooldown = %f\n", &stats->invulnerabilityCooldown);
	fscanf_s(_file, "explosion damage = %d\n", &stats->explosionDamage);
	fscanf_s(_file, "explosion timer = %f\n", &stats->explosionCountdownMax);
	fscanf_s(_file, "speed = %f\n", &stats->speed);
	fscanf_s(_file, "speed takeoff = %f\n", &stats->speedTakeoff);
	fscanf_s(_file, "altitude timer = %f\n", &stats->altitudeMax);
	fscanf_s(_file, "detect range = %f\n", &stats->detectRange);
	fscanf_s(_file, "agressive range = %f\n", &stats->agressiveRange);
	fscanf_s(_file, "fall vulnerability = %f\n", &stats->fallVulnerability);
	fscanf_s(_file, "hitbox size = %f/%f\n", &stats->hitboxSize.x, &stats->hitboxSize.y);

	stats->startState = ScanState(_file, "start state");

	char turnDir[6] = { '\0' };
	fscanf_s(_file, "start turn = %5s\n", turnDir, (unsigned)sizeof(turnDir));
	stats->startTurn = strcmp(turnDir, "left") == 0 ? ENEMY_TURN_LEFT : ENEMY_TURN_RIGHT;
}

// .-. .-. . . .-. . . .-. .-. .-.
// |(  |-  |-| |-| | |  |  | | |(
// `-' `-' ' ` ` ' `.' `-' `-' ' '

void SettingsEnemyBehavior(FILE* const _file, EnemyType _typeID)
{
	//SkipUselessLine(_file, '=');
	SkipLinesUntilCharacter(_file, '=');

	EnemyBehavior* behavior = &enemyData.behaviorList[_typeID];
	behavior->detectPlayer = ScanBoolean(_file, "detect player");
	behavior->unDetectPlayer = ScanBoolean(_file, "undetect player");
	behavior->followPlayerHoriz = ScanBoolean(_file, "follow player horizontal");
	behavior->followPlayerVerti = ScanBoolean(_file, "follow player vertical");
	behavior->gravityAffect = ScanBoolean(_file, "gravity affect");
	behavior->roam = ScanBoolean(_file, "roam");
	behavior->comeDown = ScanBoolean(_file, "come down");
	behavior->shoot = ScanBoolean(_file, "shoot");
	behavior->jump = ScanBoolean(_file, "jump");
	behavior->fly = ScanBoolean(_file, "fly");
	behavior->fallDamage = ScanBoolean(_file, "fall damage");
	behavior->explode = ScanBoolean(_file, "explode");
	behavior->landing = ScanBoolean(_file, "landing");
}

// .-. . . .-. .  . .-. .-. .-. .-. . .
// |-| |\|  |  |\/| |-|  |   |  | | |\|
// ` ' ' ` `-' '  ` ` '  '  `-' `-' ' `

void SettingsEnemyAnimation(FILE* const _file, EnemyType _typeID)
{
	enemyData.texture = sfTexture_createFromFile("Assets/Textures/Game/Enemy/Sheet.png", NULL);

	//SkipUselessLine(_file, '=');
	SkipLinesUntilCharacter(_file, '=');


	unsigned nbAnimation = 0;
	fscanf_s(_file, "number of animation = %d\n", &nbAnimation);

	//SkipUselessLine(_file, '=');
	SkipLinesUntilCharacter(_file, '=');


	for (unsigned i = 0; i < nbAnimation; i++)
	{

		EnemyState stateID = ScanState(_file, "name");
		AnimationData* anim = &enemyData.animationList[_typeID][stateID];

		fscanf_s(_file, "first frame rect = %d/%d/%d/%d\n",
			&anim->firstFrame.left, &anim->firstFrame.top,
			&anim->firstFrame.width, &anim->firstFrame.height);

		anim->sprite = CreateSpriteFromSpriteSheet(enemyData.texture, anim->firstFrame, sfFalse, SCALE);
		LoadAnimation(anim);


		fscanf_s(_file, "number of frame = %d\n", &anim->nbFrame);
		fscanf_s(_file, "framerate = %d\n", &anim->frameRate);

		anim->isLooping = ScanBoolean(_file, "looping");

		sfVector2f offset = { 0 };
		fscanf_s(_file, "origin offset = %f/%f\n", &offset.x, &offset.y);

		sfSprite_setOrigin(anim->sprite, (sfVector2f)
		{
			anim->firstFrame.width / 2 + offset.x, anim->firstFrame.height + offset.y
		});

		//SkipUselessLine(_file, '=');
		SkipLinesUntilCharacter(_file, '=');

	}
}

void PlayEnemyDeathSound(EnemyType _enemyType)
{
	switch (_enemyType)
	{
	case ENEMY_BASIC:
		PlaySound(MINI_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_TV:
		PlaySound(BIG_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_EYE:
		PlaySound(EYE_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_JUMPER:
		PlaySound(JUMPER_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_MINI:
		PlaySound(MINI_DIE, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
	default:
		break;
	}
}

void PlayEnemyHitSound(EnemyType _enemyType)
{
	switch (_enemyType)
	{
	case ENEMY_TV:
		PlaySound(BIG_HIT, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_BASIC:
		PlaySound(BASIC_HIT, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	case ENEMY_JUMPER:
		PlaySound(JUMPER_HIT, sfSprite_getPosition(GetPlayer()->sprite), sfFalse, sfFalse);
		break;
	default:
		break;

	}
}

// .-. .-. .-.
// |.. |-   |
// `-' `-'  '

const EnemyData* GetEnemy(void)
{
	return &enemyData;
}

// .-. .-. .-.
// `-. |-   |
// `-' `-'  '

EnemyData* SetEnemy(void)
{
	return &enemyData;
}

void SetEnemyState(EnemyObj* _enemy, EnemyState _state)
{
	_enemy->state = _state;
	sfSprite_destroy(_enemy->currentAnimation.sprite);
	_enemy->currentAnimation = enemyData.animationList[_enemy->type][_state];
	_enemy->currentAnimation.sprite = sfSprite_copy(enemyData.animationList[_enemy->type][_state].sprite);
	UpdateEnemyTurn(_enemy);
	LoadAnimation(&_enemy->currentAnimation);
}

void EnemyKnockback(EnemyObj* _enemy, float _knockbackPower, float _angle)
{
	_enemy->velocity.x = _knockbackPower * cosf(_angle);
	_enemy->velocity.y = _knockbackPower * sinf(_angle);
	_enemy->isKnockback = sfTrue;
}

#pragma endregion
