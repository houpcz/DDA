#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "iplayer.h"

class Human :
	public IPlayer
{
Q_OBJECT

public:
	Human(void);
	~Human(void);
	virtual void Think() {};
	void HumanTurn(int turn);
};

#endif

