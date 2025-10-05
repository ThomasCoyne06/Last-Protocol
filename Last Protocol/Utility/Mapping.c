#include "Mapping.h"

MappingData mapping;

void LoadControllJoystick();
void LoadControllKeyboard();

void LoadControl()
{
	LoadControllJoystick();
	LoadControllKeyboard();

}

void UpdateControl()
{

	LoadControllJoystick();
}

void LoadControllJoystick()
{

	sfJoystick_update();

	for (int i = 0; i < sfJoystickCount; i++)
	{
		if (sfJoystick_isConnected(i) == sfTrue)
		{
			mapping.hasAController = sfTrue;
			mapping.controllerId = i;

		}
	}


	// unhandled are set to 31 temporary

	// buttonMap on Xbox
	// 0 = A
	// 1 =B
	// 2 =X
	// 3 =Y
	// 4 = bumperLeft
	// 5 = bumperRight
	// 6 = select
	// 7 = start
	// 8 = joystickLeftPressed
	// 9 = joystickRightPressed
	mapping.key.down.controller = 31;
	mapping.key.left.controller = 31;
	mapping.key.right.controller = 31;
	mapping.key.jump.controller = 0;
	mapping.key.attack.controller = 2;
	mapping.key.dashing.controller = 5;
	mapping.key.resize.controller = 31;

	mapping.key.bulletTime.controller = 4;
	mapping.key.pause.controller = 7;

	// Menu Manette

	mapping.keyMenu.down.controller = 31;
	mapping.keyMenu.up.controller = 31;
	mapping.keyMenu.right.controller = 31;
	mapping.keyMenu.left.controller = 31;
	mapping.keyMenu.click.controller = 0;

	// Axis 
	// 0 = horizontal left stick
	// 1 = vertical left stick
	// 2 = both trigger, but left is Positive, right is negative (triggers is L2/R2 for Pierre)
	// 3 = ????
	// 4  = horizontal right sitck
	// 5=  vertical right stick
	// 6= vertical D-pad
	// 7 = horizontal D-pad
	//mapping.key.down.axis.axisID = 1;
	//mapping.key.down.axis.direction = POSITIVE;

	mapping.key.down.axis.axisID = 2;
	mapping.key.down.axis.direction = NEGATIVE;
	mapping.key.left.axis.axisID = 0;
	mapping.key.left.axis.direction = NEGATIVE;
	mapping.key.right.axis.axisID = 0;
	mapping.key.right.axis.direction = POSITIVE;
	mapping.key.jump.axis.axisID = 31;
	mapping.key.attack.axis.axisID = 31;
	mapping.key.dashing.axis.axisID = 31;
	mapping.key.resize.axis.axisID = 2;
	mapping.key.resize.axis.direction = POSITIVE;
	mapping.key.bulletTime.axis.axisID = 31;
	mapping.key.bulletTime.axis.direction = NEGATIVE;
	mapping.key.pause.axis.axisID = 31;


	//Menu 
	mapping.keyMenu.down.axis.axisID = 1;
	mapping.keyMenu.down.axis.direction = POSITIVE;
	mapping.keyMenu.up.axis.axisID = 1;
	mapping.keyMenu.up.axis.direction = NEGATIVE;
	mapping.keyMenu.left.axis.axisID = 0;
	mapping.keyMenu.left.axis.direction = NEGATIVE;
	mapping.keyMenu.right.axis.axisID = 0;
	mapping.keyMenu.right.axis.direction = POSITIVE;
	mapping.keyMenu.click.axis.axisID = 31;
	mapping.keyMenu.click.axis.direction = POSITIVE;

}

void LoadControllKeyboard()
{
	// unhandled are set to -1
	mapping.key.down.keyboard = sfKeyS;
	mapping.key.left.keyboard = sfKeyQ;
	mapping.key.right.keyboard = sfKeyD;
	mapping.key.jump.keyboard = sfKeySpace;
	mapping.key.attack.keyboard = sfKeyI;
	mapping.key.dashing.keyboard = sfKeyLShift;
	mapping.key.bulletTime.keyboard = sfKeyP;
	mapping.key.resize.keyboard = sfKeyO;
	mapping.key.pause.keyboard = sfKeyEscape;

	mapping.keyMenu.down.keyboard = sfKeyS;
	
	mapping.keyMenu.up.keyboard = sfKeyZ;

	mapping.keyMenu.left.keyboard = sfKeyQ;
	
	mapping.keyMenu.right.keyboard = sfKeyD;
	mapping.keyMenu.click.keyboard = sfKeySpace;

}

sfBool CheckMapping(MappingButton _button)
{

	if (sfKeyboard_isKeyPressed(_button.keyboard))
	{
		return sfTrue;
	}
	else if (sfJoystick_isButtonPressed(0, (int)_button.controller))
	{

		return sfTrue;
	}
	else if (sfJoystick_getAxisPosition(0, _button.axis.axisID) > MIN_DETECTION_CONTROLLER && _button.axis.direction == POSITIVE)
	{

		return sfTrue;
	}
	else if ((sfJoystick_getAxisPosition(0, _button.axis.axisID) < -MIN_DETECTION_CONTROLLER && _button.axis.direction == NEGATIVE))
	{

		return sfTrue;
	}

	return sfFalse;
}

MappingList* GetMapping()
{
	return &mapping.key;
}

MappingMenuList* GetMappingMenu()
{
	return &mapping.keyMenu;
}

// if the stick link to the event is detected, return a float between 0 and 1 else return 1
float GetAxisValue(MappingButton _button)
{
	float valueAxis = 1;
	if (sfJoystick_getAxisPosition(0, _button.axis.axisID) > MIN_DETECTION_CONTROLLER || sfJoystick_getAxisPosition(0, _button.axis.axisID) < -MIN_DETECTION_CONTROLLER)
	{
		valueAxis = abs(sfJoystick_getAxisPosition(0, _button.axis.axisID)) / 100;

	}
	return valueAxis;
}