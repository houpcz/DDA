#include "Game.h"


Game::Game(QWidget * _widget)
{
	widget = _widget;
	state = STATE_STOPPED;
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
		if(player[GetCurrentState()->GetActivePlayerID()]->IsReady())
		{
			if(PlayerTurn())
			{
				state = STATE_GAME_OVER;
				break;
			}

			widget->repaint();
			
			if(!player[GetCurrentState()->GetActivePlayerID()]->Think())
				break;
		}
	}
	widget->repaint();
}