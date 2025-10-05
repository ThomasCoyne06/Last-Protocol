#include "Miscellenous.h"

MiscData  misc;

void CreateMiscAnimation(MiscObj* _newMisc, MiscType _type);
void CheckMiscAnimationFinished(MiscObj* _misc, int _index);
MiscType ScanMiscType(FILE* const _file, const char* const _format);

void AddMiscOrigin(MiscObj* _misc);
void UpdateMiscOrigin(MiscObj* _misc);
void LoadOrigins(MiscObj* _misc, MiscType _type, sfVector2f _offset, sfIntRect _firstFrame, int _typeID);


void AddMisc(MiscType _miscType, sfVector2f _startPosition)
{
	MiscObj* newMisc = (MiscObj*)calloc(1, sizeof(MiscObj));
	if (newMisc != NULL)
	{
		newMisc->sprite = CreateSprite(misc.texture, _startPosition, sfFalse, SCALE);
		newMisc->type = _miscType;

		CreateMiscAnimation(newMisc, _miscType);
		AddMiscOrigin(newMisc);


		Element* element = CreateElement((void*)newMisc);
		InsertElement(misc.list, element, 0);
	}
	else
	{
		printf("calloc failed in CreateMisc");
	}

}

void LoadMisc(void)
{
	misc.texture = sfTexture_createFromFile("Assets/Textures/Miscellenous/Sheet.png", NULL);
	misc.list = CreateList();


}
void UpdateMisc(float _dt)
{

	Element* actualElement = misc.list->last;
	int nbMisc = misc.list->count - 1;

	while (actualElement != NULL && misc.list->count > 0)
	{
		MiscObj* miscObj = actualElement->data;

		UpdateAnimation(&miscObj->animationList[miscObj->type], _dt);
		UpdateMiscOrigin(miscObj);
		actualElement = actualElement->prev;
		CheckMiscAnimationFinished(miscObj, nbMisc);
		nbMisc--;
	}
}
void DrawMisc(sfRenderWindow* _window)
{
	Element* actualElement = misc.list->first;

	while (actualElement != NULL && misc.list->count > 0)
	{
		MiscObj* miscObj = actualElement->data;

		DrawAnimation(&miscObj->animationList[miscObj->type], _window);

		actualElement = actualElement->next;
	}
}

void CleanupMisc(void)
{
	Element* actualElement = misc.list->first;

	while (actualElement != NULL)
	{
		MiscObj* miscObj = actualElement->data;
		sfSprite_destroy(miscObj->sprite);
		actualElement = actualElement->next;
	}

	DeleteList(misc.list);

	sfTexture_destroy(misc.texture);
	misc = (MiscData){ 0 };

}
void DestroyMisc(List* _list, unsigned _index)
{
	MiscObj* miscObj = GetElement(_list, _index)->data;
	sfSprite_destroy(miscObj->sprite);
	RemoveElement(_list, _index);
}

void CreateMiscAnimation(MiscObj* _newMisc, MiscType _type)
{

	char miscTypeNames[][100] =
	{
	"BASIC",
	"MEDIUM",
	"ULTIMATE",
	"EXPLOSION",
	"SMOKE",
	"EXPLOSION_BOSS",
	"COLLISION_ENEMY",
	"JUMP_ENEMY",
	"JUMP"
	};

	char fileName[100];
	sprintf_s(fileName, sizeof(fileName), "Assets/Settings/MiscellenousSettings/%s_Settings.txt", miscTypeNames[_type]);

	FILE* miscellenousFile = NULL;

	if (fopen_s(&miscellenousFile, fileName, "r") == 0)
	{
		SkipLinesUntilCharacter(miscellenousFile, '=');

		MiscType typeID = ScanMiscType(miscellenousFile, "name");

		sfIntRect firstFrame = { 0 };
		int nbFrame = 0;
		int framerate = 0;
		sfBool isLooping;

		fscanf_s(miscellenousFile, "first frame rect = %d/%d/%d/%d\n",
			&firstFrame.left, &firstFrame.top,
			&firstFrame.width, &firstFrame.height);


		fscanf_s(miscellenousFile, "number of frame = %d\n", &nbFrame);
		fscanf_s(miscellenousFile, "framerate = %d\n", &framerate);

		isLooping = ScanBoolean(miscellenousFile, "looping");

		sfVector2f offset = { 0 };
		fscanf_s(miscellenousFile, "origin offset = %f/%f\n", &offset.x, &offset.y);

		_newMisc->animationList[typeID] = CreateAnimation(_newMisc->sprite, nbFrame, framerate, firstFrame, isLooping);

		LoadOrigins(_newMisc, _type, offset, firstFrame, typeID);

		fclose(miscellenousFile);
	}
	else
	{
		printf("Open Misc file error !!!\n");

	}

}
void AddMiscOrigin(MiscObj* _misc)
{
	sfVector2f recPos = sfSprite_getPosition(_misc->sprite);
	_misc->circle = CreateCircle(recPos, 5.f, sfTrue, sfBlue, 0.f, sfTransparent);

}
void UpdateMiscOrigin(MiscObj* _misc)
{
	sfCircleShape_setPosition(_misc->circle, sfSprite_getPosition(_misc->sprite));
}
void RotateMisc(sfSprite* _targetRotation)
{
	for (unsigned i = 0; i < misc.list->count; i++)
	{
		MiscObj* MiscObj = GetElement(misc.list, i)->data;

		sfSprite_setRotation(MiscObj->sprite, sfSprite_getRotation(_targetRotation));
	}
}

void FlipMisc(void)
{
	for (unsigned i = 0; i < misc.list->count; i++)
	{
		MiscObj* MiscObj = GetElement(misc.list, i)->data;

		sfSprite_setScale(MiscObj->sprite, (sfVector2f) { -SCALE, SCALE });
	}
}


void CheckMiscAnimationFinished(MiscObj* _misc, int _index)
{
	if (AnimationIsFinished(&_misc->animationList[_misc->type]))
	{
		_misc->canReplay = sfTrue;
		DestroyMisc(misc.list, _index);
	}
}

MiscType ScanMiscType(FILE* const _file, const char* const _format)
{
	char stateName[100] = { '\0' };
	char text[100] = { '\0' };
	MiscType typeID = EOF;

	strcpy_s(text, sizeof(text), _format);
	strcat_s(text, sizeof(text), " = %99s\n");
	fscanf_s(_file, text, stateName, (unsigned)sizeof(stateName));

	typeID = strcmp(stateName, "MISC_ATTACK_BASIC") == 0 ? MISC_ATTACK_BASIC : typeID;
	typeID = strcmp(stateName, "MISC_ATTACK_MEDIUM") == 0 ? MISC_ATTACK_MEDIUM : typeID;
	typeID = strcmp(stateName, "MISC_ATTACK_ULTIMATE") == 0 ? MISC_ATTACK_ULTIMATE : typeID;
	typeID = strcmp(stateName, "MISC_EXPLOSION") == 0 ? MISC_EXPLOSION : typeID;
	typeID = strcmp(stateName, "MISC_SMOKE") == 0 ? MISC_SMOKE : typeID;
	typeID = strcmp(stateName, "MISC_EXPLOSION_BOSS") == 0 ? MISC_EXPLOSION_BOSS : typeID;
	typeID = strcmp(stateName, "MISC_COLLISION_ENEMY") == 0 ? MISC_COLLISION_ENEMY : typeID;
	typeID = strcmp(stateName, "MISC_JUMP_ENEMY") == 0 ? MISC_JUMP_ENEMY : typeID;
	typeID = strcmp(stateName, "MISC_JUMP") == 0 ? MISC_JUMP : typeID;

	return typeID;
}


void DrawMiscOrigin(sfRenderWindow* _windows)
{
	Element* actualElement = misc.list->first;

	while (actualElement != NULL && misc.list->count > 0)
	{
		MiscObj* obj = actualElement->data;

		sfRenderWindow_drawCircleShape(_windows, obj->circle, NULL);

		actualElement = actualElement->next;
	}
}

void LoadOrigins(MiscObj* _misc, MiscType _type, sfVector2f _offset, sfIntRect _firstFrame, int _typeID)
{
	sfVector2f origin = { 0 };

	switch (_type)
	{
	case MISC_EXPLOSION :
		origin = (sfVector2f){ 0 + _offset.x,_firstFrame.height / 2 + _offset.y };
		break;
	case MISC_EXPLOSION_BOSS:
		origin = (sfVector2f){ _firstFrame.width / 2 + _offset.x,_firstFrame.height / 2 + _offset.y };
		break;
	default:
		origin = (sfVector2f){ _firstFrame.width / 2 + _offset.x,_firstFrame.height + _offset.y };
		break;
	}

	sfSprite_setOrigin(_misc->animationList[_typeID].sprite, origin);
}
const MiscData* GetMisc(void)
{
	return &misc;
}

MiscData* SetMisc(void)
{
	return &misc;
}

