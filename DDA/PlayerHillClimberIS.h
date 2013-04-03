#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "IGame.h"

using namespace std;

class PlayerHillClimberIS :
	public IPlayer
{
private :
	mt19937 * generator;

public:
	PlayerHillClimberIS(int _myID);
	~PlayerHillClimberIS(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("Hill Climber IS"); };
	IPlayer * Factory(int playerID) { return new PlayerHillClimberIS(playerID); };
	bool IsCompatibleWithGame(int gameID) { return gameID == GAME_LC_ID; }
};

