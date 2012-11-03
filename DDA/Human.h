#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "iplayer.h"

class Human :
	public IPlayer
{
Q_OBJECT

public:
	Human(void);
	virtual ~Human(void);
	virtual bool Think() { return false; };
	void HumanTurn(int turn);
	QString GetAINAme() { return QString("Human"); };
};

#endif

