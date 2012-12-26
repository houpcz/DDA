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
	QString GetAINAme() { return QString("Human"); };
};

#endif

