#include "IGameState.h"
#include "Game.h"

IGameState::IGameState()
{
	gameStat = NULL;
	currentPlayerStatus = NULL;
	lastPlayerID = -1;
	leaderTime = 0;

	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
		pStatus[loop1] = 0;
};

IGameState::IGameState(Game * _game)
{
	gameStat = NULL;
	currentPlayerStatus = NULL;
	lastPlayerID = -1;
	leaderTime = 0;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
		pStatus[loop1] = 0;

	Init(_game, this);
};

void IGameState::Init(Game * _game, const IGameState * originState, GameStat * stat)
{
	game = _game;
	lastPlayerID = originState->lastPlayerID;
	playerCount = game->GetPlayerCount();
	if(currentPlayerStatus != NULL || gameStat != NULL)
		int errro = 0;
	currentPlayerStatus = new int[playerCount - 1];
	playerLeader = 0;

	for(int loop1 = 0; loop1 < playerCount - 1; loop1++)
	{
		if(stat == NULL)
			currentPlayerStatus[loop1] = 0;
		else
			currentPlayerStatus[loop1] = originState->currentPlayerStatus[loop1];
	}

	if(stat == NULL)
	{
		gameStat = new GameStat(playerCount); 
	} else {
		gameStat = new GameStat(*stat);
	}
}

IGameState::~IGameState() {
	delete gameStat;
	delete [] currentPlayerStatus;
};

void IGameState::UpdateGameStat()
{
	int realNumberPlayer = gameStat->NumberPlayers() - 1;
	if(gameStat->TurnNumber() != 0)
	{
		for(int loop1 = 0; loop1 < gameStat->NumberPlayers(); loop1++)
		{
			gameStat->AddPlayerLevel(loop1, game->GetPlayer(loop1)->Level());
		}
	}

	gameStat->AddTurnNumber();

	int currentPlayerID = GetActivePlayerID();

	int playerChoises = GetPlayerChoises(currentPlayerID);
	if(currentPlayerID != 0)
		gameStat->AddFreedomSum(Freedom());

	if(lastPlayerID != 0)
	{
		gameStat->AddTurnNumberReal();
		leaderTime++;
	} else {
		gameStat->AddCredibilitySum(GetCredibility());
	}

	int outStatusDifference;
	int newLeaderID = GetLeaderID(&outStatusDifference);
	gameStat->SetEndStatusDifference(abs(outStatusDifference));
	gameStat->AddStatusDifference(abs(outStatusDifference));
	if(newLeaderID != playerLeader) {
		gameStat->AddPlayerLeaderTime(playerLeader, leaderTime);
		gameStat->AddLeaderSwitch();
		leaderTime = 0;
	}
	playerLeader = newLeaderID;

	gameStat->UpdatePlayerChoises(currentPlayerID, playerChoises);
	gameStat->AddPlayerTurnNumber(currentPlayerID);

	int tempStatus;
	int deltaH;
	for(int loop1 = 0; loop1 < realNumberPlayer; loop1++)
	{

		tempStatus = GetPlayerStatus(loop1 + 1);
		deltaH = tempStatus - pStatus[loop1];
		if(lastPlayerID == 0)
		{
			gameStat->AddControlSum(abs(deltaH));
			gameStat->AddPlayerDeltaH(loop1 + 1, deltaH);
		}
		pStatus[loop1] = tempStatus;
	}
	lastPlayerID = currentPlayerID;

	if(IsGameOver())
	{
		int outStatusDifference;
		int newLeaderID = GetLeaderID(&outStatusDifference);
		gameStat->AddStatusDifference(abs(outStatusDifference));
		gameStat->SetWinner(newLeaderID);
		gameStat->SetGameSpecificStat(GetGameSpecificStat());
		gameStat->AddPlayerLeaderTime(playerLeader, leaderTime);
	}
	gameStat->CountMetrics();
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