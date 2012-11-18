#include "BatchThread.h"


BatchThread::BatchThread()
{
	shouldRun = false;
}


BatchThread::~BatchThread(void)
{
}

bool BatchThread::Start(IGame * _game, int _batchSize)
{
	game = _game;
	batchSize = _batchSize;

	if(!shouldRun)
	{
		while(isRunning())
			;

		start();
		return true;
	} else {
		return false;
	}
}

void BatchThread::run() {
	shouldRun = true;
	for(int loop1 = 0; loop1 < batchSize && shouldRun; loop1++)
	{
		game->StartGame();
		emit GameOver(loop1 + 1);
	}

	shouldRun = false;
	emit BatchItemOver();
}