#include "BatchItem.h"
#include <QMessageBox>

BatchItem::BatchItem(int _batchSize, IGame * _game, QTreeWidgetItem * _treeWidgetItem)
{
	batchSize = _batchSize;
	game = _game;
	treeWidgetItem = _treeWidgetItem;
	allGameStat = new GameStat*[batchSize];
	for(int loop1 = 0; loop1 < batchSize; loop1++)
	{
		allGameStat[loop1] = new GameStat(_game->GetPlayerCount());
	}
	sumGameStat = new GameStat(_game->GetPlayerCount());
}


BatchItem::~BatchItem(void)
{
	for(int loop1 = 0; loop1 < batchSize; loop1++)
	{
		delete allGameStat[loop1];
	}
	delete [] allGameStat;
	delete game;
	delete sumGameStat;
}

QString BatchItem::GetName()
{
	QString playerNames = "";

	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
	{
		playerNames += "," + game->GetPlayer(loop1)->GetAIName() + QString::number(game->GetPlayer(loop1)->Level());
	}

	return game->GetGameName() + QString::number(batchSize) + playerNames;
}
void BatchItem::SetSumGameStat(GameStat _sumGameStat) 
{ 
	*sumGameStat = _sumGameStat; 
}

void BatchItem::ExportToCsv(QString path)
{
	QLocale czeck(QLocale::Czech);	

	QFile * file = new QFile(path);
	if (!file->open(QIODevice::WriteOnly)) {
        return;
    }


	QString temp = QString("Turn Number;Leader Switches;SSD;ESD");
	for(int loop2 = 0; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "W";
	}
	for(int loop2 = 0; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "MinC";
	}
	for(int loop2 = 0; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "MaxC";
	}
	for(int loop2 = 0; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "AvgC";
	}
	for(int loop2 = 0; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "Turn";
	}
	temp+= allGameStat[0]->GameStatHeader();
	temp+= "\n";

	file->write(temp.toAscii());
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	for(int loop1 = 0; loop1 < realBatchSize; loop1++)
	{
		temp = QString::number(allGameStat[loop1]->TurnNumberReal()) + ";";
		temp += QString::number(allGameStat[loop1]->LeaderSwitches()) + ";";
		temp += czeck.toString(allGameStat[loop1]->SumScoreDifference() / (float) allGameStat[loop1]->TurnNumberReal(), 'f') + ";";
		temp += QString::number(allGameStat[loop1]->EndScoreDifference()) + ";";
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerWinner(loop2)) + ";";
		}
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerChoisesMin(loop2)) + ";";
		}
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerChoisesMax(loop2)) + ";";
		}
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += czeck.toString(allGameStat[loop1]->PlayerChoisesSum(loop2) / (float) allGameStat[loop1]->PlayerTurnNumber(loop2)) + ";";
		}
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerTurnNumber(loop2)) + ";";
		}
		temp = temp.simplified();
		temp = temp.replace(" ", "");
		temp += allGameStat[loop1]->GameStatRecord();
		temp += "\n";  
		file->write(temp.toAscii());
	}
	file->flush();
	file->close();

	delete file;
}

void BatchItem::UpdateTreeWidget() 
{ 
	float realBatchSize = (float) treeWidgetItem->data(2, 0).toInt();
	float avgTurnNumberReal = sumGameStat->TurnNumberReal() / (float) realBatchSize;
	treeWidgetItem->setData(3, 0, avgTurnNumberReal);
	treeWidgetItem->setData(4, 0, sumGameStat->PlayerWinner(1) / (float) realBatchSize);
	treeWidgetItem->setData(5, 0, sumGameStat->LeaderSwitches() / (float) realBatchSize);
	treeWidgetItem->setData(6, 0, sumGameStat->SumScoreDifference() / (float) realBatchSize / avgTurnNumberReal);
	treeWidgetItem->setData(7, 0, sumGameStat->EndScoreDifference() / (float) realBatchSize);
}

void BatchItem::UpdatePlayerTreeWidget(QTreeWidget * playerTree)
{
	float realBatchSize = (float) treeWidgetItem->data(2, 0).toInt();
	if(realBatchSize < 0.5)
		return;

	for(int loop1 = 0; loop1 < sumGameStat->NumberPlayers(); loop1++)
	{
		QTreeWidgetItem * playerItem = playerTree->topLevelItem(loop1);
		float avgTurnNumberReal = sumGameStat->PlayerTurnNumber(loop1) / realBatchSize;
		playerItem->setData(2, 0, sumGameStat->PlayerWinner(loop1) / realBatchSize);
		playerItem->setData(3, 0, (sumGameStat->PlayerChoisesSum(loop1) / realBatchSize) / avgTurnNumberReal);
		playerItem->setData(4, 0, sumGameStat->PlayerChoisesMin(loop1) / realBatchSize);
		playerItem->setData(5, 0, sumGameStat->PlayerChoisesMax(loop1) / realBatchSize);
		playerItem->setData(6, 0, avgTurnNumberReal);
	}
}
