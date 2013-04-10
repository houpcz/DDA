#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "EnvironmentAI.h"

using namespace std;

class EMonteCarlo :
	public EnvironmentAI
{
private :

public:
	EMonteCarlo(int _myID);
	~EMonteCarlo(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("E MonteCarlo"); };
	IPlayer * Factory(int playerID) { 
		EMonteCarlo * player =  new EMonteCarlo(playerID); 
		player->level = level;
		player->SetMetricCoefs(CoefMetric());
		return player;
	};
	bool CoefsHaveMeaning() { return true; };
};


