#ifndef _GAME_H_
#define _GAME_H_

#include "igame.h"
#include "IPlayer.h"
#include <qobject.h>
#include <qwidget.h>

typedef enum State {
	STATE_STOPPED,
	STATE_RUNNING,
	STATE_GAME_OVER
};

class Game : public IGame
{
	Q_OBJECT

protected:
	State state;
	int playerCount;
	IPlayer ** player;

	QWidget * widget;
public:
	Game(QWidget * _widget);
	virtual ~Game(void);
	virtual IGameState * GetCurrentState() const = 0;
	virtual bool PlayerTurn() = 0; /// Game specific turn
	virtual void NextTurn();       /// Common for all games
};

#endif
