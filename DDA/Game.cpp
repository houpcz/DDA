#include "Game.h"


Game::Game(void)
{
	isRunning = false;
	playerCount = 0;
	player = NULL;
}


Game::~Game(void)
{
	if(playerCount > 0)
	{
		for(int loop1 = 0; loop1 < playerCount; loop1++)
		{
			delete player[loop1];
		}
		delete player;
	}
}

void Game::NextTurn()
{
	if(player[activePlayerID]->IsReady())
	{
		if(!PlayerTurn())
		{
			// END_GAME
			return;
		}

		activePlayerID++;
		activePlayerID %= playerCount;

		player[activePlayerID]->Think();
	}
}