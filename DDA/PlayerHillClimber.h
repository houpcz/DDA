#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"

using namespace std;

class PlayerHillClimber :
	public IPlayer
{
private :
	mt19937 * generator;

public:
	PlayerHillClimber(int _myID);
	~PlayerHillClimber(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("Hill Climber"); };
	IPlayer * Factory(int playerID) { return new PlayerHillClimber(playerID); };
};

