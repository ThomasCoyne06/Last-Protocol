#include "Bullet.h"

BulletData bullet;

//void CreateEnemyAnimation(BulletObj* _bullet);
void CreateBulletAnimation(BulletObj* _bullet, BulletType _type);
BulletType ScanBulletType(FILE* const _file, const char* const _format);
void AddBulletHitbox(BulletObj* _obj);
void CheckBulletTimer(BulletObj* _bullet, unsigned _index, float _dt);
void SetBulletAngle(BulletObj* _bullet, sfVector2f _startPosition, sfVector2f _targetPosition);

void SetBulletOrigin(BulletObj* _bullet, sfIntRect _firstFrame, sfVector2f _offset, int _typeID);


void UpdateBuleltMove(BulletObj* _obj, float _dt);

void UpdateBulletHitbox(BulletObj* _obj);

void AddBullet(BulletType _type, sfVector2f _startPosition, sfVector2f _targetPosition)
{
	BulletObj* newBullet = (BulletObj*)calloc(1, sizeof(BulletObj));
	if (newBullet != NULL)
	{
		newBullet->isCreated = sfTrue;
		newBullet->type = _type;

		CreateBulletAnimation(newBullet, _type);
		sfSprite_setPosition(newBullet->sprite, _startPosition);

		SetBulletAngle(newBullet, _startPosition, _targetPosition);

		AddBulletHitbox(newBullet);

		Element* element = CreateElement((void*)newBullet);
		InsertElement(bullet.list, element, 0);

		PlaySound(FIRE_ENEMY, _startPosition, sfFalse, sfTrue);
	}
	else
	{
		printf("calloc failed in AddBullet");
	}
}

void LoadBullet(void)
{
	bullet.texture = sfTexture_createFromFile("Assets/Textures/Game/Bullet/Sheet.png", NULL);
	bullet.list = CreateList();
}

void UpdateBullet(float _dt)
{
	Element* actualElement = bullet.list->last;
	int index = bullet.list->count - 1;

	while (actualElement != NULL && bullet.list->count > 0)
	{
		BulletObj* obj = actualElement->data;
		UpdateAnimation(&obj->animationList[obj->type], _dt);
		UpdateBuleltMove(obj, _dt);
		UpdateBulletHitbox(obj);

		actualElement = actualElement->prev;
		//DELET ZONE
		CheckBulletTimer(obj, index, _dt);
		////////////
		index--;
	}
}

void DrawBullet(sfRenderWindow* _window)
{
	Element* actualElement = bullet.list->first;

	while (actualElement != NULL && bullet.list->count > 0)
	{
		BulletObj* obj = actualElement->data;
		DrawAnimation(&obj->animationList[obj->type], _window);
		actualElement = actualElement->next;
	}
}

void CleanupBullet(void)
{
	Element* actualElement = bullet.list->first;

	while (actualElement != NULL)
	{
		BulletObj* obj = actualElement->data;
		sfSprite_destroy(obj->sprite);
		actualElement = actualElement->next;
	}

	DeleteList(bullet.list);

	sfTexture_destroy(bullet.texture);

	bullet = (BulletData){ 0 };
}

void DestroyBullet(List* _list, unsigned _index)
{
	BulletObj* obj = GetElement(_list, _index)->data;
	sfSprite_destroy(obj->sprite);
	obj->isCreated = sfFalse;
	RemoveElement(_list, _index);
}
void CreateBulletAnimation(BulletObj* _bullet, BulletType _type)
{

	char bulletTypeNames[][100] =
	{
	"BASIC_BULLET",
	"BOSS_BULLET",
	};

	char fileName[200];
	sprintf_s(fileName, sizeof(fileName), "Assets/Settings/BulletSettings/%s_Settings.txt", bulletTypeNames[_type]);

	FILE* propsFile = NULL;

	if (fopen_s(&propsFile, fileName, "r") == 0)
	{
		SkipLinesUntilCharacter(propsFile, '=');

		BulletType typeID = ScanBulletType(propsFile, "name");

		sfIntRect firstFrame = { 0 };
		int nbFrame = 0;
		int framerate = 0;
		sfBool isLooping;

		fscanf_s(propsFile, "first frame rect = %d/%d/%d/%d\n",
			&firstFrame.left, &firstFrame.top,
			&firstFrame.width, &firstFrame.height);

		_bullet->sprite = CreateSpriteFromSpriteSheet(bullet.texture, firstFrame, sfFalse, SCALE);


		fscanf_s(propsFile, "number of frame = %d\n", &nbFrame);
		fscanf_s(propsFile, "framerate = %d\n", &framerate);

		isLooping = ScanBoolean(propsFile, "looping");

		sfVector2f offset = { 0 };
		fscanf_s(propsFile, "origin offset = %f/%f\n", &offset.x, &offset.y);

		_bullet->animationList[typeID] = CreateAnimation(_bullet->sprite, nbFrame, framerate, firstFrame, isLooping);
		
		SetBulletOrigin(_bullet, firstFrame, offset, typeID);

		fclose(propsFile);
	}
	else
	{
		printf("Open Bullet file error !!!\n");

	}

}
BulletType ScanBulletType(FILE* const _file, const char* const _format)
{
	char stateName[50] = { '\0' };
	char text[50] = { '\0' };
	BulletType typeID = EOF;

	strcpy_s(text, sizeof(text), _format);
	strcat_s(text, sizeof(text), " = %49s\n");
	fscanf_s(_file, text, stateName, (unsigned)sizeof(stateName));

	typeID = strcmp(stateName, "BASIC_BULLET") == 0 ? BULLET_BASIC : typeID;
	typeID = strcmp(stateName, "BOSS_BULLET") == 0 ? BULLET_BOSS : typeID;


	return typeID;
}
void AddBulletHitbox(BulletObj* _obj)
{

	sfIntRect firstFrame = _obj->animationList[_obj->type].firstFrame;

	_obj->hitboxSize.x = firstFrame.width;
	_obj->hitboxSize.y = firstFrame.height;

	sfFloatRect rect = { 0, 0, _obj->hitboxSize.x, _obj->hitboxSize.y };
	_obj->hitbox = CreateHitbox(rect, sfFalse, sfRed, SCALE);
	sfVector2f recPos = sfRectangleShape_getPosition(_obj->hitbox);
	_obj->circle = CreateCircle(recPos, 5.f, sfTrue, sfBlue, 0.f, sfTransparent);

	if (_obj->type == BULLET_BOSS)
	{
		sfRectangleShape_setOrigin(_obj->hitbox, (sfVector2f) { _obj->hitboxSize.x/2, _obj->hitboxSize.y / 2 });
	}
	else
	{
		sfRectangleShape_setOrigin(_obj->hitbox, (sfVector2f) { 0.f, _obj->hitboxSize.y / 2 });
	}
}

void UpdateBuleltMove(BulletObj* _obj, float _dt)
{
	sfVector2f bulletMove = { _obj->velocity.x * _dt, _obj->velocity.y * _dt };
	sfSprite_move(_obj->sprite, bulletMove);
}
void UpdateBulletHitbox(BulletObj* _obj)
{
	sfRectangleShape_setPosition(_obj->hitbox, sfSprite_getPosition(_obj->sprite));
	sfCircleShape_setPosition(_obj->circle, sfSprite_getPosition(_obj->sprite));
}
void CheckBulletTimer(BulletObj* _bullet, unsigned _index, float _dt)
{
	if (_bullet->timer > BULLET_MAX_TIMER)
	{
		DestroyBullet(bullet.list, _index);
		_bullet->timer = 0;
	}
	else
	{
		_bullet->timer += _dt;
	}
}
void SetBulletAngle(BulletObj* _bullet, sfVector2f _startPosition, sfVector2f _targetPosition)
{

	float angle = GetAngle(_startPosition, _targetPosition);
	_bullet->velocity.x = BULLET_SPEED * cosf(angle);
	_bullet->velocity.y = BULLET_SPEED * sinf(angle);

	if (_bullet->type == BULLET_BASIC)
	{
		sfSprite_setRotation(_bullet->sprite, ConvertRadToDeg(angle) - 180.f);

	}
}
void SetBulletOrigin(BulletObj* _bullet, sfIntRect _firstFrame, sfVector2f _offset, int _typeID)
{
	sfVector2f origin = { 0 };

	if (_bullet->type == BULLET_BOSS)
	{
		origin = (sfVector2f){ _firstFrame.height / 2 + _offset.x,_firstFrame.height / 2 + _offset.y };
	}
	else
	{

		origin = (sfVector2f){ 0.f + _offset.x,_firstFrame.height / 2 + _offset.y };

	}
	sfSprite_setOrigin(_bullet->animationList[_typeID].sprite, origin);
}

void DrawBulletHitbox(sfRenderWindow* _windows)
{
	Element* actualElement = bullet.list->first;

	while (actualElement != NULL && bullet.list->count > 0)
	{
		BulletObj* obj = actualElement->data;

		sfRenderWindow_drawRectangleShape(_windows, obj->hitbox, NULL);
		sfRenderWindow_drawCircleShape(_windows, obj->circle, NULL);

		actualElement = actualElement->next;
	}
}

const BulletData* GetBullet(void)
{
	return &bullet;
}

BulletData* SetBullet(void)
{
	return &bullet;
}