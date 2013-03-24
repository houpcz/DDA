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
	void ClearBatch();
	void UpdateTreeWidget(EAggrFnc fnc);
	void UpdatePlayerTreeWidget(QTreeWidget * playerTree, EAggrFnc fnc);
	void SetSumGameStat(GameStat _sumGameStat);
	void SetGameStat(GameStat _gameStat, int gameID) { 
		if(allGameStat[gameID] == NULL)
			allGameStat[gameID] = new GameStat(game->GetPlayerCount());
		*allGameStat[gameID] = _gameStat; 
	};
	QString GetName(char splitChar = ',');
	void ExportToCsv(QString path);

	vector<float> GetStatAsVector(int statName);
	vector<float> GetPlayerStatAsVector(int statName, int playerID);
};

#endif

