#pragma once
#include "iplayer.h"
class PlayerMonteCarlo :
	public IPlayer
{
public:
	PlayerMonteCarlo(int _myID);
	virtual ~PlayerMonteCarlo(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("Monte Carlo"); };
	IPlayer * Factory(int playerID) { return new PlayerMonteCarlo(playerID); };
};

