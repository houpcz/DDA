#include "Game.h"
#include "Human.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"

Game::Game(QWidget * _widget, bool _paint)
{
	widget = _widget;
	paint = _paint;
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
		delete [] player;
	}	

	ClearAllGameStates();
}

void Game::StartGame()
{
	turnNumber = 0;
	ClearAllGameStates();
	playerLeader = -1;
}

void Game::ClearAllGameStates()
{
	for(int loop1 = 0; loop1 < gameState.size(); loop1++)
		delete gameState[loop1];
	gameState.clear();
}

void Game::NextTurn()
{
	int MAX_TURN_NUMBER = 5000;
	while(turnNumber < MAX_TURN_NUMBER)
	{
		IGameState * currentState = GetCurrentState();

		if(player[currentState->GetActivePlayerID()]->IsReady())
		{

			turnNumber++;
			

			PlayerTurn();

			if(SaveAllGameStates())
			{
				IGameState * newGameState = currentState->Clone();
				gameState.push_back(newGameState);
			}

			if(currentState->IsGameOver())
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

	if(turnNumber == MAX_TURN_NUMBER)
	{
		//gameStat->SetWinner(0);
	}

	if(paint)
		widget->repaint();
}

void Game::Paint(QPainter * painter)
{
	if(paint) {
		Draw(painter, 0);
		if(state != STATE_STOPPED)
		{
			IGameState * currentState = GetCurrentState();
			if(currentState->IsGameOver())
			{
				GameStat stat = currentState->GetGameStat();
				long long score = stat.EndRankDifference();
				
				int leaderID = 0;
				for(int loop1 = 0; loop1 < playerCount; loop1++)
				{
					if(stat.PlayerWinner(loop1) > 0)
					{
						leaderID = loop1;
						break;
					}
				}

				QString winnerText;
				QColor winnerColor;
				if(score < 0)
				{
					winnerColor = Qt::red;
					winnerText = "You lose!";
				} else {
					winnerColor = Qt::darkCyan;
					if(playerCount <= 2)
						winnerText = "You won!";
					else
						winnerText = QString("Player ") + QString::number(leaderID) + QString(" won!");
				}

				painter->setFont(QFont("Helvetica", 28, QFont::Bold));
				painter->setPen(Qt::black);
				painter->drawText(0, 0, painter->viewport().width() - 3, painter->viewport().height() - 43, Qt::AlignCenter, winnerText);
				painter->setPen(winnerColor);
				painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height() - 40, Qt::AlignCenter, winnerText);
			}
		}
	}
}

void Game::SetPlayer(int playerID, IPlayer * _player)
{
	QObject::disconnect(player[playerID], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
	delete player[playerID];
	
	player[playerID] = _player->Factory(playerID);
	player[playerID]->SetGame(this);

	QObject::connect(player[playerID], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
}