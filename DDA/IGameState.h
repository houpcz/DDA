#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

#include "ISpecificStat.h"
#include "GameStat.h"

class Game;

class IGameState
{
protected :
	GameStat * gameStat;
	int * currentPlayerStatus;
	Game * game;
	int lastPlayerID;
	int playerCount;
	int playerLeader;
	int leaderTime;

	static const int MAX_PLAYER = 4;
	int pStatus[MAX_PLAYER];
public :
	static const int ILLEGAL_GAME = -20000;

	IGameState();
	IGameState(Game * _game);
	void Init(Game * _game, GameStat * stat = NULL);
	virtual ~IGameState();
	virtual ISpecificStat * GetGameSpecificStat() = 0;
	virtual int GetPlayerChoises(int whoAskID) = 0;
	virtual int GetActivePlayerID() const = 0;
	virtual int GetPlayerRank(int playerID, int whoAskID) = 0;
	virtual int GetPlayerStatus(int playerID) = 0;
	virtual IGameState * GetRandomNextState(int whoAskID, int * outStateID) = 0;
	virtual IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates) = 0;
	virtual IGameState * SimulateToTheEnd(int whoAskID) = 0;
	virtual IGameState * GetStateFromSameInformSet(int whoAskID) { return NULL; };

	//virtual IGameState * GetRandomState() = 0;
	//virtual GetPlayerStatus(int playerID, int whoAskID) = 0;
	virtual bool IsGameOver() = 0;
	virtual void PrintToFile(const char * firstLine) {};
	virtual IGameState * Clone() = 0;

	int GetLeaderID(int * outRankDifference);
	void UpdateGameStat();
	GameStat GetGameStat() {
		return *gameStat;
	}
	GameStat * Statistics() { return gameStat;};
};

#endif