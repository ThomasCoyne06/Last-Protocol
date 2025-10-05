#include "Level.h"
#include "../Resources/Sounds.h"
#include "../Resources/Music.h"
#include "../Stats.h"

void UpdateVanishSecretZone(float _dt);

LevelData level;
int actualMusicTutorial = 0;

//void LoadLevelCollid(void);
//void LoadLevelEntity(void);

void LoadLevel(LevelNumber _levelNumber)
{
	char sizeLevel[70];
	//sprintf_s(sizeLevel, sizeof(sizeLevel), "Assets/Textures/Game/Level/Sprite_Sheet Tile %d.png", _levelNumber);
	sprintf_s(sizeLevel, sizeof(sizeLevel), "Assets/Textures/Game/Level/Sprite_Sheet Tile.png");

	level.tiledTexture = sfTexture_createFromFile(sizeLevel, NULL);
	level.firstRect = (sfFloatRect){ 0,0,CELL_WIDTH,CELL_HEIGHT };
	level.number = _levelNumber;

	level.layer[BOTTOM].name = "Bottom";
	level.layer[COLLID].name = "Collid";
	level.layer[SHADOW].name = "Shadow";
	level.layer[SECRET].name = "Secret";
	level.layer[ENTITY].name = "Entity";

	level.layer[ENTITY].invisible = sfTrue;
	// Allister
	sprintf_s(sizeLevel, sizeof(sizeLevel), "Assets/Levels/Level %d/Level_%d_Size.txt", level.number, level.number);

	FILE* levelFileText;
	if (fopen_s(&levelFileText, sizeLevel, "r") != 0)
	{
		printf("Open file error !!!\n");
	}
	else
	{
		fscanf_s(levelFileText, "SIZE %d/%d\n", &level.levelSize.line, &level.levelSize.column);
	}

	for (unsigned layerID = 0; layerID < NB_LAYER; layerID++)
	{
		level.layer[layerID].alpha = 255;
		// initialise line number
		level.layer[layerID].grid = malloc(level.levelSize.line * sizeof(Cell));

		// Initialise column number
		for (int i = 0; i < level.levelSize.line; i++)
		{
			level.layer[layerID].grid[i] = malloc(level.levelSize.column * sizeof(Cell));
		}
		// Allister
		level.layer[layerID].type = layerID;

		char layerName[50];
		sprintf_s(layerName, sizeof(layerName), "Assets/Levels/Level %d/Level_%d_%s.csv", level.number, level.number, level.layer[layerID].name);

		FILE* levelFile;
		if (fopen_s(&levelFile, layerName, "r") != 0)
		{
			printf("Open file error !!!\n");
		}
		else
		{
			for (unsigned i = 0; i < NB_ENTITY; i++)
			{
				level.entityList[i].positionList = calloc(0, sizeof(sfVector2f));
			}

			for (unsigned line = 0; line < level.levelSize.line; line++)
			{
				for (unsigned column = 0; column < level.levelSize.column; column++)
				{
					Cell* newCell = &level.layer[layerID].grid[line][column];

					fscanf_s(levelFile, "%d,", &newCell->id);

					sfVector2f position = {
						level.firstRect.left + column * CELL_WIDTH ,
						level.firstRect.top + line * CELL_HEIGHT };

					newCell->hitbox = (sfFloatRect){ position.x, position.y, CELL_WIDTH, CELL_HEIGHT };

					if (newCell->id >= 0)
					{
						sfVector2u textureSize = sfTexture_getSize(level.tiledTexture);
						CellCoordinates coordinate = { newCell->id % (textureSize.x / TILE_WIDTH), newCell->id / (textureSize.x / TILE_WIDTH) };
						sfVector2f position2 = GetPositionWithColumnLine(coordinate, sfFalse, (sfFloatRect) { 0, 0, TILE_WIDTH, TILE_HEIGHT });
						sfIntRect rect = { (int)position2.x, (int)position2.y, TILE_WIDTH, TILE_HEIGHT };

						newCell->sprite = CreateSpriteFromSpriteSheet(level.tiledTexture, rect, sfFalse, SCALE);
						sfSprite_setPosition(newCell->sprite, (sfVector2f) { newCell->hitbox.left, newCell->hitbox.top });

						if (level.layer[layerID].type == ENTITY)
						{
							EntityData* entity = NULL;

							switch (newCell->id)
							{
							case PLAYER_SPAWN_ID:
								entity = &level.entityList[PLAYER_SPAWN];
								break;
							case LIFE_ID:
								entity = &level.entityList[PICKUP_LIFE];
								break;
							case FILE_ID:
								entity = &level.entityList[PICKUP_FILE];
								break;
							case LASER_HORIZONTAL_ID:
								entity = &level.entityList[LASER_HORIZONTAL];
								break;
							case LASER_VERTICAL_ID:
								entity = &level.entityList[LASER_VERTICAL];
								break;
							case BUMPER_ID:
								entity = &level.entityList[BUMPER];
								break;
							case SIGN_ID:
								entity = &level.entityList[SIGN];
								break;
							case ENEMY_BASIC_SPAWN_ID:
								entity = &level.entityList[ENEMY_BASIC_SPAWN];
								break;
							case ENEMY_TV_SPAWN_ID:
								entity = &level.entityList[ENEMY_TV_SPAWN];
								break;
							case ENEMY_EYE_SPAWN_ID:
								entity = &level.entityList[ENEMY_EYE_SPAWN];
								break;
							case ENEMY_JUMPER_SPAWN_ID:
								entity = &level.entityList[ENEMY_JUMPER_SPAWN];
								break;
							case ENEMY_MINI_SPAWN_ID:
								entity = &level.entityList[ENEMY_MINI_SPAWN];
								break;
							case WAVE_ID:
								entity = &level.entityList[WAVE];
								break;
							case DOOR_ID:
								entity = &level.entityList[DOOR];
								break;

							case LIGHT_CEILING_LEVEL_1_ID:
								entity = &level.entityList[LIGHT_CEILING];
								break;
							case GRASS_1_LEVEL_1_ID:
								entity = &level.entityList[GRASS_1];
								break;
							case GRASS_2_LEVEL_1_ID:
								entity = &level.entityList[GRASS_2];
								break;
							case SIGN_RIGHT_LEVEL_1_ID:
								entity = &level.entityList[SIGN_RIGHT];
								break;
							case SIGN_UP_LEVEL_1_ID:
								entity = &level.entityList[SIGN_UP];
								break;
							case SIGN_DOWN_LEVEL_1_ID:
								entity = &level.entityList[SIGN_DOWN];
								break;
							case SIGN_LEFT_LEVEL_1_ID:
								entity = &level.entityList[SIGN_LEFT];
								break;
							case BIG_SIGN_RIGHT_LEVEL_1_ID:
								entity = &level.entityList[BIG_SIGN_RIGHT];
								break;
							case BIG_SIGN_LEFT_LEVEL_1_ID:
								entity = &level.entityList[BIG_SIGN_LEFT];
								break;
							case BIG_SIGN_UP_LEVEL_1_ID:
								entity = &level.entityList[BIG_SIGN_UP];
								break;
							case BIG_SIGN_DOWN_LEVEL_1_ID:
								entity = &level.entityList[BIG_SIGN_DOWN];
								break;
							case DEAD_ROBOT_LEVEL_1_ID:
								entity = &level.entityList[DEAD_ROBOT];
								break;
							case STREET_LIGHT_LEVEL_1_ID:
								entity = &level.entityList[STREET_LIGHT];
								break;
							case LIANA_1_LEVEL_1_ID:
								entity = &level.entityList[LIANA_1];
								break;
							case LIANA_2_LEVEL_1_ID:
								entity = &level.entityList[LIANA_2];
								break;
							case LIANA_3_LEVEL_1_ID:
								entity = &level.entityList[LIANA_3];
								break;


							case LIGHT_CEILING_LEVEL2_ID:
								entity = &level.entityList[LIGHT_CEILING];
								break;
							case GRASS_1_LEVEL2_ID:
								entity = &level.entityList[GRASS_1];
								break;
							case GRASS_2_LEVEL2_ID:
								entity = &level.entityList[GRASS_2];
								break;
							case SIGN_RIGHT_LEVEL2_ID:
								entity = &level.entityList[SIGN_RIGHT];
								break;
							case SIGN_DOWN_LEVEL2_ID:
								entity = &level.entityList[SIGN_DOWN];
								break;
							case SIGN_LEFT_LEVEL2_ID:
								entity = &level.entityList[SIGN_LEFT];
								break;
							case SIGN_UP_LEVEL2_ID:
								entity = &level.entityList[SIGN_UP];
								break;
							case BIG_SIGN_RIGHT_LEVEL2_ID:
								entity = &level.entityList[BIG_SIGN_RIGHT];
								break;
							case BIG_SIGN_LEFT_LEVEL2_ID:
								entity = &level.entityList[BIG_SIGN_LEFT];
								break;
							case BIG_SIGN_UP_LEVEL2_ID:
								entity = &level.entityList[BIG_SIGN_UP];
								break;
							case BIG_SIGN_DOWN_LEVEL2_ID:
								entity = &level.entityList[BIG_SIGN_DOWN];
								break;
							case DEAD_ROBOT_LEVEL2_ID:
								entity = &level.entityList[DEAD_ROBOT];
								break;
							case STREET_LIGHT_LEVEL2_ID:
								entity = &level.entityList[STREET_LIGHT];
								break;
							case LIANA_1_LEVEL2_ID:
								entity = &level.entityList[LIANA_1];
								break;
							case LIANA_2_LEVEL2_ID:
								entity = &level.entityList[LIANA_2];
								break;
							case LIANA_3_LEVEL2_ID:
								entity = &level.entityList[LIANA_3];
								break;

							default:
								break;
							}

							if (entity != NULL)
							{
								//entity->id = newCell->id;

								if (ResizeArray(&entity->positionList, entity->count + 1, sizeof(sfVector2f)))
								{
									entity->positionList[entity->count] = position;
								}
								entity->count++;
							}
						}
					}
				}
			}
			fclose(levelFile);
		}
	}

}


void UpdateLevel(float _dt)
{
	UpdateVanishSecretZone(_dt);
}

void UpdateVanishSecretZone(float _dt)
{
	for (unsigned layerID = 0; layerID < NB_LAYER; layerID++)
	{
		if (level.layer[layerID].vanish)
		{
			level.layer[layerID].alpha -= VANISH_SPEED * _dt;
			if (level.layer[layerID].alpha < 0)
			{
				level.layer[layerID].alpha = 0;
			}
		}
		else if (level.layer[layerID].alpha < 255)
		{
			level.layer[layerID].alpha += VANISH_SPEED * _dt;
			if (level.layer[layerID].alpha > 255)
			{
				level.layer[layerID].alpha = 255;
			}
		}
	}
}

void DrawLevelBehind(sfRenderWindow* _window)
{
	for (unsigned layerID = 0; layerID <= SHADOW; layerID++)
	{
		if (!level.layer[layerID].invisible)
		{
			for (unsigned line = 0; line < level.levelSize.line; line++)
			{
				for (unsigned column = 0; column < level.levelSize.column; column++)
				{
					Cell* cell = &level.layer[layerID].grid[line][column];
					if (cell->id >= 0)
					{
						sfSprite_setColor(cell->sprite, sfColor_fromRGBA(255, 255, 255, level.layer[layerID].alpha));
						sfRenderWindow_drawSprite(_window, cell->sprite, NULL);
					}
				}
			}
		}
	}
}

void DrawLevelBefore(sfRenderWindow* _window)
{
	for (unsigned layerID = SECRET; layerID < NB_LAYER; layerID++)
	{
		if (!level.layer[layerID].invisible)
		{
			for (unsigned line = 0; line < level.levelSize.line; line++)
			{
				for (unsigned column = 0; column < level.levelSize.column; column++)
				{
					Cell* cell = &level.layer[layerID].grid[line][column];
					if (cell->id >= 0)
					{
						sfSprite_setColor(cell->sprite, sfColor_fromRGBA(255, 255, 255, level.layer[layerID].alpha));
						sfRenderWindow_drawSprite(_window, cell->sprite, NULL);
					}
				}
			}
		}
	}
}

void CleanupLevel(void)
{
	for (unsigned layerID = 0; layerID < NB_LAYER; layerID++)
	{
		for (unsigned line = 0; line < level.levelSize.line; line++)
		{
			for (unsigned column = 0; column < level.levelSize.column; column++)
			{
				Cell* cell = &level.layer[layerID].grid[line][column];

				if (cell->id >= 0)
				{
					sfSprite_destroy(cell->sprite);
					cell->sprite = NULL;
				}
			}
		}
	}

	for (unsigned i = 0; i < NB_ENTITY; i++)
	{
		sfVector2f* positionList = level.entityList[i].positionList;
		free(positionList);
		level.entityList[i].count = 0;
	}

	sfTexture_destroy(level.tiledTexture);
	level.tiledTexture = NULL;

}

void KeypressedLevel(sfKeyEvent _key)
{

}
const LevelData* GetLevel(void)
{
	return &level;
}

LevelData* SetLevel(void)
{
	return &level;
}

#pragma region LevelGetParameter
sfBool GetLevelCollide(CellCoordinates _coordinates)
{
	if (_coordinates.column >= 0 && _coordinates.column < level.levelSize.column)
	{
		if (_coordinates.line >= 0 && _coordinates.line < level.levelSize.line)
		{
			return level.layer[COLLID].grid[_coordinates.line][_coordinates.column].id >= 0;
		}
	}
	return sfFalse;
}

sfBool GetLevelCollideSecret(CellCoordinates _coordinates)
{
	if (_coordinates.column >= 0 && _coordinates.column < level.levelSize.column)
	{
		if (_coordinates.line >= 0 && _coordinates.line < level.levelSize.line)
		{
			return level.layer[SECRET].grid[_coordinates.line][_coordinates.column].id >= 0;
		}
	}
	return sfFalse;
}

sfFloatRect GetLevelFirstRect(void)
{
	return level.firstRect;
}

sfFloatRect GetCellHitbox(CellCoordinates coordinates)
{
	return level.layer[COLLID].grid[coordinates.line][coordinates.column].hitbox;
}

CellCoordinates GetLevelColumnLineWidthIndex(int _id)
{
	int column = _id % level.levelSize.column;
	int line = _id / level.levelSize.column;

	return (CellCoordinates) { column, line };
}

int GetLevelIndexWidthColumnLine(CellCoordinates _columnLine)
{
	return _columnLine.line * level.levelSize.column + _columnLine.column;
}

CellCoordinates GetLevelColumnLineWithPosition(sfVector2f _position)
{
	int column = (int)((_position.x - level.firstRect.left) / level.firstRect.width);
	int line = (int)((_position.y - level.firstRect.top) / level.firstRect.height);

	return (CellCoordinates) { column, line };
}

sfVector2f GetLevelPositionWithColumnLine(CellCoordinates _columnLine, sfBool _centerOrigin)
{
	float x = level.firstRect.left + _columnLine.column * level.firstRect.width;
	float y = level.firstRect.top + _columnLine.line * level.firstRect.height;

	if (_centerOrigin)
	{
		x += level.firstRect.width / 2;
		y += level.firstRect.height / 2;
	}

	return (sfVector2f) { x, y };
}

sfFloatRect GetLevelHitbox(void)
{
	return (sfFloatRect)
	{
		level.firstRect.left + TILE_WIDTH,
			level.firstRect.top + TILE_HEIGHT,
			level.firstRect.width* level.levelSize.column - (TILE_WIDTH * 2),
			level.firstRect.height* level.levelSize.line - (TILE_HEIGHT * 2)
	};
}

LevelCollid CollisionTilesHoriz(sfFloatRect _hitbox)
{
	LevelCollid col = { 0 };

	sfVector2f northWest = { _hitbox.left + 1, _hitbox.top + 1 };
	CellCoordinates cellNorthWest = GetLevelColumnLineWithPosition(northWest);

	sfVector2f southEast = { _hitbox.left + _hitbox.width - 1, _hitbox.top + _hitbox.height - 1 };
	CellCoordinates cellSouthEast = GetLevelColumnLineWithPosition(southEast);

	for (int column = cellNorthWest.column; column <= cellSouthEast.column; column++)
	{
		for (int line = cellNorthWest.line; line <= cellSouthEast.line; line++)
		{
			CellCoordinates cell = { column,line };
			if (GetLevelCollide(cell))
			{
				sfFloatRect cellHitbox = GetCellHitbox(cell);
				float colLeft = cellHitbox.left + cellHitbox.width - _hitbox.left;
				float colRight = _hitbox.left + _hitbox.width - cellHitbox.left;

				col.offset = colLeft < colRight ? colLeft : -colRight;
				col.direction = colLeft < colRight ? COLLISION_LEFT : COLLISION_RIGHT;
				col.isColliding = sfTrue;
				col.coordinates = cell;

				return col;
			}
		}
	}
	return col;
}

LevelCollid CollisionTilesVerti(sfFloatRect _hitbox)
{
	LevelCollid col = { 0 };

	sfVector2f northWest = { _hitbox.left + 1, _hitbox.top + 1 };
	CellCoordinates cellNorthWest = GetLevelColumnLineWithPosition(northWest);

	sfVector2f southEast = { _hitbox.left + _hitbox.width - 1, _hitbox.top + _hitbox.height - 1 };
	CellCoordinates cellSouthEast = GetLevelColumnLineWithPosition(southEast);

	for (int column = cellNorthWest.column; column <= cellSouthEast.column; column++)
	{
		for (int line = cellNorthWest.line; line <= cellSouthEast.line; line++)
		{
			CellCoordinates cell = { column,line };
			if (GetLevelCollide(cell))
			{
				sfFloatRect cellHitbox = GetCellHitbox(cell);
				float colTop = cellHitbox.top + cellHitbox.height - _hitbox.top;
				float colDown = _hitbox.top + _hitbox.height - cellHitbox.top;

				col.offset = colTop < colDown ? colTop : -colDown;
				col.direction = colTop < colDown ? COLLISION_UP : COLLISION_DOWN;
				col.isColliding = sfTrue;
				col.coordinates = cell;

				return col;
			}
		}
	}
	return col;
}