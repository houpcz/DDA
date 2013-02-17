#ifndef _GAMESTAT_H_
#define _GAMESTAT_H_

#include "ISpecificStat.h"

#define MAX_INT 1000000


class PlayerStat
{
	long long choisesSum, choisesMin, choisesMax;
	long long winner;
	long long turnNumber;
	long long level;
public :
	const PlayerStat operator+(const PlayerStat &other)
	{
		PlayerStat newPlayerStat;
		newPlayerStat.choisesSum = choisesSum + other.choisesSum;
		newPlayerStat.choisesMin = choisesMin + other.choisesMin;
		newPlayerStat.choisesMax = choisesMax + other.choisesMax;
		newPlayerStat.winner = winner + other.winner;
		newPlayerStat.turnNumber = turnNumber + other.turnNumber;
		newPlayerStat.level = level + other.level;
		return newPlayerStat;
	}
	void AddLevel(int _level)
	{
		level += _level;
	}

	void SetWinner() {
		winner = 1;
	}
	void AddTurn()
	{
		turnNumber++;
	}
	void UpdateChoises(int choises)
	{
		choisesSum += choises;
		if(choises < choisesMin)
			choisesMin = choises;
		if(choises > choisesMax)
			choisesMax = choises;
	}
	void Reset() {
		choisesSum = 0;
		choisesMin = MAX_INT;
		choisesMax = 0;
		winner = 0;
		turnNumber = 0;
		level = 0;
	}

	long long Winner() { return winner; };
	long long ChoisesMin() { return choisesMin; };
	long long ChoisesMax() { return choisesMax; };
	long long ChoisesSum() { return choisesSum; };
	long long TurnNumber() { return turnNumber; };
	long long Level() { return level;};
};

class GameStat
{
private :
	long numberPlayers;
	// counts all plie number - environmental ai too
	long long turnNumber;
	// doesnt count environmental ai turn
	long long turnNumberReal;

	long long leaderSwitches;
	long long sumRankDifference;
	long long endRankDifference;

	PlayerStat * playerStat;
	ISpecificStat * gameStat; 

	void CheckPlayerID(int playerID)
	{
		if(playerID < 0 || playerID >= numberPlayers)
		{
			throw "GameStat.h -> bad playerID";
		}
	}
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
	void AddLeaderSwitch() { leaderSwitches++; };
	void AddStatusDifference(int difference) { sumRankDifference += difference; };
	void SetEndStatusDifference(int difference) { endRankDifference = difference; };

	void AddPlayerLevel(int playerID, int _level) {
		CheckPlayerID(playerID);
		playerStat[playerID].AddLevel(_level);
	}
	void AddPlayerTurnNumber(int playerID) { 
		CheckPlayerID(playerID);
		playerStat[playerID].AddTurn(); 
	};
	void SetWinner(int playerID) { 
		CheckPlayerID(playerID);
		playerStat[playerID].SetWinner(); 
	};
	void UpdatePlayerChoises(int playerID, int playerChoises) { 
		CheckPlayerID(playerID);
		playerStat[playerID].UpdateChoises(playerChoises); 
	};

	void SetGameSpecificStat(ISpecificStat * _gameStat) 
	{
		if(gameStat != NULL)
			delete gameStat;
		gameStat = _gameStat;
	}
	QString GameStatHeader() { 
		if(gameStat == NULL)
			return "";
		return gameStat->Header(); 
	};
	QString GameStatRecord() { 
		if(gameStat == NULL)
			return "";
		return gameStat->Record(); 
	};
	long NumberPlayers() { return numberPlayers; };
	long long TurnNumber() { return turnNumber; };
	long long TurnNumberReal() { return turnNumberReal; };
	long long LeaderSwitches() { return leaderSwitches; };
	long long SumRankDifference() { return sumRankDifference;};
	long long EndRankDifference() { return endRankDifference;};
	long PlayerWinner(int playerID) { 
		CheckPlayerID(playerID);
		return playerStat[playerID].Winner(); 
	};
	long long PlayerLevel(int playerID)
	{
		CheckPlayerID(playerID);
		return playerStat[playerID].Level(); 
	};
	long long PlayerChoisesMin(int playerID) { 
		CheckPlayerID(playerID);
		return playerStat[playerID].ChoisesMin(); 
	};
	long long PlayerChoisesMax(int playerID) { 
		CheckPlayerID(playerID);
		return playerStat[playerID].ChoisesMax(); 
	};
	long long PlayerChoisesSum(int playerID) { 
		CheckPlayerID(playerID);
		return playerStat[playerID].ChoisesSum(); 
	};
	long long PlayerTurnNumber(int playerID) { 
		CheckPlayerID(playerID);
		return playerStat[playerID].TurnNumber(); 
	};
};

#endif