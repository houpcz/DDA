#ifndef _BATCH_THREAD_H_
#define _BATCH_THREAD_H_

#include <QThread>
#include "IGame.h"

class BatchThread : public QThread {
    Q_OBJECT

private:
	IGame * game;
	int batchSize;
public:
	BatchThread(IGame * _game, int _batchSize);
	~BatchThread(void);
	void run();

signals:
	void GameOver(int gameID);
};

#endif