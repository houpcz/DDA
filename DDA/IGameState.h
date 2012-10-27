#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

class IGameState
{
public :
	static const int WINNER_SCORE = 10000;

	virtual int GetPlayerChoises() const = 0;
	virtual int GetActivePlayerID() const = 0;
	virtual int GetPlayerScore(int playerID) const = 0;
};

#endif