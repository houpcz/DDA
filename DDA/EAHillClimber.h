#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "EnvironmentAI.h"

using namespace std;

class EAHillClimber :
	public EnvironmentAI
{
private :
	mt19937 * generator;

public:
	EAHillClimber(int _myID);
	~EAHillClimber(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("EA Hill Climber"); };
	IPlayer * Factory(int playerID) { return new EAHillClimber(playerID); };
	bool CoefsHaveMeaning() { return true; };
};

