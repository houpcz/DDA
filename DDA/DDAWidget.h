#ifndef _DDAWIDGET_H_
#define _DDSWIDGET_H_

#include <qwidget.h>
#include <qmainwindow.h>
#include "IGame.h"
#include "GameMaze.h"
#include "Board.h"

class DDAWidget :
	public QMainWindow
{
Q_OBJECT

private:
	IGame * activeGame;
	Board * board;
public:
	DDAWidget(QWidget *parent = 0);
	~DDAWidget(void);
	void paintEvent(QPaintEvent * paintEvent);
	void mouseMoveEvent ( QMouseEvent * event );
	void mousePressEvent ( QMouseEvent * event );

	public slots:
		void NewGame();
};

#endif


