#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

class IGameState
{
public :
	static const int WINNER_SCORE = 10000;
	static const int ILLEGAL_GAME = -20000;

	virtual ~IGameState() {};
	virtual int GetPlayerChoises(int whoAskID) = 0;
	virtual int GetActivePlayerID() const = 0;
	virtual int GetPlayerScore(int playerID, int whoAskID) = 0;
	virtual IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates) = 0;
	virtual void PrintToFile(const char * firstLine) {};
};

#endif