#ifndef _MAXNPLAYER_H_
#define _MAXNPLAYER_H_

#include <vector>
#include <random>
#include <cmath>
#include "IGameState.h" 
#include "iplayer.h"
#include "IGame.h"

using namespace std;

class MaxNPlayer :
	public IPlayer
{
private:
	int leafNumber;
	float * MaxN(IGameState * state, int depth);
public:
	MaxNPlayer(int _myID);
	~MaxNPlayer(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return true; }
	QString GetAIName() { return QString("Max N"); };
	IPlayer * Factory(int playerID) { return new MaxNPlayer(playerID); };
	bool IsCompatibleWithGame(int gameID) { return gameID == GAME_LUDO_ID; }
};

#endif