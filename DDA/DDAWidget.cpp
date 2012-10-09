#include "DDAWidget.h"
#include <QMenuBar>
#include <qpainter.h>
#include <QHBoxLayout>
#include <qtime>
#include <stdlib.h>
#include <time.h>

DDAWidget::DDAWidget(QWidget *parent) : QMainWindow(parent)
{
	resize(500, 500);
	setPalette(QPalette(QColor(250, 250, 200)));
	setMouseTracking(false);

	setWindowTitle(QString::fromUtf8("Dynamic difficulty adjustement"));

	QMenu * game = menuBar()->addMenu(tr("Game"));	
	QAction * startGameAction = new QAction(tr("&New"), this);
    startGameAction->setShortcut(tr("Ctrl+S"));
    startGameAction->setStatusTip(tr("Start new game"));
    connect(startGameAction, SIGNAL(triggered()), this, SLOT(NewGame()));

	QMenu * setGameMenu = game->addMenu(tr("Set Game"));
	QAction * setGameMazeAction = new QAction(tr("Maze"), this);
    QAction * setGameMenschArgereAction = new QAction(tr("Mensch Argere"), this);
	setGameMenu->addAction(setGameMazeAction);
	setGameMenu->addAction(setGameMenschArgereAction);
    connect(setGameMazeAction, SIGNAL(triggered()), this, SLOT(SetGameMaze()));
	connect(setGameMenschArgereAction, SIGNAL(triggered()), this, SLOT(SetGameMenschArgere()));

	game->addAction(startGameAction);

	//activeGame->StartGame();

	activeGameID = GAME_MENSCH_ARGERE_ID;
	activeGame = new MenschArgere(this);
	board = new Board(this, activeGame);
	setCentralWidget(board);

	srand (time(NULL));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;

	delete board;
}

void DDAWidget::paintEvent(QPaintEvent * paintEvent)
{
	 
}

void DDAWidget::mouseMoveEvent ( QMouseEvent * event ) 
{
	activeGame->MouseMoveEvent(event->x(), event->y());
	repaint();
}

void DDAWidget::mousePressEvent ( QMouseEvent * event )
{
	activeGame->MousePressEvent(event->x(), event->y());
}

void DDAWidget::NewGame()
{
	activeGame->StartGame();
}

void DDAWidget::SetGameMaze()
{
	SetGame(GAME_MAZE_ID);
}

void DDAWidget::SetGameMenschArgere()
{
	SetGame(GAME_MENSCH_ARGERE_ID);
}

void DDAWidget::SetGame(int gameID)
{
	if(gameID == activeGameID)
		return;

	delete activeGame;
	activeGameID = gameID;
	

	switch(gameID)
	{
		case GAME_MENSCH_ARGERE_ID :
			activeGame = new MenschArgere(this);
			break;
		case GAME_MAZE_ID :
			activeGame = new GameMaze(this);
			break;
	}

	board->SetGame(activeGame);
}