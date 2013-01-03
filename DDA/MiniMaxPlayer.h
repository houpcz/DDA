#ifndef _MINIMAXPLAYER_H_
#define _MINIMAXPLAYER_H_

#include <vector>
#include <random>
#include <cmath>
#include "IGameState.h" 
#include "iplayer.h"

using namespace std;

class MiniMaxPlayer :
	public IPlayer
{
private:
	int leafNumber;
	mt19937 * generator;
	float * MaxN(IGameState * state, int depth);
public:
	MiniMaxPlayer(int _myID);
	~MiniMaxPlayer(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAINAme() { return QString("Mini Max"); };
};

#endif