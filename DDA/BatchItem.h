#ifndef _BATCH_ITEM_
#define _BATCH_ITEM_

#include <QTreeWidgetItem>
#include "IGame.h"
#include "GameStat.h"

class BatchItem
{
private:
	int batchSize;
	IGame * game;
	QTreeWidgetItem * treeWidgetItem;
	GameStat * sumGameStat;
	GameStat ** allGameStat;
public:
	BatchItem(int _batchSize, IGame * _game, QTreeWidgetItem * _treeWidgetItem);
	~BatchItem(void);
	IGame * Game() { return game; };
	int BatchSize() { return batchSize; };
	QTreeWidgetItem * TreeWidgetItem() { return treeWidgetItem;};
	void UpdateTreeWidget();
	void SetSumGameStat(GameStat _sumGameStat);
	void SetGameStat(GameStat _gameStat, int gameID) { *allGameStat[gameID] = _gameStat; };
	QString GetName();
	void ExportToCsv(QString path);
};

#endif

