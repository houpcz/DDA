#include <QGridLayout>
#include <QFileDialog>
#include "BatchWindow.h"
#include "BatchGameSetup.h"
#include "LostCities.h"
#include "Ludo.h"
#include "GameMaze.h"

BatchWindow::BatchWindow(vector<IGame *> _gameList, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, QWidget *parent) : QWidget(parent)
{
	 batchThread = new BatchThread();
	 batchIsRunning = false;
	 playerAIList = _playerAIList;
	 environmentAIList = _environmentAIList;
	 gameList = _gameList;

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

	 gameBox = new QComboBox(this);
	 for(int loop1 = 0; loop1 < gameList.size(); loop1++)
		 gameBox->addItem(gameList[loop1]->GetGameName());

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
	 bool ok = connect(saveAllToCsv, SIGNAL(clicked()), this, SLOT(SaveAllToCsv()));

	 playerStatsTree = new QTreeWidget(this);
	 playerStatsTree->setColumnCount(7);
	 QTreeWidgetItem * headerPlayer = new QTreeWidgetItem();
	 headerPlayer->setData(0, 0, "Player"); 
	 headerPlayer->setData(1, 0, "Level"); 
	 headerPlayer->setData(2, 0, "Winner"); 
	 headerPlayer->setData(3, 0, "Ch. Avg"); 
	 headerPlayer->setData(4, 0, "Ch. Min"); 
	 headerPlayer->setData(5, 0, "Ch. Max"); 
	 headerPlayer->setData(6, 0, "Turn"); 
	 for(int loop1 = 0; loop1 < playerStatsTree->columnCount(); loop1++)
		 playerStatsTree->resizeColumnToContents(loop1);
	 playerStatsTree->setHeaderItem(headerPlayer);
	 playerStatsTree->setRootIsDecorated(false);
	 playerStatsTree->setMaximumHeight(120);
	 listBatch = new QTreeWidget(this);
	 listBatch->setColumnCount(8);
	 QTreeWidgetItem * header = new QTreeWidgetItem();
	 header->setData(0, 0, "Game name"); 
	 header->setData(1, 0, "Batch"); 
	 header->setData(2, 0, "Compl."); 
	 header->setData(3, 0, "Turn N."); 
	 header->setData(4, 0, "P1 winner"); 
	 header->setData(5, 0, "Leader S."); 
	 header->setData(6, 0, "Avg Diff"); 
	 header->setData(7, 0, "End Diff"); 
	 listBatch->setHeaderItem(header);
	 listBatch->setRootIsDecorated(false);
	 for(int loop1 = 0; loop1 < listBatch->columnCount(); loop1++)
		 listBatch->resizeColumnToContents(loop1);
	 connect(listBatch, SIGNAL(itemSelectionChanged()), this, SLOT(ItemSelect()));
	 connect(listBatch, SIGNAL(itemClicked (QTreeWidgetItem*,int)), this, SLOT(ItemSelect()));

	 gridLayout->addWidget(gameBox, 0, 0);
	 gridLayout->addWidget(batchSize, 0, 1);
	 gridLayout->addWidget(addBatch, 0, 2);
	 gridLayout->addWidget(removeBatch, 1, 0);
	 gridLayout->addWidget(setupBatch, 1, 1);
	 gridLayout->addWidget(saveBatchToCsv, 1, 2);
	 gridLayout->addWidget(listBatch, 2, 0, 1, 4);
	 gridLayout->addWidget(playerStatsTree, 3, 0, 1, 4);
	 gridLayout->addWidget(startButton, 4, 0);
	 gridLayout->addWidget(stopButton, 4, 1);
	 gridLayout->addWidget(saveAllToCsv, 4, 2);
	 gridLayout->addWidget(progressBar, 4, 3);
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
		BatchGameSetup * setup = new BatchGameSetup(batchItem[currentID]->Game(), environmentAIList, playerAIList, false, this);
		setup->exec();
		listBatch->setCurrentItem(NULL);
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
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Save to CSV"), "");

	for(int loop1 = 0; loop1 < batchItem.size(); loop1++)
	{
		batchItem[loop1]->ExportToCsv(dirName + "\\" + QString::number(loop1) + batchItem[loop1]->GetName() + ".csv");
	}
}

void BatchWindow::ItemSelect()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID < 0 || currentID >= batchItem.size())
		return;

	playerStatsTree->clear();

	IGame * game = batchItem[currentID]->Game();
	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
	{
		IPlayer * player = game->GetPlayer(loop1);
		QTreeWidgetItem * tempItem;
		tempItem = new QTreeWidgetItem();
		tempItem->setData(0, 0, player->GetAIName());
		if(player->IsScalable())
			tempItem->setData(1, 0, player->Level());
		else
			tempItem->setData(1, 0, "NS");
		playerStatsTree->addTopLevelItem(tempItem);
	}

	batchItem[currentID]->UpdatePlayerTreeWidget(playerStatsTree);

	for(int loop1 = 0; loop1 < playerStatsTree->columnCount(); loop1++)
		playerStatsTree->resizeColumnToContents(loop1);
}

void BatchWindow::AddItemToBatch()
{
	QTreeWidgetItem * tempItem;
	tempItem = new QTreeWidgetItem();
	tempItem->setData(0, 0, gameBox->currentText());
	tempItem->setData(1, 0, batchSize->value());
	for(int loop1 = 2; loop1 < listBatch->columnCount(); loop1++)
	{
		tempItem->setData(loop1, 0, 0);
	}
	listBatch->addTopLevelItem(tempItem);

	batchItem.push_back(new BatchItem(batchSize->value(), gameList[gameBox->currentIndex()]->Factory(this, false), tempItem));	
}
