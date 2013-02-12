#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

#include "ISpecificStat.h"
#include "GameStat.h"

class IGameState
{
private :
	GameStat * gameStat;

public :
	static const int ILLEGAL_GAME = -20000;

	IGameState() {
		gameStat = NULL; 
	};
	virtual ~IGameState() {
		if(gameStat != NULL)
			delete gameStat;
	};
	virtual ISpecificStat * GetGameSpecificStat() = 0;
	virtual int GetPlayerChoises(int whoAskID) = 0;
	virtual int GetActivePlayerID() const = 0;
	virtual int GetPlayerRank(int playerID, int whoAskID) = 0;
	virtual IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates) = 0;
	//virtual IGameState * GetRandomState() = 0;
	//virtual GetPlayerStatus(int playerID, int whoAskID) = 0;
	virtual bool IsGameOver() = 0;
	virtual void PrintToFile(const char * firstLine) {};
	virtual IGameState * Clone() = 0;

	void SetGameStat(GameStat * _gameStat) { 
		if(gameStat != NULL)
			delete gameStat;

		gameStat = new GameStat(*_gameStat); 
	};
	GameStat * Statistics() { return gameStat;};
};

#endif