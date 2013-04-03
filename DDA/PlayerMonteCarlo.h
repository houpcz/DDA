#pragma once
#include "iplayer.h"
#include "IGame.h"

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
	IPlayer * Factory(int playerID) { 
		PlayerMonteCarlo * player = new PlayerMonteCarlo(playerID); 
		player->level = level;
		return player;
	};
	bool IsCompatibleWithGame(int gameID) { return gameID == GAME_LUDO_ID || gameID == GAME_LC_ID; }
};

