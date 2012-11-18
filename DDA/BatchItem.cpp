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
	treeWidgetItem->setData(3, 0, sumGameStat->TurnNumberReal() / (float) realBatchSize);
}
