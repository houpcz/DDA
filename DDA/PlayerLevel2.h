#ifndef _PLAYERLEVEL2_H_
#define _PLAYERLEVEL2_H_

#include <vector>
#include <random>
#include <cmath>
#include "iplayer.h"

using namespace std;

class PlayerLevel2 :
	public IPlayer
{
private :
	void UpdateLevel();
public:
	PlayerLevel2(int _myID);
	~PlayerLevel2(void);
	virtual void StartGame(IGame * _game);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	QString GetAIName() { return QString("Dynamic level 2"); };
	IPlayer * Factory(int playerID) { return new PlayerLevel2(playerID); };
};



#endif