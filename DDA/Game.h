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
	bool paint;

	State state;
	int playerCount;
	IPlayer ** player;

	int minEnvironmentalAI;
	int maxEnvironmentalAI;
	int minPlayerAI;
	int maxPlayerAI;

	QWidget * widget;
public:
	Game(QWidget * _widget, bool paint = true);
	virtual ~Game(void);
	virtual IGameState * GetCurrentState() const = 0;
	virtual bool PlayerTurn() = 0; /// Game specific turn
	virtual void NextTurn();       /// Common for all games
	void SetPlayer(int playerID, int aiID);
	virtual void Draw(QPainter * painter, int tickMillis = 0) = 0;
	virtual void Paint(QPainter * painter);

	IPlayer * GetPlayer(int ID) { return player[ID]; };
	int GetMinEnvironmentalAI() { return minEnvironmentalAI; };
	int GetMaxEnvironmentalAI() { return maxEnvironmentalAI; };
	int GetMinPlayerAI() { return minPlayerAI; };
	int GetMaxPlayerAI() { return maxPlayerAI; };
};

#endif
