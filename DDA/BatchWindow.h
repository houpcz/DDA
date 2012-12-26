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
	QTreeWidget *listBatch;
	QComboBox * gameList;
	QSpinBox * batchSize;

	bool batchIsRunning;
	int currentBatchItemID;
	vector<BatchItem *> batchItem;
	vector<IPlayer *> playerAI;
public:
	BatchWindow(vector<IPlayer *> _playerAI, QWidget *parent);
	~BatchWindow(void);
public slots:
	void StartBatch();
	void StopBatch();
	void SetupBatch();
	void RemoveTopItem();
	void AddItemToBatch();
	void NextBatchItem();
	void GameOver(int gameID);
};

#endif

