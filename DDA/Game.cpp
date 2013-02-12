#include "Game.h"
#include "Human.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"
#include "MiniMaxPlayer.h"

Game::Game(QWidget * _widget, bool _paint)
{
	widget = _widget;
	paint = _paint;
	state = STATE_STOPPED;
	playerCount = 0;
	player = NULL;
	gameStat = NULL;
	currentPlayerScore = NULL;
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
		delete [] currentPlayerScore;
	}	

	ClearAllGameStates();
}

void Game::StartGame()
{
	if(gameStat != NULL)
		delete gameStat;
	if(currentPlayerScore == NULL)
	{
		currentPlayerScore = new int[playerCount - 1];
	}

	for(int loop1 = 0; loop1 < playerCount - 1; loop1++)
	{
		currentPlayerScore[loop1] = 0;
	}

	gameStat = new GameStat(playerCount);
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
			for(int loop1 = 0; loop1 < playerCount; loop1++)
			{
				gameStat->AddPlayerLevel(loop1, player[loop1]->Level());
			}

			turnNumber++;
			gameStat->AddTurnNumber();

			int currentPlayerID = currentState->GetActivePlayerID();
			
			if(currentPlayerID != ENVIRONMENTAL_AI_ID)
			{
				gameStat->AddTurnNumberReal();

				int outScoreDifference;
				int newLeaderID = GetLeaderID(&outScoreDifference);
				gameStat->AddScoreDifference(outScoreDifference);
				if(newLeaderID != playerLeader)
					gameStat->AddLeaderSwitch();
				playerLeader = newLeaderID;
			}

			int playerChoises = currentState->GetPlayerChoises(currentPlayerID);
			gameStat->UpdatePlayerChoises(currentPlayerID, playerChoises);
			gameStat->AddPlayerTurnNumber(currentPlayerID);
			

			PlayerTurn();

			if(SaveAllGameStates())
			{
				IGameState * newGameState = currentState->Clone();
				newGameState->SetGameStat(gameStat);
				gameState.push_back(newGameState);
			}

			if(currentState->IsGameOver())
			{
				state = STATE_GAME_OVER;
				
				int outScoreDifference;
				int newLeaderID = GetLeaderID(&outScoreDifference);
				gameStat->AddScoreDifference(outScoreDifference);
				gameStat->SetEndScoreDifference(outScoreDifference);
				gameStat->SetWinner(newLeaderID);
				gameStat->SetGameSpecificStat(currentState->GetGameSpecificStat());
				
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
		gameStat->SetWinner(0);
	}

	if(paint)
		widget->repaint();
}

int Game::GetLeaderID(int * outScoreDifference)
{
	IGameState * currentState = GetCurrentState();

	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		currentPlayerScore[loop1 - 1] = currentState->GetPlayerScore(loop1, 0);
	}

	int bestID = 0;
	int bestScore = currentPlayerScore[0];
	for(int loop1 = 1; loop1 < playerCount - 1; loop1++)
	{
		if(currentPlayerScore[loop1] > bestScore)
		{ 
			bestScore = currentPlayerScore[loop1];
			bestID = loop1;
		}
	}
	// we have almost zero sum games
	// score of winner is difference between best one and second one
	*outScoreDifference = bestScore;

	if(bestScore < 0)
		return 0;

	return bestID + 1;
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
				int score;
				int leaderID = GetLeaderID(&score);

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