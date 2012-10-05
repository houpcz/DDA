#ifndef _IGAME_H_
#define _IGAME_H_

#include <qpainter.h>
#include <QMouseEvent>
#include <qobject.h>

class IGame : public QObject
{
Q_OBJECT

public:
	IGame() {};
	virtual ~IGame() {};
	virtual void NextTurn() = 0;
	virtual bool PlayerTurn() = 0;
	virtual void StartGame() = 0;
	virtual int * GetPlayerChoises() = 0;
	virtual void Draw(QPainter * painter, int tickMillis = 0) = 0;
	virtual void MouseMoveEvent ( int xMouse, int yMouse ) {};
	virtual void MousePressEvent ( int xMouse, int yMouse ) {};
public slots:
	void PlayerIsReady();
};

#endif
