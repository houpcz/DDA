#ifndef _GAMESTAT_H_
#define _GAMESTAT_H_

#define MAX_INT 1000000

class PlayerStat
{
	long choisesSum, choisesMin, choisesMax;
	long AIhelpedMe;
	long AIdamagedMe;

public :
	const PlayerStat operator+(const PlayerStat &other)
	{
		PlayerStat newPlayerStat;
		newPlayerStat.choisesSum = choisesSum + other.choisesSum;
		newPlayerStat.choisesMin = choisesMin + other.choisesMin;
		newPlayerStat.choisesMax = choisesMax + other.choisesMax;
		newPlayerStat.AIhelpedMe = AIhelpedMe + other.AIhelpedMe;
		newPlayerStat.AIdamagedMe = AIdamagedMe + other.AIdamagedMe;
		return newPlayerStat;
	}
	void Reset() {
		choisesSum = 0;
		choisesMin = MAX_INT;
		choisesMax = 0;
		AIhelpedMe = 0;
		AIdamagedMe = 0;
	}
};

class GameStat
{
private :
	long numberPlayers;
	// counts all plie number - environmental ai too
	long turnNumber;
	// doesnt count environmental ai turn
	long turnNumberReal;
	PlayerStat * playerStat;
	long sureWinner;
	long sureLooser;

	void CopyToMe(const GameStat &origin);
public:
	GameStat();
	GameStat(int _numberPlayers);
	GameStat(const GameStat &origin);
	~GameStat(void);
	GameStat& GameStat::operator=(const GameStat &origin);
	const GameStat operator+(const GameStat &other);

	void Reset();
	void AddTurnNumber() { turnNumber++; }
	void AddTurnNumberReal() { turnNumberReal++; }

	int TurnNumber() { return turnNumber; }
	int TurnNumberReal() { return turnNumberReal; }
};

#endif