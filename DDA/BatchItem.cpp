#include "BatchItem.h"


BatchItem::BatchItem(int _batchSize, IGame * _game, QTreeWidgetItem * _treeWidgetItem)
{
	batchSize = _batchSize;
	game = _game;
	treeWidgetItem = _treeWidgetItem;
	sumGameStat = new GameStat(_game->GetPlayerCount());
}


BatchItem::~BatchItem(void)
{
	delete game;
	delete sumGameStat;
}

void BatchItem::SetSumGameStat(GameStat _sumGameStat) 
{ 
	*sumGameStat = _sumGameStat; 
}

void BatchItem::UpdateTreeWidget() 
{ 
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	float avgTurnNumberReal = sumGameStat->TurnNumberReal() / (float) realBatchSize;
	treeWidgetItem->setData(3, 0, avgTurnNumberReal);
	treeWidgetItem->setData(4, 0, sumGameStat->PlayerWinner(0) / (float) realBatchSize);
	treeWidgetItem->setData(5, 0, sumGameStat->LeaderSwitches() / (float) realBatchSize);
	treeWidgetItem->setData(6, 0, sumGameStat->SumScoreDifference() / (float) realBatchSize / avgTurnNumberReal);
	treeWidgetItem->setData(7, 0, sumGameStat->EndScoreDifference() / (float) realBatchSize);
}
