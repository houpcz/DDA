#include <QGridLayout>
#include <QFileDialog>
#include "BatchWindow.h"
#include "BatchGameSetup.h"
#include "BatchDiagrams.h"
#include "LostCities.h"
#include "Ludo.h"
#include "GameMaze.h"
#include <ctime>

BatchWindow::BatchWindow(vector<IGame *> _gameList, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, QWidget *parent) : QWidget(parent)
{
	 for(int loop1 = 0; loop1 < MAX_THREAD; loop1++)
		batchThread[loop1] = new BatchThread();
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
	 progressBar->setTextVisible(false);
	 progressBar->setMinimum(0);
	 //gameIDNumber = new QLCDNumber(this);
	 //connect(batchThread, SIGNAL(GameOver(int)), gameIDNumber, SLOT(display(int)), Qt::QueuedConnection);

	 for(int loop1 = 0; loop1 < MAX_THREAD; loop1++)
	 {
		connect(batchThread[loop1], SIGNAL(GameOver(int)), this, SLOT(GameOver(int)), Qt::QueuedConnection);
		connect(batchThread[loop1], SIGNAL(BatchItemOver()), this, SLOT(BatchOver()), Qt::QueuedConnection);
	 }
	 sizeThread = 4;
	 sizeThreadLabel = new QLabel(QString::fromLocal8Bit("Vl�ken"));
	 sizeThreadBox = new QSpinBox(this);
	 sizeThreadBox->setMinimum(1);	
	 sizeThreadBox->setMaximum(MAX_THREAD);
	 sizeThreadBox->setValue(sizeThread);
	 connect(sizeThreadBox, SIGNAL(valueChanged(int)), this, SLOT(ThreadChange(int)), Qt::QueuedConnection);


	 gameBox = new QComboBox(this);
	 for(int loop1 = 0; loop1 < gameList.size(); loop1++)
		 gameBox->addItem(gameList[loop1]->GetGameName());

	 batchSize = new QSpinBox(this);
	 batchSize->setMinimum(0);
	 batchSize->setMaximum(1000000);
	 batchSize->setValue(1000); 
	 addBatch = new QPushButton(QString::fromLocal8Bit("P�idej"), this);
	 connect(addBatch, SIGNAL(clicked()), this, SLOT(AddItemToBatch()));
	 removeBatch = new QPushButton(QString::fromLocal8Bit("Odstra�"), this);
	 connect(removeBatch, SIGNAL(clicked()), this, SLOT(RemoveTopItem()));
	 setupBatch = new QPushButton(QString::fromLocal8Bit("Nastaven�"), this);
	 connect(setupBatch, SIGNAL(clicked()), this, SLOT(SetupBatch()));
	 saveBatchToCsv = new QPushButton(QString::fromLocal8Bit("Ulo�"), this);
	 connect(saveBatchToCsv, SIGNAL(clicked()), this, SLOT(SaveBatchToCsv()));
	 saveAllToCsv = new QPushButton(QString::fromLocal8Bit("Ulo� v�e"), this);
	 connect(saveAllToCsv, SIGNAL(clicked()), this, SLOT(SaveAllToCsv()));
	 diagramBatch = new QPushButton(tr("Histogram"), this);
	 connect(diagramBatch, SIGNAL(clicked()), this, SLOT(OpenDiagramWindow()));

	 aggrFnc = new QComboBox(this);
	 aggrFnc->addItem(QString::fromLocal8Bit("Pr�m�r"));
	 aggrFnc->addItem(QString::fromLocal8Bit("Sm. odchylka"));
	 aggrFnc->addItem(QString::fromLocal8Bit("Medi�n"));
	 aggrFnc->addItem("Min");
	 aggrFnc->addItem("Max");
	 bool ok = connect(aggrFnc, SIGNAL(activated(int)), this, SLOT(AggrFnc(int)));

	 playerStatsTree = new QTreeWidget(this);
	 playerStatsTree->setColumnCount(8);
	 QTreeWidgetItem * headerPlayer = new QTreeWidgetItem();
	 headerPlayer->setData(0, 0, QString::fromLocal8Bit("Hr��")); 
	 headerPlayer->setData(1, 0, QString::fromLocal8Bit("�rove�")); 
	 headerPlayer->setData(2, 0, QString::fromLocal8Bit("V�t�z")); 
	 headerPlayer->setData(3, 0, QString::fromLocal8Bit("T. pr�.")); 
	 headerPlayer->setData(4, 0, QString::fromLocal8Bit("T. min.")); 
	 headerPlayer->setData(5, 0, QString::fromLocal8Bit("T. max.")); 
	 headerPlayer->setData(6, 0, QString::fromLocal8Bit("Tah�")); 
	 //headerPlayer->setData(7, 0, "Justice"); 
	 headerPlayer->setData(7, 0, QString::fromLocal8Bit("Veden�")); 
	 for(int loop1 = 0; loop1 < playerStatsTree->columnCount(); loop1++)
	 	 playerStatsTree->resizeColumnToContents(loop1);
	 playerStatsTree->setMinimumHeight(150);
	 playerStatsTree->setHeaderItem(headerPlayer);
	 playerStatsTree->setRootIsDecorated(false);
	 playerStatsTree->setMaximumHeight(150);
	 listBatch = new QTreeWidget(this);
	 listBatch->setColumnCount(12);
	 QTreeWidgetItem * header = new QTreeWidgetItem();
	 header->setData(0, 0, QString::fromLocal8Bit("Jm�no hry")); 
	 header->setData(1, 0, QString::fromLocal8Bit("D�vka")); 
	 header->setData(2, 0, QString::fromLocal8Bit("Hotovo")); 
	 header->setData(3, 0, QString::fromLocal8Bit("Tah�")); 
	 header->setData(4, 0, QString::fromLocal8Bit("H1 v�t�z")); 
	 header->setData(5, 0, QString::fromLocal8Bit("Pr. v�t.")); 
	 header->setData(6, 0, QString::fromLocal8Bit("Nap�t�")); 
	 header->setData(7, 0, QString::fromLocal8Bit("N�skok")); 
	 header->setData(8, 0, QString::fromLocal8Bit("Sprav.")); 
	 header->setData(9, 0, QString::fromLocal8Bit("Uv��it.")); 
	 header->setData(10, 0, QString::fromLocal8Bit("N�hod.")); 
	 header->setData(11, 0, QString::fromLocal8Bit("Doba v.")); 
	 listBatch->setHeaderItem(header);
	 listBatch->setRootIsDecorated(false);
	 for(int loop1 = 0; loop1 < listBatch->columnCount(); loop1++)
		 listBatch->resizeColumnToContents(loop1);
	 connect(listBatch, SIGNAL(itemSelectionChanged()), this, SLOT(ItemSelect()));
	 connect(listBatch, SIGNAL(itemClicked (QTreeWidgetItem*,int)), this, SLOT(ItemSelect()));
	 threadRunning = 0;

	 timePerItem = 1.0;
	 elapsedTimeLabel = new QLabel("");
	 progressTimeLabel = new QLabel("");
	 progressTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	 elapsedTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	 gridLayout->addWidget(gameBox, 0, 0);
	 gridLayout->addWidget(batchSize, 0, 1);
	 gridLayout->addWidget(addBatch, 0, 2);
	 gridLayout->addWidget(sizeThreadLabel, 0, 3);
	 gridLayout->addWidget(sizeThreadBox, 0, 4);
	 gridLayout->addWidget(removeBatch, 1, 0);
	 gridLayout->addWidget(setupBatch, 1, 1);
	 gridLayout->addWidget(saveBatchToCsv, 1, 2);
	 gridLayout->addWidget(aggrFnc, 1, 3);
	 gridLayout->addWidget(diagramBatch, 1, 4);
	 gridLayout->addWidget(listBatch, 2, 0, 1, 6);
	 gridLayout->addWidget(playerStatsTree, 3, 0, 1, 6);
	 gridLayout->addWidget(startButton, 4, 0);
	 gridLayout->addWidget(stopButton, 4, 1);
	 gridLayout->addWidget(saveAllToCsv, 4, 2);
	 gridLayout->addWidget(progressBar, 4, 3, 1, 2);
	 gridLayout->addWidget(elapsedTimeLabel, 4, 5, 1, 1);
	 gridLayout->addWidget(progressTimeLabel, 4, 3, 1, 1);
     setLayout(gridLayout);
}


BatchWindow::~BatchWindow(void)
{
	for(int loop1 = 0; loop1 < MAX_THREAD; loop1++)
		batchThread[loop1]->Stop();
}

void BatchWindow::BatchOver()
{
	threadRunning--;
	NextBatchItem();
}
void BatchWindow::NextBatchItem()
{
	if(currentBatchItemID >= 0)
	{
		if(threadRunning > 0)
		{
			return;
		}

		time_t rawtime;
		struct tm * ti;
		char buffer [256];
		time (&rawtime);
		ti = localtime (&rawtime);
		sprintf (buffer,"%04d-%02d-%02d_%02d-%02d_", ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min);

		if(batchProgress == batchItem[currentBatchItemID]->BatchSize())
		{
			batchItem[currentBatchItemID]->ExportToCsv("temp\\" + QString(buffer) + QString::number(currentBatchItemID) + "-" + batchItem[currentBatchItemID]->GetName() + ".csv");
		}
		batchItem[currentBatchItemID]->UpdateTreeWidget((BatchItem::EAggrFnc) aggrFnc->currentIndex());
	}

	currentBatchItemID++;
	if(currentBatchItemID < batchItem.size() && batchIsRunning)
	{
		batchProgress = 0;
		int batchSize = batchItem[currentBatchItemID]->BatchSize();
		int itemPerThread = batchSize / sizeThread;
		//if(itemPerThread * sizeThread != batchSize)
		//	itemPerThread += 1; 

		int minID = 0;
		for(int loop1 = 0; loop1 < sizeThread; loop1++)
		{
			int maxID = (loop1 + 1) * itemPerThread;
			if(loop1 == sizeThread - 1)
				maxID = batchSize;

			batchThread[loop1]->Start(batchItem[currentBatchItemID], minID, maxID);
			threadRunning++;
			minID = maxID;
		}
	} else {
		basicTimer.stop();
		removeBatch->setEnabled(true);
		addBatch->setEnabled(true);
		setupBatch->setEnabled(true);
		saveAllToCsv->setEnabled(true);
		saveBatchToCsv->setEnabled(true);
		aggrFnc->setEnabled(true);
		diagramBatch->setEnabled(true);
		sizeThreadBox->setEnabled(true);
		progressTimeLabel->setText(QString::fromLocal8Bit("Hotovo."));
		batchIsRunning = false;
	}

	ItemSelect();
}

void BatchWindow::GameOver(int gameID)
{
	batchProgress++;
	batchItem[currentBatchItemID]->TreeWidgetItem()->setData(2, 0, batchProgress);
	progressValue++;
	timePerItem = elapsedTime.elapsed() / (double) progressValue / 1000.0;
	progressBar->setValue(progressValue);


}
void BatchWindow::StartBatch()
{
	bool running = false;
	for(int loop1 = 0; loop1 < MAX_THREAD; loop1++)
		running = running || batchThread[loop1]->isRunning();

	if(running || batchItem.empty())
		return;

	basicTimer.start(1000, this);
	elapsedTime.start();

	int sumBatchSize = 0;
	for(int loop1 = 0; loop1 < batchItem.size(); loop1++)
	{
		sumBatchSize+=batchItem[loop1]->BatchSize();
		batchItem[loop1]->TreeWidgetItem()->setData(2, 0, 0);
		batchItem[loop1]->ClearBatch();
	}
	progressBar->setMaximum(sumBatchSize);

	currentBatchItemID = -1;
	progressValue = 0;
	batchProgress = 0;
	batchIsRunning = true;
	removeBatch->setEnabled(false);
	addBatch->setEnabled(false);
	setupBatch->setEnabled(false);
	saveAllToCsv->setEnabled(false);
	saveBatchToCsv->setEnabled(false);
	aggrFnc->setEnabled(false);
	diagramBatch->setEnabled(false);
	sizeThreadBox->setEnabled(false);
	NextBatchItem();
}

void BatchWindow::StopBatch()
{
	if(batchIsRunning)
		progressTimeLabel->setText(QString::fromLocal8Bit("  Zastaveno, po�kejte pros�m."));
	batchIsRunning = false;

	basicTimer.stop();
	for(int loop1 = 0; loop1 < MAX_THREAD; loop1++)
		batchThread[loop1]->Stop();
}

void BatchWindow::RemoveTopItem()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID >= 0)
	{
		delete batchItem[currentID];
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
		QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Ulo�it jako CSV"), "", tr("CSV (*.csv);;Textov� soubory (*.txt)"));
		batchItem[currentID]->ExportToCsv(fileName);
	}
}

void BatchWindow::SaveAllToCsv()
{
	QString dirName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("Ulo�it jako CSV"), "");

	for(int loop1 = 0; loop1 < batchItem.size(); loop1++)
	{
		batchItem[loop1]->ExportToCsv(dirName + "\\" + QString::number(loop1) + batchItem[loop1]->GetName() + ".csv");
	}
}

void BatchWindow::ItemSelect()
{
	int currentID = listBatch->currentIndex().row();
	if(batchIsRunning && currentBatchItemID == currentID)
		return;

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
	
	batchItem[currentID]->UpdatePlayerTreeWidget(playerStatsTree, (BatchItem::EAggrFnc) aggrFnc->currentIndex());

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

void BatchWindow::AggrFnc(int fnc)
{
	for(int loop1 = 0; loop1 < batchItem.size(); loop1++)
	{
		batchItem[loop1]->UpdateTreeWidget((BatchItem::EAggrFnc) fnc);
	}

	int currentID = listBatch->currentIndex().row();
	batchItem[currentID]->UpdatePlayerTreeWidget(playerStatsTree, (BatchItem::EAggrFnc) aggrFnc->currentIndex());

	for(int loop1 = 0; loop1 < playerStatsTree->columnCount(); loop1++)
		playerStatsTree->resizeColumnToContents(loop1);
}

void BatchWindow::OpenDiagramWindow()
{
	int currentID = listBatch->currentIndex().row();
	if(currentID >= 0)
	{
		BatchDiagrams *diagramWindow = new BatchDiagrams(batchItem[currentID], currentID);
		diagramWindow->show();
	}
}

void BatchWindow::DisassemblyTimeInSeconds(int seconds, int *outS, int *outMin, int *outHour)
{
	int minute = seconds / 60;
	
	*outHour = minute / 60;
	*outMin = minute % 60;
	*outS = seconds % 60;
}

void BatchWindow::ThreadChange(int threads)
{
	sizeThread = threads;
}

void BatchWindow::timerEvent(QTimerEvent *event)
{
	int batchSum = progressBar->maximum();

	int timeElapsed = (int) (elapsedTime.elapsed() / 1000.0);
	int timeToEnd = (int) (timePerItem * batchSum - timeElapsed);
	
	int h, m, s;
	QString printString;
	if(progressBar->maximum() == progressValue || progressValue == 0)
	{
		progressTimeLabel->setText(" ");
	} else {
		DisassemblyTimeInSeconds(timeToEnd, &s, &m, &h);
		if(h > 0)
			printString += QString::number(h) + QString(" h ");
		if(m > 0 || printString.length() > 0)
			printString += QString::number(m) + QString(" m ");
		if(s > 0 || printString.length() > 0)
			progressTimeLabel->setText(printString + QString::number(s) + " s");
	}

	printString = "";
	DisassemblyTimeInSeconds(timeElapsed, &s, &m, &h);
	if(h > 0)
		printString += QString::number(h) + QString(" h ");
	if(m > 0 || printString.length() > 0)
		printString += QString::number(m) + QString(" m ");
	elapsedTimeLabel->setText(printString + QString::number(s) + " s");
}