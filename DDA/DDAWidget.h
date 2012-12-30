#ifndef _DDAWIDGET_H_
#define _DDSWIDGET_H_

#include <qmenu.h>
#include <QSignalMapper>
#include <vector>

#include <qwidget.h>
#include <qmainwindow.h>
#include "BatchWindow.h"
#include "IGame.h"
#include "GameMaze.h"
#include "Ludo.h"
#include "LostCities.h"
#include "Board.h"

using namespace std;

class DDAWidget :
	public QMainWindow
{
Q_OBJECT

private:
	static const int GAME_MAZE_ID = 0;
	static const int GAME_MENSCH_ARGERE_ID = 1;
	static const int GAME_LOST_CITIES_ID = 2;

	IGame * activeGame;
	int activeGameID;
	Board * board;
	BatchWindow * batchWindow;

	QMenu * playersMenu;
    QSignalMapper * signalMapper;

	QMenu * playerMenu[5];
	vector<IPlayer *> playerAI;

	void ChangePlayerMenu();
public:
	DDAWidget(QWidget *parent = 0);
	virtual ~DDAWidget(void);
	void paintEvent(QPaintEvent * paintEvent);
	void mouseMoveEvent ( QMouseEvent * event );
	void mousePressEvent ( QMouseEvent * event );

	public slots:
		void BatchMenu();
		void NewGame();
		void SetGame(int gameID);
		void SetGameMaze();
		void SetGameLudo();
		void SetGameLostCities();
		void ChangePlayer(int player);
};

#endif


