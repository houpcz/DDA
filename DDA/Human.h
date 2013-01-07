#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "iplayer.h"

class Human :
	public IPlayer
{
Q_OBJECT

public:
	Human(int _myID) : IPlayer(_myID) {};
	virtual ~Human(void);
	virtual bool Think() { return false; };
	void HumanTurn(int turn);
	virtual bool IsScalable() { return false; };
	QString GetAIName() { return QString("Human"); };
	IPlayer * Factory(int playerID) { return new Human(playerID); };
};

#endif

