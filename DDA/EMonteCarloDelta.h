#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "EnvironmentAI.h"

using namespace std;

class EMonteCarloDelta :
	public EnvironmentAI
{
private :

public:
	EMonteCarloDelta(int _myID);
	~EMonteCarloDelta(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("E MonteCarlo Delta"); };
	IPlayer * Factory(int playerID) { 
		EMonteCarloDelta * player =  new EMonteCarloDelta(playerID); 
		player->level = level;
		player->SetMetricCoefs(CoefMetric());
		return player;
	};
	bool CoefsHaveMeaning() { return true; };
};



