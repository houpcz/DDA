#pragma once

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"
#include "IGame.h"

using namespace std;

class PlayerHillClimber :
	public IPlayer
{
public:
	PlayerHillClimber(int _myID);
	~PlayerHillClimber(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("Hill Climber"); };
	IPlayer * Factory(int playerID) { 
		PlayerHillClimber * player =  new PlayerHillClimber(playerID); 
		player->level = level;
		return player;
	};
	bool IsCompatibleWithGame(int gameID) { return gameID == GAME_LUDO_ID || gameID == GAME_MAZE_ID; }
};

