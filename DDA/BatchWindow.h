#ifndef _BATCH_WINDOW_H_
#define _BATCH_WINDOW_H_

#include <vector>
#include <qwidget.h>
#include <QPushButton>
#include <QLCDNumber>
#include <QSpinBox>
#include <QComboBox>
#include <QTreeWidget>
#include <QLabel>
#include <QProgressBar>
#include <QBasicTimer>
#include <QElapsedTimer>
#include "BatchThread.h"
#include "IGame.h"
#include "BatchItem.h"
#include "IEnvironmentAI.h"

using namespace std;

class BatchWindow : public QWidget
{
    Q_OBJECT

private :
	static const int MAX_THREAD = 32;
	int sizeThread;
	BatchThread *batchThread[MAX_THREAD];
	QLCDNumber *gameIDNumber;
	int batchProgress;
	int progressValue;
	int threadRunning;
	QBasicTimer basicTimer;
	QElapsedTimer elapsedTime;
	QLabel * progressTimeLabel;
	QLabel * elapsedTimeLabel;
	QProgressBar *progressBar;
	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *addBatch;
	QPushButton *removeBatch;
	QPushButton *setupBatch;
	QPushButton *diagramBatch;
	QPushButton *saveBatchToCsv;
	QPushButton *saveAllToCsv;
	QTreeWidget *listBatch;
	QTreeWidget *playerStatsTree;
	QComboBox * gameBox;
	QComboBox * aggrFnc;
	QSpinBox * batchSize;
	QSpinBox * sizeThreadBox;
	QLabel * sizeThreadLabel;

	bool batchIsRunning;
	double timePerItem;
	int currentBatchItemID;
	vector<BatchItem *> batchItem;
	vector<IGame *> gameList;
	vector<IEnvironmentAI *> environmentAIList;
	vector<IPlayer *> playerAIList;

	void DisassemblyTimeInSeconds(int seconds, int *outS, int *outMin, int *outHour);
public:
	BatchWindow(vector<IGame *> _gameList, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, QWidget *parent);
	~BatchWindow(void);

	void NextBatchItem();
	void timerEvent(QTimerEvent *event);
public slots:
	void StartBatch();
	void StopBatch();
	void SetupBatch();
	void SaveBatchToCsv();
	void SaveAllToCsv();
	void RemoveTopItem();
	void AddItemToBatch();
	void ItemSelect();
	void OpenDiagramWindow();
	void BatchOver();
	void GameOver(int gameID);
	void AggrFnc(int fnc);
	void ThreadChange(int threads);
};

#endif

