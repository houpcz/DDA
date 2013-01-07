#ifndef _BATCH_WINDOW_H_
#define _BATCH_WINDOW_H_

#include <vector>
#include <qwidget.h>
#include <QPushButton>
#include <QLCDNumber>
#include <QSpinBox>
#include <QComboBox>
#include <QTreeWidget>
#include <QProgressBar>
#include "BatchThread.h"
#include "IGame.h"
#include "BatchItem.h"
#include "IEnvironmentAI.h"

using namespace std;

class BatchWindow : public QWidget
{
    Q_OBJECT

private :
	BatchThread *batchThread;
	QLCDNumber *gameIDNumber;
	int progressValue;
	QProgressBar *progressBar;
	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *addBatch;
	QPushButton *removeBatch;
	QPushButton *setupBatch;
	QPushButton *saveBatchToCsv;
	QPushButton *saveAllToCsv;
	QTreeWidget *listBatch;
	QTreeWidget *playerStatsTree;
	QComboBox * gameBox;
	QSpinBox * batchSize;

	bool batchIsRunning;
	int currentBatchItemID;
	vector<BatchItem *> batchItem;
	vector<IGame *> gameList;
	vector<IEnvironmentAI *> environmentAIList;
	vector<IPlayer *> playerAIList;
public:
	BatchWindow(vector<IGame *> _gameList, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, QWidget *parent);
	~BatchWindow(void);
public slots:
	void StartBatch();
	void StopBatch();
	void SetupBatch();
	void SaveBatchToCsv();
	void SaveAllToCsv();
	void RemoveTopItem();
	void AddItemToBatch();
	void NextBatchItem();
	void ItemSelect();
	void GameOver(int gameID);
};

#endif

