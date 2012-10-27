#include "GameStat.h"


GameStat::GameStat(int _numberPlayers)
{
	numberPlayers = _numberPlayers;
	playerStat = new PlayerStat[numberPlayers];
}


GameStat::~GameStat(void)
{
	delete [] playerStat;
}
