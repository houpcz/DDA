#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "EnvironmentAI.h"
#include "IGameState.h"

using namespace std;

class EMaxMax :
	public EnvironmentAI
{
private :
	int leaves;
	double bestTurnValue;
	int treeDepth;
	int maxChoises;
public:
	EMaxMax(int _myID);
	~EMaxMax(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	void MaxMax(IGameState * state, int depth);
	QString GetAIName() { return QString("E MaxMax"); };
	IPlayer * Factory(int playerID) { 
		EMaxMax * player =  new EMaxMax(playerID); 
		player->level = level;
		player->SetMetricCoefs(CoefMetric());
		return player;
	};
	bool CoefsHaveMeaning() { return true; };
};

