#pragma once
#include "iplayer.h"
class PlayerRandomAI :
	public IPlayer
{
public:
	PlayerRandomAI(void);
	virtual ~PlayerRandomAI(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	QString GetAINAme() { return QString("Random AI"); };
};

