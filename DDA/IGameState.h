#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

class IGameState
{
public :
	virtual int GetPlayerChoises() const = 0;
	virtual int GetActivePlayerID() const = 0;
};

#endif