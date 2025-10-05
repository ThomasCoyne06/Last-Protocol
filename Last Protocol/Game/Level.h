#ifndef LEVEL_H
#define LEVEL_H

#include "../Utility/Common.h"

#define TILE_WIDTH 20
#define TILE_HEIGHT 20

#define CELL_WIDTH 60
#define CELL_HEIGHT 60

#define VANISH_SPEED 1000

typedef enum
{
	LEVEL_1 = 1,
	LEVEL_2,
	LEVEL_3,

	NB_LEVEL_PLUS_ONE
}LevelNumber;

typedef enum
{
	BOTTOM,
	COLLID,
	SHADOW,
	SECRET,
	ENTITY,

	NB_LAYER
}LayerType;

typedef enum
{
	PLAYER_SPAWN_ID = 242,
	FILE_ID = 224,
	LIFE_ID = 291,
	LASER_HORIZONTAL_ID = 210,
	LASER_VERTICAL_ID = 229,
	BUMPER_ID = 273,
	SIGN_ID = 274,
	ENEMY_BASIC_SPAWN_ID = 288,
	ENEMY_TV_SPAWN_ID = 290,
	ENEMY_EYE_SPAWN_ID = 289,
	ENEMY_JUMPER_SPAWN_ID = 304,
	ENEMY_MINI_SPAWN_ID = 305,
	WAVE_ID = 244,
	DOOR_ID = 260,

	LIGHT_CEILING_LEVEL_1_ID = 187,
	GRASS_1_LEVEL_1_ID = 176,
	GRASS_2_LEVEL_1_ID = 177,
	SIGN_RIGHT_LEVEL_1_ID = 199,
	SIGN_LEFT_LEVEL_1_ID = 200,
	SIGN_UP_LEVEL_1_ID = 201,
	SIGN_DOWN_LEVEL_1_ID = 202,
	BIG_SIGN_RIGHT_LEVEL_1_ID = 183,
	BIG_SIGN_LEFT_LEVEL_1_ID = 184,
	BIG_SIGN_UP_LEVEL_1_ID = 185,
	BIG_SIGN_DOWN_LEVEL_1_ID = 186,
	DEAD_ROBOT_LEVEL_1_ID = 189,
	STREET_LIGHT_LEVEL_1_ID = 188,
	LIANA_1_LEVEL_1_ID = 190,
	LIANA_2_LEVEL_1_ID = 206,
	LIANA_3_LEVEL_1_ID = 222,


	LIGHT_CEILING_LEVEL2_ID = 411,
	GRASS_1_LEVEL2_ID = 310,
	GRASS_2_LEVEL2_ID = 311,
	SIGN_RIGHT_LEVEL2_ID = 424,
	SIGN_LEFT_LEVEL2_ID = 423,
	SIGN_UP_LEVEL2_ID = 425,
	SIGN_DOWN_LEVEL2_ID = 426,
	BIG_SIGN_RIGHT_LEVEL2_ID = 407,
	BIG_SIGN_LEFT_LEVEL2_ID = 408,
	BIG_SIGN_UP_LEVEL2_ID = 409,
	BIG_SIGN_DOWN_LEVEL2_ID = 410,
	DEAD_ROBOT_LEVEL2_ID = 413,
	STREET_LIGHT_LEVEL2_ID = 412,
	LIANA_1_LEVEL2_ID = 398,
	LIANA_2_LEVEL2_ID = 414,
	LIANA_3_LEVEL2_ID = 430

}EntityID;

typedef enum
{
	PLAYER_SPAWN,
	PICKUP_FILE,
	PICKUP_LIFE,
	LASER_HORIZONTAL,
	LASER_VERTICAL,
	BUMPER,
	SIGN,
	ENEMY_BASIC_SPAWN,
	ENEMY_TV_SPAWN,
	ENEMY_EYE_SPAWN,
	ENEMY_JUMPER_SPAWN,
	ENEMY_MINI_SPAWN,
	WAVE,
	DOOR,

	LIGHT_CEILING,
	GRASS_1,
	GRASS_2,
	SIGN_RIGHT,
	SIGN_LEFT,
	SIGN_UP,
	SIGN_DOWN,
	BIG_SIGN_RIGHT,
	BIG_SIGN_LEFT,
	BIG_SIGN_UP,
	BIG_SIGN_DOWN,
	DEAD_ROBOT,
	STREET_LIGHT,
	LIANA_1,
	LIANA_2,
	LIANA_3,

	NB_ENTITY
}EntityType;

typedef struct
{
	//EntityID id;
	sfVector2f* positionList;
	unsigned count;
}EntityData;

typedef struct
{
	sfSprite* sprite;
	sfFloatRect hitbox;
	int id;
}Cell;

typedef struct
{
	LayerType type;
	char* name;
	sfBool invisible;
	sfBool vanish;
	Cell** grid;
	float alpha;
}LayerData;

typedef struct
{
	sfTexture* tiledTexture;
	unsigned number;
	LayerData layer[NB_LAYER];
	EntityData entityList[NB_ENTITY];
	sfFloatRect firstRect;

	CellCoordinates levelSize;
	int nbRow;
	int nbColumn;
}LevelData;

typedef struct
{
	CollisionDirection direction;
	float offset;
	sfBool isColliding;
	CellCoordinates coordinates;
}LevelCollid;

void LoadLevel(LevelNumber _levelNumber);
void UpdateLevel(float _dt);
void DrawLevelBehind(sfRenderWindow* _window);
void DrawLevelBefore(sfRenderWindow* _window);
void CleanupLevel(void);
void KeypressedLevel(sfKeyEvent _key);

const LevelData* GetLevel(void);
LevelData* SetLevel(void);

#pragma region LevelGetParameter
sfFloatRect GetLevelFirstRect(void);
CellCoordinates GetLevelColumnLineWidthIndex(int _id);
int GetLevelIndexWidthColumnLine(CellCoordinates _columnLine);
CellCoordinates GetLevelColumnLineWithPosition(sfVector2f _position);
sfVector2f GetLevelPositionWithColumnLine(CellCoordinates _columnLine, sfBool _centerOrigin);
sfFloatRect GetLevelHitbox(void);
sfBool GetLevelCollide(CellCoordinates _coordinates);
sfBool GetLevelCollideSecret(CellCoordinates _coordinates);
sfFloatRect GetCellHitbox(CellCoordinates coordinates);
LevelCollid CollisionTilesHoriz(sfFloatRect _hitbox);
LevelCollid CollisionTilesVerti(sfFloatRect _hitbox);

#pragma endregion

#endif