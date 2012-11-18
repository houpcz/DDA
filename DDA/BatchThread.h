#ifndef _BATCH_THREAD_H_
#define _BATCH_THREAD_H_

#include <QThread>
#include "IGame.h"

class BatchThread : public QThread {
    Q_OBJECT

private:
	IGame * game;
	int batchSize;
	bool shouldRun;
public:
	BatchThread();
	~BatchThread(void);
	void run();
	bool Start(IGame * _game, int _batchSize);
	void Stop() { shouldRun = false; };
	bool ShouldRun() { return shouldRun; }

signals:
	void GameOver(int gameID);
	void BatchItemOver();
};

#endif