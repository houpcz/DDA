#ifndef _MINMAXISPLAYER_H_
#define _MINMAXISPLAYER_H_

#include <vector>
#include <random>
#include <cmath>
#include "IGameState.h" 
#include "iplayer.h"

using namespace std;

class MinMaxISPlayer :
	public IPlayer
{
private:
	int leafNumber;
	mt19937 * generator;
	float MiniMax(IGameState * state, float alfa, float beta, int depth);
public:
	MinMaxISPlayer(int _myID);
	~MinMaxISPlayer(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	QString GetAIName() { return QString("Mini Max IS"); };
	IPlayer * Factory(int playerID) { return new MinMaxISPlayer(playerID); };
};

#endif