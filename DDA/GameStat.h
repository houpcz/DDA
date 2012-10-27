#ifndef _GAMESTAT_H_
#define _GAMESTAT_H_

class PlayerStat
{
	int choisesSum, choisesMin, choisesMax;
	int AIhelpedMe;
	int AIdamagedMe;
};

class GameStat
{
private :
	int numberPlayers;
	int turnNumber;
	PlayerStat * playerStat;
	int sureWinner;
	int sureLooser;
public:
	GameStat(int _numberPlayers);
	~GameStat(void);
};

#endif