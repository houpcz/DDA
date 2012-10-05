#include "Board.h"
#include <qpainter.h>

Board::Board(QWidget *parent, IGame * _activeGame) : QWidget(parent)
{
	activeGame = _activeGame;
	this->resize(500, 500);
	setMouseTracking(true);
}


Board::~Board(void)
{
}

void Board::paintEvent(QPaintEvent * paintEvent)
{
	QPainter painter(this);
	activeGame->Draw(&painter);
}

void Board::mouseMoveEvent ( QMouseEvent * event ) 
{
	activeGame->MouseMoveEvent(event->x(), event->y());
	repaint();
}

void Board::mousePressEvent ( QMouseEvent * event )
{
	activeGame->MousePressEvent(event->x(), event->y());
}