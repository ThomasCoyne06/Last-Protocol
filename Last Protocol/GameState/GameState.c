#include "GameState.h"

GameState gameState;

GameState GetGameState(void)
{
	return gameState;
}

void SetGameState(GameState _gameState)
{
	gameState = _gameState;
}
