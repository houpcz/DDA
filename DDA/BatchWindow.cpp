#include <QGridLayout>
#include "BatchWindow.h"
#include "LostCities.h"
#include "MenschArgere.h"
#include "GameMaze.h"

BatchWindow::BatchWindow(QWidget *parent) : QWidget(parent)
{
	 activeGame = NULL;
	 batchThread = new BatchThread();

	 QGridLayout *gridLayout = new QGridLayout;
	 startButton = new QPushButton(tr("Start"), this);
	 connect(startButton, SIGNAL(clicked()), this, SLOT(StartBatch()));
	 stopButton = new QPushButton(tr("Stop"), this);
	 connect(stopButton, SIGNAL(clicked()), this, SLOT(StopBatch()));
	 gameIDNumber = new QLCDNumber(this);
	 connect(batchThread, SIGNAL(GameOver(int)), gameIDNumber, SLOT(display(int)), Qt::QueuedConnection);

	 gameList = new QComboBox(this);
	 gameList->addItem(tr("Lost Cities"));
	 gameList->addItem(tr("Maze"));
	 gameList->addItem(tr("Mensch Argere"));
	 batchSize = new QSpinBox(this);
	 batchSize->setMinimum(0);
	 batchSize->setMaximum(1000000);
	 batchSize->setValue(1000); 
	 addBatch = new QPushButton(tr("Add"), this);
	 connect(addBatch, SIGNAL(clicked()), this, SLOT(AddItemToBatch()));
	 removeBatch = new QPushButton(tr("Remove"), this);
	 connect(removeBatch, SIGNAL(clicked()), this, SLOT(RemoveTopItem()));
	 setupBatch = new QPushButton(tr("Setup"), this);
	 listBatch = new QTreeWidget(this);
	 listBatch->setColumnCount(3);
	 QTreeWidgetItem * header = new QTreeWidgetItem();
	 header->setData(0, 0, "Game"); 
	 header->setData(1, 0, "Batch"); 
	 header->setData(2, 0, "Completed"); 
	 listBatch->setHeaderItem(header);

	 gridLayout->addWidget(gameList, 0, 0);
	 gridLayout->addWidget(batchSize, 0, 1);
	 gridLayout->addWidget(addBatch, 0, 2);
	 gridLayout->addWidget(removeBatch, 1, 0);
	 gridLayout->addWidget(setupBatch, 1, 1);
	 gridLayout->addWidget(listBatch, 2, 0, 1, 3);
	 gridLayout->addWidget(startButton, 3, 0);
	 gridLayout->addWidget(stopButton, 3, 1);
	 gridLayout->addWidget(gameIDNumber, 3, 2);
     setLayout(gridLayout);
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

void BatchWindow::RemoveTopItem()
{
	delete listBatch->currentItem();
}

void BatchWindow::AddItemToBatch()
{
	QTreeWidgetItem * tempItem;
	tempItem = new QTreeWidgetItem();
	tempItem->setData(0, 0, gameList->currentText());
	tempItem->setData(1, 0, batchSize->value());
	tempItem->setData(2, 0, 0);
	listBatch->addTopLevelItem(tempItem);
}
