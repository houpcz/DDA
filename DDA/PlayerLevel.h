#ifndef _PLAYERLEVEL_H_
#define _PLAYERLEVEL_H_

#include <vector>
#include <random>
#include <cmath>
#include "EnvironmentAI.h"
#include "IGame.h"

using namespace std;

class PlayerLevel :
	public EnvironmentAI
{
private :
	void UpdateLevel();
public:
	PlayerLevel(int _myID);
	~PlayerLevel(void);
	virtual void StartGame(IGame * _game);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	QString GetAIName() { return QString::fromLocal8Bit("Dynamická úroveò"); };
	IPlayer * Factory(int playerID) { return new PlayerLevel(playerID); };
	bool IsCompatibleWithGame(int gameID) { return gameID == GAME_LC_ID || gameID == GAME_LUDO_ID; }
};



#endif