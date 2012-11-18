#ifndef _BATCH_THREAD_H_
#define _BATCH_THREAD_H_

#include <QThread>
#include "IGame.h"
#include "GameStat.h"
#include "BatchItem.h"

class BatchThread : public QThread {
    Q_OBJECT

private:
	GameStat * sumGameStat;
	IGame * game;
	BatchItem * batchItem;
	int batchSize;
	bool shouldRun;
public:
	BatchThread();
	~BatchThread(void);
	void run();
	bool Start(BatchItem * _batchItem);
	void Stop() { shouldRun = false; };
	bool ShouldRun() { return shouldRun; }

signals:
	void GameOver(int gameID);
	void BatchItemOver();
};

#endif