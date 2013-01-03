#include "Board.h"
#include <qpainter.h>
#include <QTime>

Board::Board(QWidget *parent, IGame * _activeGame) : QWidget(parent)
{
	activeGame = _activeGame;

	QTime time = QTime::currentTime();
	srand((uint)time.msec());
	qsrand((uint)time.msec());

	this->resize(500, 500);
	setMouseTracking(true);
}


Board::~Board(void)
{
}

void Board::paintEvent(QPaintEvent * paintEvent)
{
	QPainter painter(this);
	activeGame->Paint(&painter);
}

void Board::mouseMoveEvent ( QMouseEvent * event ) 
{
	activeGame->MouseMoveEvent(event->x(), event->y());
	repaint();
}

void Board::mousePressEvent ( QMouseEvent * event )
{
	activeGame->MousePressEvent(event->x(), event->y());
	repaint();
}