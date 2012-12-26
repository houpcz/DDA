#pragma once
#include "iplayer.h"
class PlayerHillClimber :
	public IPlayer
{
public:
	PlayerHillClimber(int _myID);
	~PlayerHillClimber(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	QString GetAINAme() { return QString("Hill Climber"); };
};

