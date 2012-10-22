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

	playersMenu = menuBar()->addMenu(tr("Players"));

	signalMapper = new QSignalMapper(this);

	activeGameID = GAME_MENSCH_ARGERE_ID;
	activeGame = new GameMaze(this);
	board = new Board(this, activeGame);
	setCentralWidget(board);

	SetGame(GAME_MAZE_ID);

	srand (time(NULL));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;

	delete board;
	delete signalMapper;
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

	ChangePlayerMenu();

	board->SetGame(activeGame);
	activeGame->StartGame();
}
#define MAX_PLAYER_ID 2

void DDAWidget::ChangePlayerMenu()
{
	playersMenu->clear();
	signalMapper->disconnect();

	QMenu * playerMenu;
	QAction * setPlayer;
	for(int loop1 = 0; loop1 < activeGame->GetMaxEnvironmentalAI(); loop1++)
	{
		playerMenu = playersMenu->addMenu(tr("Environmental AI"));
	}
	for(int loop1 = 0; loop1 < activeGame->GetMaxPlayerAI(); loop1++)
	{
		playerMenu = playersMenu->addMenu(tr("Player AI"));
		
		for(int loop2 = 0; loop2 < MAX_PLAYER_ID; loop2++)
		{
			switch(loop2)
			{
				case 0 :
					setPlayer = new QAction(tr("Human"), this);
					break;
				case 1 :
					setPlayer = new QAction(tr("AI random"), this);
					break;
			}
			connect(setPlayer, SIGNAL(triggered()), signalMapper, SLOT(map()));
			int signalInt = loop2 + (loop1 + activeGame->GetMaxEnvironmentalAI()) * MAX_PLAYER_ID;
			signalMapper->setMapping(setPlayer, signalInt);
			playerMenu->addAction(setPlayer);
		}
	}

	connect(signalMapper, SIGNAL(mapped(int)),
             this, SLOT(ChangePlayer(int)));
}

void DDAWidget::ChangePlayer(int player)
{
	int playerID = player / MAX_PLAYER_ID;
	int aiID = player % MAX_PLAYER_ID;
	
	activeGame->SetPlayer(playerID, aiID);
}