#ifndef MAPPING_H
#define MAPPING_H

#include "Common.h"

#define MIN_DETECTION_CONTROLLER 25



typedef enum
{
	NEGATIVE,
	POSITIVE,

}AxisDirection;

typedef struct
{
	char axisID;
	AxisDirection direction;
	float value;

}AxisButton;

typedef struct
{
	char controller;
	char keyboard;
	AxisButton axis;

}MappingButton;




typedef struct
{

	MappingButton down;
	MappingButton left;
	MappingButton right;
	MappingButton shoot;
	MappingButton jump;
	MappingButton attack;
	MappingButton shield;
	MappingButton dashing;
	MappingButton resize;
	MappingButton pause;

	MappingButton bulletTime;

}MappingList;


typedef struct
{

	MappingButton up;
    MappingButton down;
	MappingButton left;
	MappingButton right;
	MappingButton click;
}MappingMenuList;

typedef struct
{
	sfBool hasAController;
	int controllerId;
	MappingList key;
	MappingMenuList keyMenu;

} MappingData;

void LoadControl();
void UpdateControl();
sfBool CheckMapping(MappingButton _button);
MappingList* GetMapping();
MappingMenuList* GetMappingMenu();
float GetAxisValue(MappingButton _button);

MappingData mapping;


#endif // !MAPPING_H

