#ifndef _PLAYERRANDOMAI_H_
#define _PLAYERRANDOMAI_H_

#include "iplayer.h"
class PlayerRandomAI :
	public IPlayer
{
public:
	PlayerRandomAI(int _myID);
	virtual ~PlayerRandomAI(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
	virtual bool IsScalable() { return false; }
	QString GetAIName() { return QString::fromLocal8Bit("N�hodn� AI"); };
	IPlayer * Factory(int playerID) { return new PlayerRandomAI(playerID); };
	bool IsCompatibleWithGame(int gameID) { return true; }
};

#endif