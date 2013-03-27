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
		allGameStat[loop1] = NULL;
	}
	sumGameStat = new GameStat(_game->GetPlayerCount());
	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
		sumGameStat->UpdatePlayerChoises(loop1, 0);
}

void BatchItem::ClearBatch()
{
	for(int loop1 = 0; loop1 < batchSize; loop1++)
	{
		if(allGameStat[loop1] != NULL)
		{
			delete allGameStat[loop1];
			allGameStat[loop1] = NULL;
		}
	}
}


BatchItem::~BatchItem(void)
{
	for(int loop1 = 0; loop1 < batchSize; loop1++)
	{
		if(allGameStat[loop1] != NULL)
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
	QLocale czech(QLocale::Czech);	

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
	QString temp = QString("Turn Number;Leader Switches;SSD;ESD;J;C;LT");
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
	for(int loop2 = 1; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "Justice";
	}
	for(int loop2 = 1; loop2 < allGameStat[0]->NumberPlayers(); loop2++)
	{
		temp+= ";P" + QString::number(loop2+1) + "LT";
	}
	temp+= allGameStat[0]->GameStatHeader();
	temp+= "\n";

	file->write(temp.toAscii());
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	for(int loop1 = 0; loop1 < realBatchSize; loop1++)
	{
		temp = QString::number(allGameStat[loop1]->TurnNumberReal()) + ";";
		temp += QString::number(allGameStat[loop1]->LeaderSwitches()) + ";";
		temp += czech.toString(allGameStat[loop1]->SumRankDifference() / (float) allGameStat[loop1]->TurnNumberReal(), 'f') + ";";
		temp += QString::number(allGameStat[loop1]->EndRankDifference()) + ";";
		temp += czech.toString(allGameStat[loop1]->Justice()) + ";";
		temp += czech.toString(allGameStat[loop1]->Control()) + ";";
		temp += czech.toString(allGameStat[loop1]->LeaderTime()) + ";";
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
			temp += czech.toString(allGameStat[loop1]->PlayerChoisesSum(loop2) / (float) allGameStat[loop1]->PlayerTurnNumber(loop2)) + ";";
		}
		for(int loop2 = 0; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerTurnNumber(loop2)) + ";";
		}
		for(int loop2 = 1; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerDeltaH(loop2)) + ";";
		}
		for(int loop2 = 1; loop2 < allGameStat[loop1]->NumberPlayers(); loop2++)
		{
			temp += QString::number(allGameStat[loop1]->PlayerLeaderTime(loop2)) + ";";
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
	delete sumGameStat;
	sumGameStat = new GameStat(game->GetPlayerCount());
	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
		sumGameStat->UpdatePlayerChoises(loop1, 0);

	GameStat * tempStat;
	int left = 0;
	int right = batchSize-1;

	while(left< right)
	{
		while(allGameStat[left] != NULL && left < right)
			left++;

		while(allGameStat[right] == NULL && left < right)
			right--;

		if(left >= right)
			break;

		tempStat = allGameStat[left];
		allGameStat[left] = allGameStat[right];
		allGameStat[right] = tempStat;
		left++;
		right--;
	}

	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	for(int loop1 = 0; loop1 < realBatchSize; loop1++)
	{
		if(allGameStat[loop1] != NULL)
			(*sumGameStat) = (*sumGameStat) + *allGameStat[loop1];
	}

	float avgTurnNumberReal = sumGameStat->TurnNumberReal() / (float) realBatchSize;

	float turnNumber;
	float winnerPlayer1;
	float leaderSwitches;
	float gameRankDiff;
	float endRankDiff;
	float justice;
	float control;
	float leaderTime;

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
		justice = sumGameStat->Justice() / (float) realBatchSize;
		control = sumGameStat->Control() / (float) realBatchSize;
		leaderTime = sumGameStat->LeaderTime() / (float) realBatchSize;

		if(fnc == AGGR_DEVIATION)
		{
			float sumVal;

			for(int loop2 = 0; loop2 < 8; loop2++)
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
						case 5 : val = allGameStat[loop1]->Justice() - justice; break;
						case 6 : val = allGameStat[loop1]->Control() - control; break;
						case 7 : val = allGameStat[loop1]->LeaderTime() - leaderTime; break;
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
					case 5 : justice = deviation; break;
					case 6 : control = deviation; break;
					case 7 : leaderTime = deviation; break;
				}
			}
		}
	}
	else {
		for(int loop2 = 0; loop2 < 8; loop2++)
		{
			values = GetStatAsVector(loop2);
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
				case 5 : justice = val; break;
				case 6 : control = val; break;
				case 7 : leaderTime = val; break;
			}
		}
	}
	

	treeWidgetItem->setData(3, 0, turnNumber);
	treeWidgetItem->setData(4, 0, winnerPlayer1);
	treeWidgetItem->setData(5, 0, leaderSwitches);
	treeWidgetItem->setData(6, 0, gameRankDiff);
	treeWidgetItem->setData(7, 0, endRankDiff);
	treeWidgetItem->setData(8, 0, justice);
	treeWidgetItem->setData(9, 0, control);
	treeWidgetItem->setData(10, 0, leaderTime);
}

void BatchItem::UpdatePlayerTreeWidget(QTreeWidget * playerTree, EAggrFnc fnc)
{
	int player0turn = sumGameStat->TurnNumber() - sumGameStat->TurnNumberReal();
	float realBatchSize = (float) treeWidgetItem->data(2, 0).toInt();
	if(realBatchSize < 0.5)
		return;

	for(int loop3 = 0; loop3 < sumGameStat->NumberPlayers(); loop3++)
	{
		QTreeWidgetItem * playerItem = playerTree->topLevelItem(loop3);
		float avgTurnNumberReal = sumGameStat->PlayerTurnNumber(loop3) / realBatchSize;
		float turnNumber;
		float winner;
		float choisesAvg;
		float choisesMin;
		float choisesMax;
		float justice;
		float leaderTime;

		vector<float> values;
		values.reserve(batchSize);

		float val;
		if(fnc == AGGR_MEAN || fnc == AGGR_DEVIATION)
		{
			turnNumber = avgTurnNumberReal;
			winner = sumGameStat->PlayerWinner(loop3) / realBatchSize;
			choisesAvg = (sumGameStat->PlayerChoisesSum(loop3) / realBatchSize) / avgTurnNumberReal;
			choisesMin = sumGameStat->PlayerChoisesMin(loop3) / realBatchSize;
			choisesMax = sumGameStat->PlayerChoisesMax(loop3) / realBatchSize;
			justice = sumGameStat->PlayerDeltaH(loop3) / (float) player0turn / realBatchSize;
			leaderTime = sumGameStat->PlayerLeaderTime(loop3) / realBatchSize;

			if(fnc == AGGR_DEVIATION)
			{
				float sumVal;

				for(int loop2 = 0; loop2 < 7; loop2++)
				{
					sumVal = 0.0f;
					for(int loop1 = 0; loop1 < realBatchSize; loop1++)
					{
						switch(loop2)
						{
							case 0 : val = allGameStat[loop1]->PlayerTurnNumber(loop3) - turnNumber; break;
							case 1 : val = allGameStat[loop1]->PlayerWinner(loop3) - winner; break;
							case 2 : val = (allGameStat[loop1]->PlayerChoisesSum(loop3) / (float) allGameStat[loop1]->PlayerTurnNumber(loop3)) - choisesAvg; break;
							case 3 : val = allGameStat[loop1]->PlayerChoisesMin(loop3) - choisesMin; break;
							case 4 : val = allGameStat[loop1]->PlayerChoisesMax(loop3) - choisesMax; break;
							case 5 : val = allGameStat[loop1]->PlayerDeltaH(loop3) / (float) player0turn - justice; break;
							case 6 : val = allGameStat[loop1]->PlayerLeaderTime(loop3) - leaderTime; break;
						}
						sumVal += (val * val) / realBatchSize;
					}
					float deviation = sqrt(sumVal);

					switch(loop2)
					{
						case 0 : turnNumber = deviation; break;
						case 1 : winner = deviation; break;
						case 2 : choisesAvg = deviation; break;
						case 3 : choisesMin = deviation; break;
						case 4 : choisesMax = deviation; break;
						case 5 : justice = deviation; break;
						case 6 : leaderTime = deviation; break;
					}
				}
			}
		}
		else {
			for(int loop2 = 0; loop2 < 7; loop2++)
			{
				values = GetPlayerStatAsVector(loop2, loop3);
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
					case 1 : winner = val; break;
					case 2 : choisesAvg = val; break;
					case 3 : choisesMin = val; break;
					case 4 : choisesMax = val; break;
					case 5 : justice = val; break;
					case 6 : leaderTime = val; break;
				}
			}
		}
	
		playerItem->setData(2, 0, winner);
		playerItem->setData(3, 0, choisesAvg);
		playerItem->setData(4, 0, choisesMin);
		playerItem->setData(5, 0, choisesMax);
		playerItem->setData(6, 0, turnNumber);
		playerItem->setData(7, 0, justice);
		playerItem->setData(8, 0, leaderTime);
	}
}


vector<float> BatchItem::GetPlayerStatAsVector(int statName, int playerID)
{
	vector<float> result;

	float val;
	int realBatchSize = treeWidgetItem->data(2, 0).toInt();
	for(int loop1 = 0; loop1 < realBatchSize; loop1++)
	{
		switch(statName)
		{
			case 0 : val = allGameStat[loop1]->PlayerTurnNumber(playerID); break;
			case 1 : val = allGameStat[loop1]->PlayerWinner(playerID); break;
			case 2 : val = allGameStat[loop1]->PlayerChoisesSum(playerID) / (float) allGameStat[loop1]->PlayerTurnNumber(playerID); break;
			case 3 : val = allGameStat[loop1]->PlayerChoisesMin(playerID); break;
			case 4 : val = allGameStat[loop1]->PlayerChoisesMax(playerID); break;
			case 5 : val = allGameStat[loop1]->PlayerDeltaH(playerID); break;
			case 6 : val = allGameStat[loop1]->PlayerLeaderTime(playerID); break;
		}

		result.push_back(val);
	}

	return result;
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
			case 5 : val = allGameStat[loop1]->Justice(); break;
			case 6 : val = allGameStat[loop1]->Control(); break;
			case 7 : val = allGameStat[loop1]->LeaderTime(); break;
		}

		result.push_back(val);
	}

	return result;
}
