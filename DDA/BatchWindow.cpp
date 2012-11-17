#include <QGridLayout>
#include "BatchWindow.h"
#include "LostCities.h"
#include "MenschArgere.h"

BatchWindow::BatchWindow(QWidget *parent) : QWidget(parent)
{
	 QGridLayout *gridLayout = new QGridLayout;
	 startButton = new QPushButton(tr("Start"));
	 connect(startButton, SIGNAL(clicked()), this, SLOT(StartBatch()));
	 gameIDNumber = new QLCDNumber(this);
	 gridLayout->addWidget(gameIDNumber, 0, 1);
	 gridLayout->addWidget(startButton, 1, 1);
     setLayout(gridLayout);

	 batchThread = NULL;
}


BatchWindow::~BatchWindow(void)
{
}

void BatchWindow::StartBatch()
{
	if(batchThread != NULL)
	{
	}
	activeGame = new MenschArgere(this, false);
	batchThread = new BatchThread(activeGame, 1000);
	connect(batchThread, SIGNAL(GameOver(int)), gameIDNumber, SLOT(display(int)), Qt::QueuedConnection);
	batchThread->start();
}
