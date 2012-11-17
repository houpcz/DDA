#include "BatchThread.h"


BatchThread::BatchThread(IGame * _game, int _batchSize)
{
	game = _game;
	batchSize = _batchSize;
}


BatchThread::~BatchThread(void)
{
}

void BatchThread::run() {
	for(int loop1 = 0; loop1 < batchSize; loop1++)
	{
		game->StartGame();
		emit GameOver(loop1);
	}
}