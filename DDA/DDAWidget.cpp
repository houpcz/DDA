#include "DDAWidget.h"
#include <QMenuBar>
#include <qpainter.h>
#include <QHBoxLayout>
#include <qtime>
#include <stdlib.h>
#include <time.h>

#include "Human.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"

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
	game->addAction(startGameAction);

	QMenu * setGameMenu = game->addMenu(tr("Set Game"));
	QAction * setGameMazeAction = new QAction(tr("Maze"), this);
    QAction * setGameMenschArgereAction = new QAction(tr("Mensch Argere"), this);
	QAction * setGameLostCitiesAction = new QAction(tr("Lost cities"), this);
	setGameMenu->addAction(setGameMazeAction);
	setGameMenu->addAction(setGameMenschArgereAction);
	setGameMenu->addAction(setGameLostCitiesAction);
    connect(setGameMazeAction, SIGNAL(triggered()), this, SLOT(SetGameMaze()));
	connect(setGameMenschArgereAction, SIGNAL(triggered()), this, SLOT(SetGameMenschArgere()));
	connect(setGameLostCitiesAction, SIGNAL(triggered()), this, SLOT(SetGameLostCities()));

	QAction * batchMenuAction = new QAction(tr("&Batch"), this);
    batchMenuAction->setShortcut(tr("Ctrl+B"));
    batchMenuAction->setStatusTip(tr("Start batch of games"));
    connect(batchMenuAction, SIGNAL(triggered()), this, SLOT(BatchMenu()));
	game->addAction(batchMenuAction);

	playersMenu = menuBar()->addMenu(tr("Players"));

	signalMapper = new QSignalMapper(this);

	activeGameID = GAME_MENSCH_ARGERE_ID;
	activeGame = new LostCities(this);
	board = new Board(this, activeGame);
	setCentralWidget(board);

	// set all players
	playerAI.push_back(new Human(1));
	playerAI.push_back(new PlayerRandomAI(2));
	playerAI.push_back(new PlayerHillClimber(3));

	SetGame(GAME_LOST_CITIES_ID);

	srand (time(NULL));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;

	delete signalMapper;

	for(int loop1 = 0; loop1 < playerAI.size(); loop1++)
	{
		delete playerAI[loop1];
	}
	playerAI.clear();
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
	repaint();
}

void DDAWidget::NewGame()
{
	//for(int loop1 = 0; loop1 < 10000; loop1++)
	activeGame->StartGame();
	repaint();
}

void DDAWidget::BatchMenu()
{
	batchWindow = new BatchWindow(this);
	setCentralWidget(batchWindow);
}

void DDAWidget::SetGameLostCities()
{
	SetGame(GAME_LOST_CITIES_ID);
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
	board = new Board(this, activeGame);
	setCentralWidget(board);

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
		case GAME_LOST_CITIES_ID :
			activeGame = new LostCities(this);
			break;
	}

	ChangePlayerMenu();

	board->SetGame(activeGame);
	board->repaint();
}

void DDAWidget::ChangePlayerMenu()
{
	playersMenu->clear();
	signalMapper->disconnect();

	QAction * setPlayer;
	playerMenu[0] = playersMenu->addMenu(tr("Environmental AI"));
	int environmentalAINumber = 1;
	for(int loop1 = 0; loop1 < activeGame->GetMaxPlayerAI(); loop1++)
	{
		playerMenu[loop1 + environmentalAINumber] = playersMenu->addMenu(activeGame->GetPlayer(loop1 + environmentalAINumber)->GetAINAme());
		
		for(int loop2 = 0; loop2 < playerAI.size(); loop2++)
		{
			if(loop1 > 0 && loop2 == 0)
				continue;

			setPlayer = new QAction(playerAI[loop2]->GetAINAme(), this);
			connect(setPlayer, SIGNAL(triggered()), signalMapper, SLOT(map()));
			int signalInt = loop2 + (loop1 + environmentalAINumber) * playerAI.size();
			signalMapper->setMapping(setPlayer, signalInt);
			playerMenu[loop1 + environmentalAINumber]->addAction(setPlayer);
		}
	}

	connect(signalMapper, SIGNAL(mapped(int)),
             this, SLOT(ChangePlayer(int)));
}

void DDAWidget::ChangePlayer(int player)
{
	int playerID = player / playerAI.size();
	int aiID = player % playerAI.size();
	
	activeGame->SetPlayer(playerID, aiID);
	playerMenu[playerID]->setTitle(playerAI[aiID]->GetAINAme());
}