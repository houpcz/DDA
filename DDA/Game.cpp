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
		delete player;
		delete [] currentPlayerScore;
	}	
}

void Game::StartGame()
{
	if(gameStat != NULL)
		delete gameStat;
	if(currentPlayerScore == NULL)
		currentPlayerScore = new int[playerCount - 1];

	for(int loop1 = 0; loop1 < playerCount - 1; loop1++)
	{
		currentPlayerScore[loop1] = 0;
	}

	gameStat = new GameStat(playerCount - 1);
	playerLeader = -1;
}

void Game::NextTurn()
{
	int turnNumber = 0;

	while(turnNumber < 10000)
	{
		IGameState * currentState = GetCurrentState();
		if(player[currentState->GetActivePlayerID()]->IsReady())
		{
			turnNumber++;

			gameStat->AddTurnNumber();

			int currentPlayerID = currentState->GetActivePlayerID();
			
			if(currentPlayerID != ENVIRONMENTAL_AI_ID)
			{
				gameStat->AddTurnNumberReal();
				int playerChoises = currentState->GetPlayerChoises(currentPlayerID);
				gameStat->UpdatePlayerChoises(currentPlayerID - 1, playerChoises);
				gameStat->AddPlayerTurnNumber(currentPlayerID - 1);

				int outScoreDifference;
				int newLeaderID = GetLeaderID(&outScoreDifference);
				gameStat->AddScoreDifference(outScoreDifference);
				if(newLeaderID != playerLeader)
					gameStat->AddLeaderSwitch();
				playerLeader = newLeaderID;
			}

			if(PlayerTurn())
			{
				state = STATE_GAME_OVER;
				
				int outScoreDifference;
				int newLeaderID = GetLeaderID(&outScoreDifference);
				gameStat->AddScoreDifference(outScoreDifference);
				gameStat->SetEndScoreDifference(outScoreDifference);
				gameStat->SetWinner(newLeaderID);
				
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
	return bestID;

	/*
	int maxScore = currentPlayerScore[0];
	int maxSecondScore = currentPlayerScore[1];
	int maxScoreID;
	if(maxScore < maxSecondScore)
	{
		maxScore = currentPlayerScore[1];
		maxSecondScore = currentPlayerScore[0];
		maxScoreID = 1;
	} else {
		maxScoreID = 0;
	}

	for(int loop1 = 2; loop1 < playerCount - 1; loop1++)
	{
		if(currentPlayerScore[loop1] > maxSecondScore)
		{
			if(currentPlayerScore[loop1] > maxScore)
			{
				maxSecondScore = maxScore;
				maxScore = currentPlayerScore[loop1];
				maxScoreID = loop1;
			} else {
				maxSecondScore = currentPlayerScore[loop1];
			}
		}
	}
	*outScoreDifference = maxScore - maxSecondScore;
	return maxScoreID;
	*/
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
		case 2 : player[playerID] = new PlayerHillClimber(playerID); break;
		default :
			throw "Not implemented yet";
			break;
	}

	QObject::connect(player[playerID], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
}