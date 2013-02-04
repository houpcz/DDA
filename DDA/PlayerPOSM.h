#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"

using namespace std;

class PlayerPOSM :
	public IPlayer
{
private :
	void UpdateDifficultyLevel();
	static const int W_LENGTH = 50;
	double w[W_LENGTH];
	double A[W_LENGTH];
	double B[W_LENGTH];
	double beta;
	int kdiff;
public:
	PlayerPOSM(int _myID);
	~PlayerPOSM(void);
	virtual bool Think();
	virtual void StartGame(IGame * _game);
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	QString GetAIName() { return QString("POSM"); };
	IPlayer * Factory(int playerID) { return new PlayerPOSM(playerID); };
};



