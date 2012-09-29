#ifndef _GAME_H_
#define _GAME_H_

#include "igame.h"
#include "IPlayer.h"
#include <qobject.h>

class Game : public IGame
{
	Q_OBJECT

protected:
	bool isRunning;
	int playerCount;
	int activePlayerID;
	IPlayer ** player;
public:
	Game(void);
	virtual ~Game(void);
	virtual int * GetEnvironmentChoises() = 0;
	virtual bool PlayerTurn() = 0; /// Game specific turn
	virtual void NextTurn();       /// Common for all games
};

#endif
