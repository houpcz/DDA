#ifndef _DDAWIDGET_H_
#define _DDSWIDGET_H_

#include <qwidget.h>
#include <qmainwindow.h>
#include "IGame.h"
#include "GameMaze.h"
#include "MenschArgere.h"
#include "Board.h"
#include <qmenu.h>
#include <QSignalMapper>
class DDAWidget :
	public QMainWindow
{
Q_OBJECT

private:
	static const int GAME_MAZE_ID = 0;
	static const int GAME_MENSCH_ARGERE_ID = 1;

	IGame * activeGame;
	int activeGameID;
	Board * board;

	QMenu * playersMenu;
    QSignalMapper * signalMapper;


	void ChangePlayerMenu();
public:
	DDAWidget(QWidget *parent = 0);
	~DDAWidget(void);
	void paintEvent(QPaintEvent * paintEvent);
	void mouseMoveEvent ( QMouseEvent * event );
	void mousePressEvent ( QMouseEvent * event );

	public slots:
		void NewGame();
		void SetGame(int gameID);
		void SetGameMaze();
		void SetGameMenschArgere();
		void ChangePlayer(int player);
};

#endif


