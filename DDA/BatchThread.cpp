#include "BatchThread.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <QGlobal.h>
#include <QTime>

BatchThread::BatchThread()
{
	shouldRun = false;
	sumGameStat = NULL;
}


BatchThread::~BatchThread(void)
{
}

bool BatchThread::Start(BatchItem * _batchItem)
{
	batchItem = _batchItem;
	game = batchItem->Game();
	batchSize = batchItem->BatchSize();
	
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
	QTime time = QTime::currentTime();
	srand((uint)time.msec() + (int) batchItem->TreeWidgetItem());
	qsrand((uint)time.msec() + (int) batchItem->TreeWidgetItem());

	sumGameStat = new GameStat(game->GetPlayerCount() - 1);
	for(int loop1 = 0; loop1 < game->GetPlayerCount() - 1; loop1++)
		sumGameStat->UpdatePlayerChoises(loop1, 0);

	shouldRun = true;
	for(int loop1 = 0; loop1 < batchSize && shouldRun; loop1++)
	{
		game->StartGame();
		(*sumGameStat) = (*sumGameStat) + game->GetGameStat();
		emit GameOver(loop1 + 1);
	}

	batchItem->SetSumGameStat(*sumGameStat);
	delete sumGameStat;

	shouldRun = false;
	emit BatchItemOver();
}