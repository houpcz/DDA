#ifndef _GAME_H_
#define _GAME_H_

#include "igame.h"
#include "IPlayer.h"
#include <qobject.h>
#include <qwidget.h>

class Game : public IGame
{
	Q_OBJECT

protected:
	bool isRunning;
	int playerCount;
	int activePlayerID;
	IPlayer ** player;

	QWidget * widget;
public:
	Game(QWidget * _widget);
	virtual ~Game(void);
	virtual int * GetPlayerChoises() = 0;
	virtual bool PlayerTurn() = 0; /// Game specific turn
	virtual void NextTurn();       /// Common for all games
};

#endif
