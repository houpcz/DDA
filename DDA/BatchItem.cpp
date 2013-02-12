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

QString BatchItem::GetName(char splitChar)
{
	QString playerNames = "";

	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
	{
		playerNames += splitChar + game->GetPlayer(loop1)->GetAIName() + splitChar + QString::number(game->GetPlayer(loop1)->Level());
	}

	return game->GetGameName() + splitChar + QString::number(batchSize) + playerNames;
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

	pair<QString, QString> gameSetup = game->GetSetupString();
	QString setupHeader = QString("Game Name;Batch Size;EAI name;EAI level");
	for(int loop1 = 1; loop1 < sumGameStat->NumberPlayers(); loop1++)
		setupHeader += ";P" + QString::number(loop1) + ";Level";
	setupHeader += ";" + gameSetup.first + '\n';
	file->write(setupHeader.toAscii());
	file->write(GetName(';').toAscii() + ';' + gameSetup.second.toAscii() + '\n');
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
		temp += czeck.toString(allGameStat[loop1]->SumRankDifference() / (float) allGameStat[loop1]->TurnNumberReal(), 'f') + ";";
		temp += QString::number(allGameStat[loop1]->EndRankDifference()) + ";";
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

void BatchItem::UpdateTreeWidget(EAggrFnc fnc) 
{ 
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	float avgTurnNumberReal = sumGameStat->TurnNumberReal() / (float) realBatchSize;

	float turnNumber;
	float winnerPlayer1;
	float leaderSwitches;
	float gameRankDiff;
	float endRankDiff;
	
	vector<float> values;
	values.reserve(batchSize);

	float val;
	if(fnc == AGGR_MEAN || fnc == AGGR_DEVIATION)
	{
		turnNumber = avgTurnNumberReal;
		winnerPlayer1 = sumGameStat->PlayerWinner(1) / (float) realBatchSize;
		leaderSwitches = sumGameStat->LeaderSwitches() / (float) realBatchSize;
		gameRankDiff = sumGameStat->SumRankDifference() / (float) realBatchSize / avgTurnNumberReal;
		endRankDiff = sumGameStat->EndRankDifference() / (float) realBatchSize;

		if(fnc == AGGR_DEVIATION)
		{
			float sumVal;

			for(int loop2 = 0; loop2 < 5; loop2++)
			{
				sumVal = 0.0f;
				for(int loop1 = 0; loop1 < realBatchSize; loop1++)
				{
					switch(loop2)
					{
						case 0 : val = allGameStat[loop1]->TurnNumberReal() - turnNumber; break;
						case 1 : val = allGameStat[loop1]->PlayerWinner(1) - winnerPlayer1; break;
						case 2 : val = allGameStat[loop1]->LeaderSwitches() - leaderSwitches; break;
						case 3 : val = allGameStat[loop1]->SumRankDifference() / (float) allGameStat[loop1]->TurnNumber() - gameRankDiff; break;
						case 4 : val = allGameStat[loop1]->EndRankDifference() - endRankDiff; break;
					}
					sumVal += (val * val) / realBatchSize;
				}
				float deviation = sqrt(sumVal);

				switch(loop2)
				{
					case 0 : turnNumber = deviation; break;
					case 1 : winnerPlayer1 = deviation; break;
					case 2 : leaderSwitches = deviation; break;
					case 3 : gameRankDiff = deviation; break;
					case 4 : endRankDiff = deviation; break;
				}
			}
		}
	}
	else {
		for(int loop2 = 0; loop2 < 5; loop2++)
		{
			values = GetStatAsVector((EStatName) loop2);
			sort(values.begin(), values.end());

			switch(fnc)
			{
				case AGGR_MIN : val = values[0]; break;
				case AGGR_MAX : val = values.back(); break;
				case AGGR_MEDIAN : val = values[values.size() / 2]; break;
				default : val = 0.0f; break;
			}

			switch(loop2)
			{
				case 0 : turnNumber = val; break;
				case 1 : winnerPlayer1 = val; break;
				case 2 : leaderSwitches = val; break;
				case 3 : gameRankDiff = val; break;
				case 4 : endRankDiff = val; break;
			}
		}
	}
	

	treeWidgetItem->setData(3, 0, turnNumber);
	treeWidgetItem->setData(4, 0, winnerPlayer1);
	treeWidgetItem->setData(5, 0, leaderSwitches);
	treeWidgetItem->setData(6, 0, gameRankDiff);
	treeWidgetItem->setData(7, 0, endRankDiff);
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

vector<float> BatchItem::GetStatAsVector(int statName)
{
	vector<float> result;

	float val;
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	for(int loop1 = 0; loop1 < realBatchSize; loop1++)
	{
		switch(statName)
		{
			case 0 : val = allGameStat[loop1]->TurnNumberReal(); break;
			case 1 : val = allGameStat[loop1]->PlayerWinner(1); break;
			case 2 : val = allGameStat[loop1]->LeaderSwitches(); break;
			case 3 : val = allGameStat[loop1]->SumRankDifference() / (float) allGameStat[loop1]->TurnNumber(); break;
			case 4 : val = allGameStat[loop1]->EndRankDifference(); break;
		}

		result.push_back(val);
	}

	return result;
}
