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
	sureLooser = 0;
	sureWinner = 0;
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
	sureWinner = origin.sureWinner;
	sureLooser = origin.sureLooser;
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
	newGameStat.sureLooser = sureLooser + other.sureLooser;
	newGameStat.sureWinner = sureWinner + other.sureWinner;

	return newGameStat;
}
 GameStat& GameStat::operator=(const GameStat &origin) {
    if (this != &origin) {
      delete playerStat;
    }
	
	CopyToMe(origin);

    return *this;
  }
