#include "DDAWidget.h"
#include <qpainter.h>
#include <qtime>
DDAWidget::DDAWidget(QWidget *parent) : QWidget(parent)
{
	resize(500, 500);
	setPalette(QPalette(QColor(250, 250, 200)));
	setMouseTracking(true);
	activeGame = new GameMaze();
	activeGame->StartGame();

	QTime midnight(0, 0, 0);
	qsrand(midnight.secsTo(QTime::currentTime()));
}


DDAWidget::~DDAWidget(void)
{
	if(activeGame)
		delete activeGame;
}

void DDAWidget::paintEvent(QPaintEvent * paintEvent)
{
	 QPainter painter(this);
	 activeGame->Draw(&painter);
}

void DDAWidget::mouseMoveEvent ( QMouseEvent * event ) 
{
	activeGame->MouseMoveEvent(event);
	repaint();
}

void DDAWidget::mousePressEvent ( QMouseEvent * event )
{
	activeGame->MousePressEvent(event);
}

