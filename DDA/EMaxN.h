#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "EnvironmentAI.h"
#include "IGameState.h"

using namespace std;

class EMaxN :
	public EnvironmentAI
{
private :
	mt19937 * generator;
	float * MaxN(IGameState * state, int depth);
public:
	EMaxN(int _myID);
	~EMaxN(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("E MaxN"); };
	IPlayer * Factory(int playerID) { 
		EMaxN * player =  new EMaxN(playerID); 
		player->level = level;
		player->SetMetricCoefs(CoefMetric());
		return player;
	};
	bool CoefsHaveMeaning() { return true; };
};


