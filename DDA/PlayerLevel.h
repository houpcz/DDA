#ifndef _PLAYERLEVEL_H_
#define _PLAYERLEVEL_H_

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"

using namespace std;

class PlayerLevel :
	public IPlayer
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
	QString GetAIName() { return QString("Dynamic level"); };
	IPlayer * Factory(int playerID) { return new PlayerLevel(playerID); };
};



#endif