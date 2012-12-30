#include <QGridLayout>
#include <QFileDialog>
#include "BatchWindow.h"
#include "BatchGameSetup.h"
#include "LostCities.h"
#include "Ludo.h"
#include "GameMaze.h"

BatchWindow::BatchWindow(vector<IPlayer *> _playerAI, QWidget *parent) : QWidget(parent)
{
	 batchThread = new BatchThread();
	 batchIsRunning = false;
	 playerAI = _playerAI;

	 QGridLayout *gridLayout = new QGridLayout;
	 startButton = new QPushButton(tr("Start"), this);
	 connect(startButton, SIGNAL(clicked()), this, SLOT(StartBatch()));
	 stopButton = new QPushButton(tr("Stop"), this);
	 connect(stopButton, SIGNAL(clicked()), this, SLOT(StopBatch()));
	 progressBar = new QProgressBar(this);
	 progressBar->setMinimum(0);
	 //gameIDNumber = new QLCDNumber(this);
	 //connect(batchThread, SIGNAL(GameOver(int)), gameIDNumber, SLOT(display(int)), Qt::QueuedConnection);
	 connect(batchThread, SIGNAL(GameOver(int)), this, SLOT(GameOver(int)), Qt::QueuedConnection);
	 connect(batchThread, SIGNAL(BatchItemOver()), this, SLOT(NextBatchItem()), Qt::QueuedConnection);

	 gameList = new QComboBox(this);
	 gameList->addItem(tr("Lost Cities"));
	 gameList->addItem(tr("Maze"));
	 gameList->addItem(tr("Ludo"));
	 batchSize = new QSpinBox(this);
	 batchSize->setMinimum(0);
	 batchSize->setMaximum(1000000);
	 batchSize->setValue(1000); 
	 addBatch = new QPushButton(tr("Add"), this);
	 connect(addBatch, SIGNAL(clicked()), this, SLOT(AddItemToBatch()));
	 removeBatch = new QPushButton(tr("Remove"), this);
	 connect(removeBatch, SIGNAL(clicked()), this, SLOT(RemoveTopItem()));
	 setupBatch = new QPushButton(tr("Setup"), this);
	 connect(setupBatch, SIGNAL(clicked()), this, SLOT(SetupBatch()));
	 saveBatchToCsv = new QPushButton(tr("Save"), this);
	 connect(saveBatchToCsv, SIGNAL(clicked()), this, SLOT(SaveBatchToCsv()));
	 saveAllToCsv = new QPushButton(tr("Save All"), this);
	 connect(saveAllToCsv, SIGNAL(clicked()), this, SLOT(SaveAllToCsv()));
	 listBatch = new QTreeWidget(this);
	 listBatch->setColumnCount(8);
	 QTreeWidgetItem * header = new QTreeWidgetItem();
	 header->setData(0, 0, "Game"); 
	 header->setData(1, 0, "Batch"); 
	 header->setData(2, 0, "Completed"); 
	 header->setData(3, 0, "Turn Number"); 
	 header->setData(4, 0, "P1 winner"); 
	 header->setData(5, 0, "LS"); 
	 header->setData(6, 0, "Avg Diff"); 
	 header->setData(7, 0, "End Diff"); 
	 listBatch->setHeaderItem(header);

	 gridLayout->addWidget(gameList, 0, 0);
	 gridLayout->addWidget(batchSize, 0, 1);
	 gridLayout->addWidget(addBatch, 0, 2);
	 gridLayout->addWidget(removeBatch, 1, 0);
	 gridLayout->addWidget(setupBatch, 1, 1);
	 gridLayout->addWidget(saveBatchToCsv, 1, 2);
	 gridLayout->addWidget(listBatch, 2, 0, 1, 4);
	 gridLayout->addWidget(startButton, 3, 0);
	 gridLayout->addWidget(stopButton, 3, 1);
	 gridLayout->addWidget(saveAllToCsv, 3, 2);
	 gridLayout->addWidget(progressBar, 3, 3);
     setLayout(gridLayout);
}


BatchWindow::~BatchWindow(void)
{
	batchThread->Stop();
}


void BatchWindow::NextBatchItem()
{
	if(currentBatchItemID >= 0)
		batchItem[currentBatchItemID]->UpdateTreeWidget();

	currentBatchItemID++;
	if(currentBatchItemID < batchItem.size() && batchIsRunning)
	{
		batchThread->Start(batchItem[currentBatchItemID]);
	} else {
		removeBatch->setEnabled(true);
		addBatch->setEnabled(true);
		setupBatch->setEnabled(true);
		saveAllToCsv->setEnabled(true);
		saveBatchToCsv->setEnabled(true);
		batchIsRunning = false;
	}
}

void BatchWindow::GameOver(int gameID)
{
	batchItem[currentBatchItemID]->TreeWidgetItem()->setData(2, 0, gameID);
	progressValue++;
	progressBar->setValue(progressValue);
}
void BatchWindow::StartBatch()
{
	if(batchThread->isRunning() || batchItem.empty())
		return;

	int sumBatchSize = 0;
	for(int loop1 = 0; loop1 < batchItem.size(); loop1++)
	{
		sumBatchSize+=batchItem[loop1]->BatchSize();
		batchItem[loop1]->TreeWidgetItem()->setData(2, 0, 0);
	}
	progressBar->setMaximum(sumBatchSize);

	currentBatchItemID = -1;
	progressValue = 0;
	batchIsRunning = true;
	removeBatch->setEnabled(false);
	addBatch->setEnabled(false);
	setupBatch->setEnabled(false);
	saveAllToCsv->setEnabled(false);
	saveBatchToCsv->setEnabled(false);
	NextBatchItem();
}

void BatchWindow::StopBatch()
{
	batchIsRunning = false;
	batchThread->Stop();
}

void BatchWindow::RemoveTopItem()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID >= 0)
	{
		batchItem.erase(batchItem.begin() + currentID, batchItem.begin() + currentID + 1);
		delete listBatch->currentItem();
	}
}

void BatchWindow::SetupBatch()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID >= 0)
	{
		BatchGameSetup * setup = new BatchGameSetup(batchItem[currentID]->Game(), playerAI, this);
		setup->exec();
	}
}
void BatchWindow::SaveBatchToCsv()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID >= 0)
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save to CSV"), "", tr("CSV (*.csv);;Text files (*.txt)"));
		batchItem[currentID]->ExportToCsv(fileName);
	}
}

void BatchWindow::SaveAllToCsv()
{
}

void BatchWindow::AddItemToBatch()
{
	QTreeWidgetItem * tempItem;
	tempItem = new QTreeWidgetItem();
	tempItem->setData(0, 0, gameList->currentText());
	tempItem->setData(1, 0, batchSize->value());
	for(int loop1 = 2; loop1 < listBatch->columnCount(); loop1++)
	{
		tempItem->setData(loop1, 0, 0);
	}
	listBatch->addTopLevelItem(tempItem);

	switch(gameList->currentIndex())
	{
		case 0 :
			batchItem.push_back(new BatchItem(batchSize->value(), new LostCities(this, false), tempItem));
			break;
		case 1 :
			batchItem.push_back(new BatchItem(batchSize->value(), new GameMaze(this, false), tempItem));
			break;
		case 2 :
			batchItem.push_back(new BatchItem(batchSize->value(), new Ludo(this, false), tempItem));
			break;
	}
	
}
