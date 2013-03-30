#include "GameStat.h"

GameStat::GameStat()
{
	GameStat(2);
}
GameStat::GameStat(int _numberPlayers)
{
	numberPlayers = _numberPlayers;
	playerStat = new PlayerStat[numberPlayers];
	gameStat = NULL;
	Reset();
}

void GameStat::Reset()
{
	for(int loop1 = 0; loop1 < numberPlayers; loop1++)
	{
		playerStat[loop1].Reset();
	}
	turnNumber = 0;
	turnNumberReal = 0;
	leaderSwitches = 0;
	sumRankDifference = 0;
	endRankDifference = 0;
	controlSum = 0;
	credibilitySum = 0.0f;
	justice = 0.0f;
	leaderTime = 0.0f;
	credibility = 0.0f;
	randomness = 0.0f;

	if(gameStat != NULL)
		gameStat->Reset();
}

GameStat::~GameStat(void)
{
	delete [] playerStat;
	if(gameStat != NULL)
	{
		delete gameStat;
		gameStat = NULL;
	}
}

void GameStat::CountMetrics()
{
	int player0turn = turnNumber - turnNumberReal;
	int realNumberPlayers = numberPlayers - 1;

	if(player0turn == 0)
	{
		randomness = 0.0f;
		credibility = 0.0f;
	} else {
		randomness = controlSum / (float) (realNumberPlayers * player0turn);
		credibility = credibilitySum / player0turn;
	}

	float muJustice = 0.0f;
	float muLeaderTime = 0.0;
	float * sP = new float[numberPlayers];
	for(int loop1 = 0; loop1 < realNumberPlayers; loop1++)
	{
		if(player0turn == 0)
		{
			sP[loop1] = 0.0f;
		} else
			sP[loop1] = playerStat[loop1 + 1].DeltaH() / (float) player0turn;

		muJustice += sP[loop1];
		muLeaderTime += playerStat[loop1 + 1].LeaderTime();
	}
	muJustice /= realNumberPlayers;
	muLeaderTime /= realNumberPlayers;
	float sumJustice = 0.0;
	float sumLeaderTime = 0.0;
	for(int loop1 = 0; loop1 < realNumberPlayers; loop1++)
	{
		float dJ = muJustice - sP[loop1];
		sumJustice += dJ * dJ;
		float dL = muLeaderTime - playerStat[loop1 + 1].LeaderTime();
		sumLeaderTime += dL * dL;
	}
	justice = sqrt(sumJustice / realNumberPlayers);
	leaderTime = sqrt(sumLeaderTime / realNumberPlayers);

	delete [] sP;
}

GameStat::GameStat(const GameStat &origin)
{
	CopyToMe(origin);
}

void GameStat::CopyToMe(const GameStat &origin)
{
	numberPlayers = origin.numberPlayers;
	turnNumber = origin.turnNumber;
	turnNumberReal = origin.turnNumberReal;
	leaderSwitches = origin.leaderSwitches;
	sumRankDifference = origin.sumRankDifference;
	endRankDifference = origin.endRankDifference;
	controlSum = origin.controlSum;
	leaderTime = origin.leaderTime;
	justice = origin.justice;
	credibility = origin.credibility;
	credibilitySum = origin.credibilitySum;
	randomness = origin.randomness;

	if(origin.gameStat == NULL)
		gameStat = NULL;
	else {
		gameStat = origin.gameStat->CopyYourself();
	}

	playerStat = new PlayerStat[numberPlayers];

	for(int loop1 = 0; loop1 < numberPlayers; loop1++)
	{
		playerStat[loop1] = origin.playerStat[loop1];
	}
}

const GameStat GameStat::operator+(const GameStat &other)
{
	if(numberPlayers != other.numberPlayers)
	{
		throw "Different player numbers";
	}
	GameStat newGameStat(other.numberPlayers);
	for(int loop1 = 0; loop1 < numberPlayers; loop1++)
	{
		newGameStat.playerStat[loop1] = playerStat[loop1] + other.playerStat[loop1];
	}
	newGameStat.turnNumber = turnNumber + other.turnNumber;
	newGameStat.turnNumberReal = turnNumberReal + other.turnNumberReal;
	newGameStat.leaderSwitches = leaderSwitches + other.leaderSwitches;
	newGameStat.sumRankDifference = sumRankDifference + other.sumRankDifference;
	newGameStat.endRankDifference = endRankDifference + other.endRankDifference;
	newGameStat.controlSum = controlSum + other.controlSum;
	newGameStat.leaderTime = leaderTime + other.leaderTime;
	newGameStat.credibility = credibility + other.credibility;
	newGameStat.credibilitySum = credibilitySum + other.credibilitySum;
	newGameStat.randomness = randomness + other.randomness;
	newGameStat.justice = justice + other.justice;

	if(gameStat != NULL && other.gameStat != NULL)
		newGameStat.gameStat = gameStat->Plus(other.gameStat);
	else if(other.gameStat != NULL)
	{
		newGameStat.gameStat = other.gameStat->CopyYourself();
	} else {
		newGameStat.gameStat = NULL;
	}


	return newGameStat;
}
 GameStat& GameStat::operator=(const GameStat &origin) {
    if (this != &origin) {
      delete playerStat;
	  if(gameStat != NULL)
	  {
		  delete gameStat;
		  gameStat = NULL;
	  }
    }
	
	CopyToMe(origin);

    return *this;
  }
