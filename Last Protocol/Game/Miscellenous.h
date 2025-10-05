#ifndef MISCELLENOUS_H
#define MISCELLENOUS_H

#include "../Utility/Common.h"
#include "../Utility/Animation.h"
#include "../Utility/LinkedList.h"
#include "../Utility/FileReader.h"


#define ULTIMATE_OFFSET_X 100.f

typedef enum
{
	MISC_ATTACK_BASIC,
	MISC_ATTACK_MEDIUM,
	MISC_ATTACK_ULTIMATE,
	MISC_EXPLOSION,
	MISC_SMOKE,
	MISC_EXPLOSION_BOSS,
	MISC_COLLISION_ENEMY,
	MISC_JUMP_ENEMY,
	MISC_JUMP,

	NB_MISC_TYPE
}MiscType;

typedef struct
{
	sfSprite* sprite;

	AnimationData animationList[NB_MISC_TYPE];
	MiscType type;

	sfBool isCreated;
	sfBool canReplay;

	sfCircleShape* circle;
}MiscObj;

typedef struct
{
	sfTexture* texture;
	List* list;

}MiscData;

void LoadMisc(void);
void UpdateMisc(float _dt);
void DrawMisc(sfRenderWindow* _window);
void CleanupMisc(void);
void DestroyMisc(List* _list, unsigned _index);
const MiscData* GetMisc(void);
MiscData* SetMisc(void);

void AddMisc(MiscType _MiscType, sfVector2f _startPosition);
void RotateMisc(sfSprite* _targetRotation);
void FlipMisc(void);

void DrawMiscOrigin(sfRenderWindow* _windows);


#endif // !MISCELLENOUS_H