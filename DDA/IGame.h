#ifndef _IGAME_H_
#define _IGAME_H_

#include <qpainter.h>
#include <QMouseEvent>
#include <qobject.h>
#include "IGameState.h"

class IGame : public QObject
{
Q_OBJECT

public:
	IGame() {};
	virtual ~IGame() {};
	virtual void NextTurn() = 0;
	virtual bool PlayerTurn() = 0;
	virtual void StartGame() = 0;
	virtual IGameState * GetCurrentState() const = 0;
	virtual void Draw(QPainter * painter, int tickMillis = 0) = 0;
	virtual void MouseMoveEvent ( int xMouse, int yMouse ) {};
	virtual void MousePressEvent ( int xMouse, int yMouse ) {};

	virtual void SetPlayer(int playerID, int aiID) = 0;
	virtual int GetMinEnvironmentalAI() = 0;
	virtual int GetMaxEnvironmentalAI() = 0;
	virtual int GetMinPlayerAI() = 0;
	virtual int GetMaxPlayerAI() = 0;
public slots:
	void PlayerIsReady();
};

#endif
