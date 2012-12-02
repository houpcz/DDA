#include "Game.h"
#include "Human.h"
#include "PlayerRandomAI.h"

Game::Game(QWidget * _widget, bool _paint)
{
	widget = _widget;
	paint = _paint;
	state = STATE_STOPPED;
	playerCount = 0;
	player = NULL;
	gameStat = NULL;
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

void Game::StartGame()
{
	if(gameStat != NULL)
		delete gameStat;

	gameStat = new GameStat(playerCount - 1);
}

void Game::NextTurn()
{
	int turnNumber = 0;
	while(true)
	{
		if(player[GetCurrentState()->GetActivePlayerID()]->IsReady())
		{
			turnNumber++;

			gameStat->AddTurnNumber();

			if(GetCurrentState()->GetActivePlayerID() != ENVIRONMENTAL_AI_ID)
				gameStat->AddTurnNumberReal();

			if(PlayerTurn())
			{
				state = STATE_GAME_OVER;
				break;
			}

			if(paint)
				widget->repaint();
			
			IGameState * gameState = GetCurrentState();
			int activePlayerID = gameState->GetActivePlayerID();
			if(!player[activePlayerID]->Think())
				break;
		}
	}
	if(paint)
		widget->repaint();
}

void Game::Paint(QPainter * painter)
{
	if(paint)
		Draw(painter, 0);
}

void Game::SetPlayer(int playerID, int aiID)
{
	QObject::disconnect(player[playerID], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
	delete player[playerID];
	
	switch(aiID)
	{
		case 0 : player[playerID] = new Human(playerID); break;
		case 1 : player[playerID] = new PlayerRandomAI(playerID); break;
	}

	QObject::connect(player[playerID], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
}