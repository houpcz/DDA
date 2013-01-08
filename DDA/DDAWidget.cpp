#include "DDAWidget.h"
#include <QMenuBar>
#include <qpainter.h>
#include <QHBoxLayout>
#include <qtime>
#include <stdlib.h>
#include <time.h>

#include "BatchGameSetup.h" 
#include "Human.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"
#include "MiniMaxPlayer.h"
#include "EnvironmentAIBasic.h"

DDAWidget::DDAWidget(QWidget *parent) : QMainWindow(parent)
{
	resize(500, 500);
	setPalette(QPalette(QColor(250, 250, 200)));
	setMouseTracking(false);

	setWindowTitle(QString::fromUtf8("Dynamic difficulty adjustement"));

	MakePluginLists();

	QMenu * game = menuBar()->addMenu(tr("Game"));	
	QAction * startGameAction = new QAction(tr("&New"), this);
    startGameAction->setShortcut(tr("N"));
    startGameAction->setStatusTip(tr("Start new game"));
    connect(startGameAction, SIGNAL(triggered()), this, SLOT(NewGame()));
	game->addAction(startGameAction);

	QMenu * setGameMenu = game->addMenu(tr("Change Game"));

	gameMapper = new QSignalMapper(this);
	for(int loop1 = 0; loop1 < gameList.size(); loop1++)
	{
		QAction * setGame = new QAction(gameList[loop1]->GetGameName(), this);
		connect(setGame, SIGNAL(triggered()), gameMapper, SLOT(map()));
		gameMapper->setMapping(setGame, loop1);
		setGameMenu->addAction(setGame);
	}
	connect(gameMapper, SIGNAL(mapped(int)),
             this, SLOT(SetGame(int)));

	QAction * setupMenuAction = new QAction(tr("&Setup"), this);
	setupMenuAction->setShortcut(tr("S"));
	connect(setupMenuAction, SIGNAL(triggered()), this, SLOT(SetupGame()));
	game->addAction(setupMenuAction);

	QAction * batchMenuAction = new QAction(tr("&Batch"), this);
    batchMenuAction->setShortcut(tr("B"));
    batchMenuAction->setStatusTip(tr("Start batch of games"));
    connect(batchMenuAction, SIGNAL(triggered()), this, SLOT(BatchMenu()));
	game->addAction(batchMenuAction);

	playersMenu = menuBar()->addMenu(tr("Players"));

	signalMapper = new QSignalMapper(this);
	environmentMapper = new QSignalMapper(this);

	activeGameID = -1;
	activeGame = new GameMaze(this);
	board = new Board(this, activeGame);
	setCentralWidget(board);

	SetGame(0);

	srand (time(NULL));
}

void DDAWidget::MakePluginLists()
{
	// set all players
	playerAIList.push_back(new Human(1));
	playerAIList.push_back(new PlayerRandomAI(2));
	playerAIList.push_back(new PlayerHillClimber(3));
	playerAIList.push_back(new MiniMaxPlayer(4));

	environmentAIList.push_back(new EnvironmentAIBasic(0));

	// set all games
	gameList.push_back(new GameMaze(this));
	gameList.push_back(new LostCities(this));
	gameList.push_back(new Ludo(this));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;

	delete gameMapper;
	delete signalMapper;
	delete environmentMapper;

	for(int loop1 = 0; loop1 < playerAIList.size(); loop1++)
	{
		delete playerAIList[loop1];
	}
	playerAIList.clear();

	for(int loop1 = 0; loop1 < gameList.size(); loop1++)
	{
		delete gameList[loop1];
	}
	gameList.clear();

	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{
		delete environmentAIList[loop1];
	}
	gameList.clear();
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
	if(centralWidget() != board)
	{
		board = new Board(this, activeGame);
		setCentralWidget(board);
		repaint();
	} else {
		activeGame->StartGame();
		repaint();
	}
}

void DDAWidget::BatchMenu()
{
	batchWindow = new BatchWindow(gameList, environmentAIList, playerAIList, this);
	setCentralWidget(batchWindow);
}

void DDAWidget::SetGame(int gameID)
{
	board = new Board(this, activeGame);
	setCentralWidget(board);

	if(gameID == activeGameID)
		return;

	delete activeGame;
	activeGameID = gameID;
	activeGame = gameList[gameID]->Factory(this, true);

	ChangePlayerMenu();

	board->SetGame(activeGame);
	activeGame->StartGame();
	board->repaint();
}

void DDAWidget::ChangePlayerMenu()
{
	playersMenu->clear();
	signalMapper->disconnect();
	environmentMapper->disconnect();

	QAction * setPlayer;
	playerMenu[0] = playersMenu->addMenu(activeGame->GetPlayer(0)->GetAIName());
	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{
		setPlayer = new QAction(environmentAIList[loop1]->GetAIName(), this);
		connect(setPlayer, SIGNAL(triggered()), environmentMapper, SLOT(map()));
		environmentMapper->setMapping(setPlayer, loop1);
		playerMenu[0]->addAction(setPlayer);
	}

	int environmentalAINumber = 1;
	for(int loop1 = 0; loop1 < activeGame->GetMaxPlayerAI(); loop1++)
	{
		playerMenu[loop1 + environmentalAINumber] = playersMenu->addMenu(activeGame->GetPlayer(loop1 + environmentalAINumber)->GetAIName());
		
		for(int loop2 = 0; loop2 < playerAIList.size(); loop2++)
		{
			if(loop1 > 0 && loop2 == 0)
				continue;

			setPlayer = new QAction(playerAIList[loop2]->GetAIName(), this);
			connect(setPlayer, SIGNAL(triggered()), signalMapper, SLOT(map()));
			int signalInt = loop2 + (loop1 + environmentalAINumber) * playerAIList.size();
			signalMapper->setMapping(setPlayer, signalInt);
			playerMenu[loop1 + environmentalAINumber]->addAction(setPlayer);
		}
	}

	connect(environmentMapper, SIGNAL(mapped(int)),
             this, SLOT(ChangeEnvironmentPlayer(int)));
	
	connect(signalMapper, SIGNAL(mapped(int)),
             this, SLOT(ChangePlayer(int)));
}

void DDAWidget::ChangePlayer(int player)
{
	int playerID = player / playerAIList.size();
	int aiID = player % playerAIList.size();
	
	activeGame->SetPlayer(playerID, playerAIList[aiID]);
	playerMenu[playerID]->setTitle(playerAIList[aiID]->GetAIName());
}

void DDAWidget::ChangeEnvironmentPlayer(int player)
{
	activeGame->SetPlayer(0, environmentAIList[player]);
	playerMenu[0]->setTitle(environmentAIList[player]->GetAIName());
}

void DDAWidget::SetupGame()
{
	BatchGameSetup * setup = new BatchGameSetup(activeGame, environmentAIList, playerAIList, true, this);
	setup->exec();
}