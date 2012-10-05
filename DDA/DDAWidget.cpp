#include "DDAWidget.h"
#include <QMenuBar>
#include <qpainter.h>
#include <qtime>
#include <stdlib.h>
#include <time.h>

DDAWidget::DDAWidget(QWidget *parent) : QMainWindow(parent)
{
	resize(500, 500);
	setPalette(QPalette(QColor(250, 250, 200)));
	setMouseTracking(true);
	activeGame = new GameMaze(this);

	setWindowTitle(QString::fromUtf8("Dynamic difficulty adjustement"));

	QMenu * game = menuBar()->addMenu(tr("Game"));	
	QAction * startGameAction = new QAction(tr("&New"), this);
    startGameAction->setShortcut(tr("Ctrl+S"));
    startGameAction->setStatusTip(tr("Start new game"));
    connect(startGameAction, SIGNAL(triggered()), this, SLOT(NewGame()));
	game->addAction(startGameAction);

	activeGame->StartGame();

	srand (time(NULL));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;
}

void DDAWidget::paintEvent(QPaintEvent * paintEvent)
{
	 QPainter painter(this);

	 //painter.setWindow(0, menuBar()->height(), painter.window().width(), painter.window().height());
	 painter.setViewport(0, menuBar()->height(), painter.window().width(), painter.window().height() - menuBar()->height() / 2);
	 activeGame->Draw(&painter);
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
