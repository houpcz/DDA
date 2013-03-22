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

bool BatchThread::Start(BatchItem * _batchItem, int _firstID, int _lastID)
{
	batchItem = _batchItem;
	IGame * g = batchItem->Game();
	game = g->Factory(NULL, false);
	for(int loop1 = 0; loop1 < g->GetPlayerCount(); loop1++)
		game->SetPlayer(loop1, g->GetPlayer(loop1)->Factory(loop1));

	batchSize = batchItem->BatchSize();
	firstID = _firstID;
	lastID = _lastID;
	
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

	//sumGameStat = new GameStat(game->GetPlayerCount());
	//for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
	//	sumGameStat->UpdatePlayerChoises(loop1, 0);

	shouldRun = true;
	GameStat * gameStat = new GameStat(game->GetPlayerCount());
	for(int loop1 = firstID; loop1 < lastID && shouldRun; loop1++)
	{
		game->StartGame();
		*gameStat = game->GetGameStat();
		//(*sumGameStat) = (*sumGameStat) + *gameStat;
		batchItem->SetGameStat(*gameStat, loop1);
		emit GameOver(loop1 + 1);
	}

	//batchItem->SetSumGameStat(*sumGameStat);
	//delete sumGameStat;

	shouldRun = false;
	emit BatchItemOver();
}