#include "Props.h"

PropsData  propsData;

void CreatePropsAnimation(PropsObj* _newProps, PropsType _type);
PropsType ScanPropsType(FILE* const _file, const char* const _format);
void LoadTexture(LevelNumber _levelNumber);
void LoadPropsSpwanMap(void);
void CreatePropsTroughEntityList(EntityID _entityID, PropsType _type);

void SetFrameAndFramerateToZero(PropsObj* _prop, PropsType _type);

void UpdatePropsBehaviour(PropsObj* _props, float _dt);
void UpdateStreetLightFlicker(PropsObj* _props, PropsType _type, float _dt);
void UpdatePropsIndividualFramerates(PropsObj* _props, PropsType _type, int _maxFramerate);
void UpdatePropsInvidualFrame(PropsObj* _props, PropsType _type, int _frame);
void UpdateHitbox(PropsObj* _props);


void AddPropsHitbox(PropsObj* _prop);
void FlipProp(PropsObj* _props, PropsType _type);

void AddProps(PropsType _propsType, sfVector2f _position)
{
	PropsObj* newProp = (PropsObj*)calloc(1, sizeof(PropsObj));
	if (newProp != NULL)
	{
		newProp->type = _propsType;
		newProp->maxTimer = 3 + rand() % (8 - 3 + 1);

		CreatePropsAnimation(newProp, _propsType);
		sfSprite_setPosition(newProp->sprite, _position);

		FlipProp(newProp, PROPS_DEAD_ROBOT);

		SetFrameAndFramerateToZero(newProp, PROPS_STREET_LIGHT);
		SetFrameAndFramerateToZero(newProp, PROPS_LIGHT_CEILING);


		AddPropsHitbox(newProp);

		Element* element = CreateElement((void*)newProp);
		InsertElement(propsData.list, element, 0);
	}
	else
	{
		printf("calloc failed in CreateMisc");
	}

}
void LoadProps(LevelNumber _levelNumber)
{
	propsData.list = CreateList();
	LoadTexture(_levelNumber);

	LoadPropsSpwanMap();

}
void LoadTexture(LevelNumber _levelNumber)
{
	switch (_levelNumber)
	{
	case LEVEL_1:
		propsData.texture = sfTexture_createFromFile("Assets/Textures/Props/SheetPropsLevel1.png", NULL);
		break;
	case LEVEL_2:
		propsData.texture = sfTexture_createFromFile("Assets/Textures/Props/SheetPropsLevel2.png", NULL);
		break;
	}
}

void UpdateProps(float _dt)
{
	Element* actualElement = propsData.list->last;
	int propsID = propsData.list->count - 1;

	while (actualElement != NULL && propsData.list->count > 0)
	{
		PropsObj* props = actualElement->data;

		UpdateAnimation(&props->animationList[props->type], _dt);

		UpdateHitbox(props);
		UpdatePropsBehaviour(props, _dt);

		actualElement = actualElement->prev;
		propsID--;
	}
}

void DrawProps(sfRenderWindow* _window)
{
	Element* actualElement = propsData.list->first;

	while (actualElement != NULL && propsData.list->count > 0)
	{
		PropsObj* props = actualElement->data;


		DrawAnimation(&props->animationList[props->type], _window);

		actualElement = actualElement->next;
	}
}

void CleanupProps(void)
{
	Element* actualElement = propsData.list->first;

	while (actualElement != NULL)
	{
		PropsObj* props = actualElement->data;
		sfSprite_destroy(props->sprite);
		props->sprite = NULL;
		actualElement = actualElement->next;
	}

	DeleteList(propsData.list);

	sfTexture_destroy(propsData.texture);
	propsData = (PropsData){ 0 };
}

void DestroyProps(List* _list, unsigned _index)
{
	PropsObj* props = GetElement(_list, _index)->data;
	sfSprite_destroy(props->sprite);
	RemoveElement(_list, _index);
}
void CreatePropsAnimation(PropsObj* _newProps, PropsType _type)
{

	char propTypeNames[][100] =
	{
	"GRASS_1",
	"GRASS_2",
	"LIGHT_CEILING",
	"SLIDE_SIGN",
	"JUMP_SIGN",
	"RUN_LEFT_SIGN",
	"RUN_RIGHT_SIGN",
	"UP_SIGN",
	"RIGHT_SIGN",
	"DOWN_SIGN",
	"LEFT_SIGN",
	"DEAD_ROBOT",
	"STREET_LIGHT",
	"VINE_1",
	"VINE_2",
	"VINE_3",
	};

	char fileName[200];
	sprintf_s(fileName, sizeof(fileName), "Assets/Settings/PropsSettings/%s_Settings.txt", propTypeNames[_type]);

	FILE* propsFile = NULL;

	if (fopen_s(&propsFile, fileName, "r") == 0)
	{
		SkipLinesUntilCharacter(propsFile, '=');

		PropsType typeID = ScanPropsType(propsFile, "name");

		sfIntRect firstFrame = { 0 };
		int nbFrame = 0;
		int framerate = 0;
		sfBool isLooping;

		fscanf_s(propsFile, "first frame rect = %d/%d/%d/%d\n",
			&firstFrame.left, &firstFrame.top,
			&firstFrame.width, &firstFrame.height);

		_newProps->sprite = CreateSpriteFromSpriteSheet(propsData.texture, firstFrame, sfFalse, SCALE);


		fscanf_s(propsFile, "number of frame = %d\n", &nbFrame);
		fscanf_s(propsFile, "framerate = %d\n", &framerate);

		isLooping = ScanBoolean(propsFile, "looping");

		sfVector2f offset = { 0 };
		fscanf_s(propsFile, "origin offset = %f/%f\n", &offset.x, &offset.y);



		_newProps->animationList[typeID] = CreateAnimation(_newProps->sprite, nbFrame, framerate, firstFrame, isLooping);

		sfSprite_setOrigin(_newProps->animationList[typeID].sprite, (sfVector2f)
		{
			firstFrame.width / 2 + offset.x, firstFrame.height + offset.y
		});

		_newProps->isInteractible = ScanBoolean(propsFile, "isAnimInteractible");
		_newProps->isUpFront = ScanBoolean(propsFile, "isUpFront");

		fclose(propsFile);
	}
	else
	{
		printf("Open Props file error !!!\n");

	}

}
PropsType ScanPropsType(FILE* const _file, const char* const _format)
{
	char stateName[50] = { '\0' };
	char text[50] = { '\0' };
	PropsType typeID = EOF;

	strcpy_s(text, sizeof(text), _format);
	strcat_s(text, sizeof(text), " = %49s\n");
	fscanf_s(_file, text, stateName, (unsigned)sizeof(stateName));

	typeID = strcmp(stateName, "GRASS_1") == 0 ? PROPS_GRASS_1 : typeID;
	typeID = strcmp(stateName, "GRASS_2") == 0 ? PROPS_GRASS_2 : typeID;
	typeID = strcmp(stateName, "LIGHT_CEILING") == 0 ? PROPS_LIGHT_CEILING : typeID;
	typeID = strcmp(stateName, "SLIDE_SIGN") == 0 ? PROPS_SLIDE_SIGN : typeID;
	typeID = strcmp(stateName, "JUMP_SIGN") == 0 ? PROPS_JUMP_SIGN : typeID;
	typeID = strcmp(stateName, "RUN_LEFT_SIGN") == 0 ? PROPS_RUN_LEFT_SIGN : typeID;
	typeID = strcmp(stateName, "RUN_RIGHT_SIGN") == 0 ? PROPS_RUN_RIGHT_SIGN : typeID;
	typeID = strcmp(stateName, "UP_SIGN") == 0 ? PROPS_UP_SIGN : typeID;
	typeID = strcmp(stateName, "RIGHT_SIGN") == 0 ? PROPS_RIGHT_SIGN : typeID;
	typeID = strcmp(stateName, "DOWN_SIGN") == 0 ? PROPS_DOWN_SIGN : typeID;
	typeID = strcmp(stateName, "LEFT_SIGN") == 0 ? PROPS_LEFT_SIGN : typeID;
	typeID = strcmp(stateName, "DEAD_ROBOT") == 0 ? PROPS_DEAD_ROBOT : typeID;
	typeID = strcmp(stateName, "STREET_LIGHT") == 0 ? PROPS_STREET_LIGHT : typeID;
	typeID = strcmp(stateName, "VINE_1") == 0 ? PROPS_VINE_1 : typeID;
	typeID = strcmp(stateName, "VINE_2") == 0 ? PROPS_VINE_2 : typeID;
	typeID = strcmp(stateName, "VINE_3") == 0 ? PROPS_VINE_3 : typeID;

	return typeID;
}

void LoadPropsSpwanMap(void)
{

	////Light ceiling
	CreatePropsTroughEntityList(LIGHT_CEILING, PROPS_LIGHT_CEILING);

	//////Grass
	CreatePropsTroughEntityList(GRASS_1, PROPS_GRASS_1);
	CreatePropsTroughEntityList(GRASS_2, PROPS_GRASS_2);

	//////Small Signs
	CreatePropsTroughEntityList(SIGN_UP, PROPS_UP_SIGN);
	CreatePropsTroughEntityList(SIGN_RIGHT, PROPS_RIGHT_SIGN);
	CreatePropsTroughEntityList(SIGN_DOWN, PROPS_DOWN_SIGN);
	CreatePropsTroughEntityList(SIGN_LEFT, PROPS_LEFT_SIGN);

	//////Big Signs
	CreatePropsTroughEntityList(BIG_SIGN_UP, PROPS_JUMP_SIGN);
	CreatePropsTroughEntityList(BIG_SIGN_RIGHT, PROPS_RUN_RIGHT_SIGN);
	CreatePropsTroughEntityList(BIG_SIGN_DOWN, PROPS_SLIDE_SIGN);
	CreatePropsTroughEntityList(BIG_SIGN_LEFT, PROPS_RUN_LEFT_SIGN);

	////Dead robot head
	CreatePropsTroughEntityList(DEAD_ROBOT, PROPS_DEAD_ROBOT);

	////Stree lights
	CreatePropsTroughEntityList(STREET_LIGHT, PROPS_STREET_LIGHT);

	CreatePropsTroughEntityList(LIANA_1, PROPS_VINE_1);
	CreatePropsTroughEntityList(LIANA_2, PROPS_VINE_2);
	CreatePropsTroughEntityList(LIANA_3, PROPS_VINE_3);


}
void SetFrameAndFramerateToZero(PropsObj* _prop, PropsType _type)
{
	SetAnimationFrame(&_prop->animationList[_type], 1);
	SetAnimationFramerate(&_prop->animationList[_type], 0);
}

void AddPropsHitbox(PropsObj* _prop)
{
	if (_prop->isInteractible)
	{
		sfIntRect firstFrame = _prop->animationList[_prop->type].firstFrame;
		_prop->hitboxSize.x = firstFrame.width;
		_prop->hitboxSize.y = firstFrame.height;

		sfFloatRect rect = { 0, 0, _prop->hitboxSize.x, _prop->hitboxSize.y };
		_prop->hitbox = CreateHitbox(rect, sfFalse, sfRed, SCALE);
		sfRectangleShape_setOrigin(_prop->hitbox, (sfVector2f) { _prop->hitboxSize.x / 2, _prop->hitboxSize.y });
	}
}

void CreatePropsTroughEntityList(EntityID _entityID, PropsType _type)
{
	EntityData entity = GetLevel()->entityList[_entityID];
	for (unsigned i = 0; i < entity.count; i++)
	{
		sfVector2f position = { entity.positionList[i].x + CELL_WIDTH / 2, entity.positionList[i].y + CELL_HEIGHT - 3 };
		AddProps(_type, position);
	}

}

void UpdatePropsBehaviour(PropsObj* _props, float _dt)
{
	switch (_props->type)
	{
	case PROPS_STREET_LIGHT:
		UpdateStreetLightFlicker(_props, PROPS_STREET_LIGHT, _dt);
		break;
	case PROPS_LIGHT_CEILING:
		UpdateStreetLightFlicker(_props, PROPS_LIGHT_CEILING, _dt);
		break;
	}
}
void UpdateStreetLightFlicker(PropsObj* _props, PropsType _type, float _dt)
{

	_props->flickerTimer += _dt;

	if (_props->flickerTimer > _props->maxTimer)
	{
		SetAnimationFramerate(&_props->animationList[_type], 1 + rand() % STREET_LIGHT_FLICKER_MAX_FRAMERATE);

		if (_props->flickerTimer > _props->maxTimer + 0.5f)
		{
			_props->flickerTimer = 0.f;
		}
	}
	else
	{
		UpdatePropsInvidualFrame(_props, _type, 1);
		SetAnimationFramerate(&_props->animationList[_type], 0);
	}
}

void UpdatePropsIndividualFramerates(PropsObj* _props, PropsType _type, int _maxFramerate)
{
	if (_props->type == _type)
	{
		SetAnimationFramerate(&_props->animationList[_type], 1 + rand() % _maxFramerate);
	}
}

void UpdatePropsInvidualFrame(PropsObj* _props, PropsType _type, int _frame)
{
	if (_props->type == _type)
	{
		SetAnimationFrame(&_props->animationList[_type], _frame);
	}
}

void UpdateHitbox(PropsObj* _props)
{
	if (_props->isInteractible)
	{
		sfRectangleShape_setPosition(_props->hitbox, sfSprite_getPosition(_props->sprite));

	}
}

void FlipProp(PropsObj* _props, PropsType _type)
{
	int random = rand() % MAX_RANDOM;

	if (_props->type == _type)
	{
		if (random == 1)
		{
			sfSprite_setScale(_props->sprite, (sfVector2f) { -SCALE, SCALE });
		}
		else
		{
			sfSprite_setScale(_props->sprite, (sfVector2f) { SCALE, SCALE });
		}

	}
}
void DrawPropsHitbox(sfRenderWindow* _windows)
{
	Element* actualElement = propsData.list->first;

	while (actualElement != NULL && propsData.list->count > 0)
	{
		PropsObj* obj = actualElement->data;

		if (obj->isInteractible)
		{
			sfRenderWindow_drawRectangleShape(_windows, obj->hitbox, NULL);

		}
		actualElement = actualElement->next;
	}
}

const PropsData* GetProps(void)
{
	return &propsData;
}

PropsData* SetProps(void)
{
	return &propsData;
}
