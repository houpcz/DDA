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
	enum EStatName {
		STAT_TURN_NUMBER,
	};

	enum EAggrFnc {
		AGGR_MEAN,
		AGGR_DEVIATION,
		AGGR_MEDIAN,
		AGGR_MIN,
		AGGR_MAX, 
	};

	BatchItem(int _batchSize, IGame * _game, QTreeWidgetItem * _treeWidgetItem);
	~BatchItem(void);
	IGame * Game() { return game; };
	int BatchSize() { return batchSize; };
	QTreeWidgetItem * TreeWidgetItem() { return treeWidgetItem;};
	void UpdateTreeWidget(EAggrFnc fnc);
	void UpdatePlayerTreeWidget(QTreeWidget * playerTree);
	void SetSumGameStat(GameStat _sumGameStat);
	void SetGameStat(GameStat _gameStat, int gameID) { *allGameStat[gameID] = _gameStat; };
	QString GetName(char splitChar = ',');
	void ExportToCsv(QString path);
	vector<float> GetStatAsVector(int statName);
};

#endif

