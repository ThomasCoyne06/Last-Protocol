#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../Utility/Common.h"

typedef enum
{
	LAUNCHING_GAME,
	MENU,
	CUTSCENE,
	GAME,
	GAME_OVER,
	WIN,
	NB_GAME_STATE
}GameState;

GameState GetGameState(void);
void SetGameState(GameState _gameState);

#endif
