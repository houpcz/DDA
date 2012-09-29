#include "Game.h"


Game::Game(QWidget * _widget)
{
	widget = _widget;
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
	while(true)
	{
		if(player[activePlayerID]->IsReady())
		{
			if(!PlayerTurn())
			{
				// END_GAME
				return;
			}

			widget->repaint();
			activePlayerID++;
			activePlayerID %= playerCount;

			if(!player[activePlayerID]->Think())
				break;
		}
	}
}