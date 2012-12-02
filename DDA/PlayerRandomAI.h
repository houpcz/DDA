#pragma once
#include "iplayer.h"
class PlayerRandomAI :
	public IPlayer
{
public:
	PlayerRandomAI(int _myID);
	virtual ~PlayerRandomAI(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	QString GetAINAme() { return QString("Random AI"); };
};

