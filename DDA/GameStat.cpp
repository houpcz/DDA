#include "GameStat.h"

GameStat::GameStat()
{
	GameStat(2);
}
GameStat::GameStat(int _numberPlayers)
{
	numberPlayers = _numberPlayers;
	playerStat = new PlayerStat[numberPlayers];
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
	sumScoreDifference = 0;
	endScoreDifference = 0;
}

GameStat::~GameStat(void)
{
	delete [] playerStat;
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
	sumScoreDifference = origin.sumScoreDifference;
	endScoreDifference = origin.endScoreDifference;
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
	newGameStat.sumScoreDifference = sumScoreDifference + other.sumScoreDifference;
	newGameStat.endScoreDifference = endScoreDifference + other.endScoreDifference;

	return newGameStat;
}
 GameStat& GameStat::operator=(const GameStat &origin) {
    if (this != &origin) {
      delete playerStat;
    }
	
	CopyToMe(origin);

    return *this;
  }
