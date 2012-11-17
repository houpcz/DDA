#include <QGridLayout>
#include "BatchWindow.h"
#include "LostCities.h"
#include "MenschArgere.h"
#include "GameMaze.h"

BatchWindow::BatchWindow(QWidget *parent) : QWidget(parent)
{
	 QGridLayout *gridLayout = new QGridLayout;
	 startButton = new QPushButton(tr("Start"));
	 connect(startButton, SIGNAL(clicked()), this, SLOT(StartBatch()));
	 stopButton = new QPushButton(tr("Stop"));
	 connect(stopButton, SIGNAL(clicked()), this, SLOT(StopBatch()));

	 gameIDNumber = new QLCDNumber(this);
	 gridLayout->addWidget(gameIDNumber, 0, 1);
	 gridLayout->addWidget(startButton, 1, 0);
	 gridLayout->addWidget(stopButton, 1, 1);
     setLayout(gridLayout);

	 activeGame = NULL;
	 batchThread = new BatchThread();
	 connect(batchThread, SIGNAL(GameOver(int)), gameIDNumber, SLOT(display(int)), Qt::QueuedConnection);
}


BatchWindow::~BatchWindow(void)
{
	batchThread->Stop();
}

void BatchWindow::StartBatch()
{
	if(batchThread->isRunning())
		return;

	if(activeGame != NULL)
	{
		delete activeGame;
		activeGame = NULL;
	}
	activeGame = new LostCities(this, false);
	batchThread->Start(activeGame, 1000);
}

void BatchWindow::StopBatch()
{
	batchThread->Stop();
}
