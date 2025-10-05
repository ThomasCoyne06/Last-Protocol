#ifndef COMMON_H
#define COMMON_H

// Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/Window/Joystick.h>
#include <SFML/Audio.h>

#include "../GameState/GameState.h"
#include "../Utility/Animation.h"
#include "../Utility/Collision.h"
#include "../Utility/Creation.h"
#include "../Utility/LinkedList.h"
#include "../Utility/Mathematique.h"
#include "../Utility/FileReader.h"
//#include "../Utility/Transition.h"
#include "../Score.h"
//#include "Transition.h"
//#include "TiledReader.h"

#define SCALE 3 //Global Scale

#define SCREEN_WIDTH 640 * SCALE //1920
#define SCREEN_HEIGHT 360 * SCALE //1080

#define GRAVITY  1500 
#define GRAVITY_LIMIT 4000

#define OFFSET_COL 0.5f//avoid sprite vibration
#define HITBOX_THICKNESS 0.5f

//controller define
#define SENSI_HORIZONTAL_CONTROLLER 300
#define SENSI_VERTICAL_CONTROLLER 300


#endif
