#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

#include "ISpecificStat.h"

class IGameState
{
public :
	static const int ILLEGAL_GAME = -20000;

	virtual ~IGameState() {};
	virtual ISpecificStat * GetGameSpecificStat() = 0;
	virtual int GetPlayerChoises(int whoAskID) = 0;
	virtual int GetActivePlayerID() const = 0;
	virtual int GetPlayerScore(int playerID, int whoAskID) = 0;
	virtual IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates) = 0;
	virtual bool IsGameOver() = 0;
	virtual void PrintToFile(const char * firstLine) {};
	virtual IGameState * Clone() = 0;
};

#endif