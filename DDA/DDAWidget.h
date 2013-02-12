#ifndef _DDAWIDGET_H_
#define _DDSWIDGET_H_

#include <qmenu.h>
#include <QSignalMapper>
#include <vector>

#include <qwidget.h>
#include <qmainwindow.h>
#include "BatchWindow.h"
#include "IGame.h"
#include "IEnvironmentAI.h"
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
	IGame * activeGame;
	int activeGameID;
	Board * board;
	BatchWindow * batchWindow;

	QMenu * playersMenu;
    QSignalMapper * signalMapper;
	QSignalMapper * environmentMapper;
	QSignalMapper * gameMapper;

	QMenu * playerMenu[5];

	vector<IGame *> gameList;
	vector<IEnvironmentAI *> environmentAIList;
	vector<IPlayer *> playerAIList;

	void MakePluginLists();
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
		void ChangePlayer(int player);
		void ChangeEnvironmentPlayer(int player);
		void SetupGame();
		void ShowFlowView();
		void ShowLevelView();
};

#endif


