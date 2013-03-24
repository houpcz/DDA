#include "IGameState.h"
#include "Game.h"

IGameState::IGameState()
{
	gameStat = NULL;
};

IGameState::IGameState(Game * _game)
{
	Init(_game);
};

void IGameState::Init(Game * _game)
{
	game = _game;
	playerCount = game->GetPlayerCount();
	currentPlayerStatus = new int[playerCount - 1];
	playerLeader = 0;

	for(int loop1 = 0; loop1 < playerCount - 1; loop1++)
	{
		currentPlayerStatus[loop1] = 0;
	}

	gameStat = new GameStat(playerCount); 
}

IGameState::~IGameState() {
	if(gameStat != NULL)
		delete gameStat;

	delete [] currentPlayerStatus;
};

void IGameState::UpdateGameStat()
{
	for(int loop1 = 0; loop1 < gameStat->NumberPlayers(); loop1++)
	{
		gameStat->AddPlayerLevel(loop1, game->GetPlayer(loop1)->Level());
		gameStat->AddTurnNumber();

		int currentPlayerID = GetActivePlayerID();
			
		if(currentPlayerID != 0)
		{
			gameStat->AddTurnNumberReal();

			int outStatusDifference;
			int newLeaderID = GetLeaderID(&outStatusDifference);
			gameStat->AddStatusDifference(outStatusDifference);
			if(newLeaderID != playerLeader)
				gameStat->AddLeaderSwitch();
			playerLeader = newLeaderID;
		}

		int playerChoises = GetPlayerChoises(currentPlayerID);
		gameStat->UpdatePlayerChoises(currentPlayerID, playerChoises);
		gameStat->AddPlayerTurnNumber(currentPlayerID);

		if(IsGameOver())
		{
			int outStatusDifference;
			int newLeaderID = GetLeaderID(&outStatusDifference);
			gameStat->AddStatusDifference(outStatusDifference);
			gameStat->SetEndStatusDifference(outStatusDifference);
			gameStat->SetWinner(newLeaderID);
			gameStat->SetGameSpecificStat(GetGameSpecificStat());
				
			break;
		}
	}
}

int IGameState::GetLeaderID(int * outStatusDifference)
{
	int playerCount = gameStat->NumberPlayers();


	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		currentPlayerStatus[loop1 - 1] = GetPlayerStatus(loop1);
	}

	int bestID = 0;
	int bestStatus = currentPlayerStatus[0];
	for(int loop1 = 1; loop1 < playerCount - 1; loop1++)
	{
		if(currentPlayerStatus[loop1] > bestStatus)
		{ 
			bestStatus = currentPlayerStatus[loop1];
			bestID = loop1;
		}
	}
	// we have almost zero sum games
	// score of winner is difference between best one and second one
	*outStatusDifference = bestStatus;

	if(bestStatus < 0)
		return 0;

	return bestID + 1;
}