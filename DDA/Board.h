#ifndef _BOARD_H_
#define _BOARD_H_

#include "IGame.h"
#include <qwidget.h>

class Board :
	public QWidget
{
private:
	IGame * activeGame;

public:
	Board(QWidget *parent, IGame * _activeGame);
	~Board(void);

	void paintEvent(QPaintEvent * paintEvent);
	void mouseMoveEvent ( QMouseEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void SetGame(IGame * _activeGame) {
		activeGame = _activeGame;
	}
};

#endif

