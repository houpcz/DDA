#include "BatchGameSetup.h"
#include <QLabel>

BatchGameSetup::BatchGameSetup(IGame * _game, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, bool _human, QWidget *parent) : QDialog(parent)
{
	init = false;
	game = _game;
	human = _human;
	setWindowTitle(game->GetGameName());
	playerAIList = _playerAIList;
	environmentAIList = _environmentAIList;

	playerCount = game->GetPlayerCount();

	gridLayout = new QGridLayout();
	playerList = new QComboBox*[playerCount];
	playerOff = new vector<int>[playerCount - 1];
	playerLevel = new QSpinBox*[playerCount - 1];

	playerList[0] = new QComboBox(this);
	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{
		if(game->GetGameID() != 0 && loop1 >= environmentAIList.size() - 2)
			break;

		if(game->GetGameID() == 0 && loop1 == environmentAIList.size() - 3)
			continue;

		playerList[0]->addItem(environmentAIList[loop1]->GetAIName());
	}

	connect(playerList[0], SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateLevelSpin()));
	gridLayout->addWidget(playerList[0], 0, 2);
	int leapedNumber;
	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		leapedNumber = 0;

		playerList[loop1] = new QComboBox(this);
		playerLevel[loop1-1] = new QSpinBox(this);
		playerLevel[loop1-1]->setRange(1, 100);
		playerLevel[loop1-1]->setValue(game->GetPlayer(loop1)->Level());
		connect(playerList[loop1], SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateLevelSpin()));
		for(int loop2 = (human && loop1 == 1) ? 0 : 1; loop2 < playerAIList.size(); loop2++)
		{
			if(playerAIList[loop2]->IsCompatibleWithGame(game->GetGameID()))
			{
				playerOff[loop1 - 1].push_back(leapedNumber);
				playerList[loop1]->addItem(playerAIList[loop2]->GetAIName());
			} else {
				leapedNumber++;
			}
		}

		gridLayout->addWidget(playerList[loop1], loop1 - 1, 0);
		gridLayout->addWidget(playerLevel[loop1-1], loop1 - 1, 1);
	} 


	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{
		QString str1 = playerList[0]->itemText(loop1);
		QString str2 = game->GetPlayer(0)->GetAIName();
		if(str1.compare(str2) == 0)
		{
			playerList[0]->setCurrentIndex(loop1);
			break;
		}
	}

	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		for(int loop2 = 1; loop2 < playerAIList.size(); loop2++)
		{
			QString str1 = playerList[loop1]->itemText(loop2);
			QString str2 = game->GetPlayer(loop1)->GetAIName();
			if(str1.compare(str2) == 0)
			{
				playerList[loop1]->setCurrentIndex(loop2);
				break;
			}
		}
	}

	okButton = new QPushButton(QString::fromLocal8Bit("Uložit"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(SaveSetup()));
	gridLayout->addWidget(okButton, 1000, 3);

	vector<pair<QWidget *, QString> > gSS = game->GetSetupWidget();
	for(int loop1 = 0; loop1 < gSS.size(); loop1++)
	{
		gridLayout->addWidget(new QLabel(gSS[loop1].second), playerCount + loop1 - 1, 0);
		gridLayout->addWidget(gSS[loop1].first, playerCount + loop1 - 1, 1);
	}

	eaCoefLabel[COEF_LEADER_SWITCHES] = new QLabel(QString::fromLocal8Bit("Prohození vítìzù"));
	eaCoefLabel[COEF_CREDIBILITY] = new QLabel(QString::fromLocal8Bit("Uvìøitelnost"));
	eaCoefLabel[COEF_JUSTICE] = new QLabel(QString::fromLocal8Bit("Spravedlnost"));
	eaCoefLabel[COEF_LEADER_TIME] = new QLabel(QString::fromLocal8Bit("Doba vedení"));
	eaCoefLabel[COEF_AVG_STATUS_DIFFERENCE] = new QLabel(QString::fromLocal8Bit("Napínavost"));
	eaCoefLabel[COEF_END_STATUS_DIFFERENCE] = new QLabel(QString::fromLocal8Bit("Náskok"));
	eaCoefLabel[COEF_RANDOMNESS] = new QLabel(QString::fromLocal8Bit("Náhodnost"));
	eaCoefLabel[COEF_FREEDOM] = new QLabel(QString::fromLocal8Bit("Svoboda"));
	for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
	{
		eaCoefBox[loop1] = new QSpinBox(this);
		eaCoefBox[loop1]->setMinimum(0);
		eaCoefBox[loop1]->setMaximum(999999);
		gridLayout->addWidget(eaCoefBox[loop1], loop1 + 1, 3);
		gridLayout->addWidget(eaCoefLabel[loop1], loop1 + 1, 2);
	}

	setLayout(gridLayout);

	init = true;
	UpdateLevelSpin();
}


BatchGameSetup::~BatchGameSetup(void)
{
	for(int loop1 = 0; loop1 < playerCount; loop1++)
	{
		delete playerList[loop1];
	}
	for(int loop1 = 0; loop1 < playerCount - 1; loop1++)
	{
		delete playerLevel[loop1];
	}
	delete [] playerList;
	delete [] playerLevel;
	delete [] playerOff;
}

void BatchGameSetup::SaveSetup()
{
	int playerCount = game->GetPlayerCount();

	QString str1 = playerList[0]->currentText();
	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{		
		QString str2 = environmentAIList[loop1]->GetAIName();
		if(str1.compare(str2) == 0)
		{
			for(int loop2 = 0; loop2 < COEF_COUNT; loop2++)
			{
				environmentAIList[loop1]->SetMetricCoef(loop2, eaCoefBox[loop2]->value());
			}

			game->SetPlayer(0, environmentAIList[loop1]);
			IPlayer * player = game->GetPlayer(0);
			EnvironmentAI * ePlayer = dynamic_cast<EnvironmentAI*>(player);

			for(int loop2 = 0; loop2 < COEF_COUNT; loop2++)
			{
				ePlayer->SetMetricCoef(loop2, eaCoefBox[loop2]->value());
			}
			
			break;
		}
	}

	


	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		str1 = playerList[loop1]->currentText();
		for(int loop2 = 0; loop2 < playerAIList.size(); loop2++)
		{		
			QString str2 = playerAIList[loop2]->GetAIName();
			if(str1.compare(str2) == 0)
			{
				game->SetPlayer(loop1, playerAIList[loop2]);
				game->GetPlayer(loop1)->SetLevel(playerLevel[loop1 - 1]->value());
				break;
			}
		}
	}

	this->close();
}

void BatchGameSetup::UpdateLevelSpin()
{
	if(!init)
		return;

	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		int id1 = playerOff[loop1 - 1][playerList[loop1]->currentIndex()];
		int id2 = playerList[loop1]->currentIndex();
		int id3 = ((human && loop1 == 1) ? 0 : 1);
		int sumID = id1 + id2 + id3;
		playerLevel[loop1 - 1]->setEnabled(playerAIList[sumID]->IsScalable());
	}


	EnvironmentAI * ePlayer = dynamic_cast<EnvironmentAI*>(game->GetPlayer(0));
	float * coefs = ePlayer->CoefMetric();
	for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
	{
		eaCoefBox[loop1]->setValue(coefs[loop1]);
	}
	/*
	QString str1 = playerList[0]->currentText();
	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
	{		
		QString str2 = environmentAIList[loop1]->GetAIName();
		if(str1.compare(str2) == 0)
		{
			EnvironmentAI * ePlayer = dynamic_cast<EnvironmentAI*>(environmentAIList[loop1]);
			float * coefs = ePlayer->CoefMetric();
			for(int loop1 = 0; loop1 < KOEF_COUNT; loop1++)
			{
				eaCoefBox[loop1]->setValue(coefs[loop1]);
				eaCoefBox[loop1]->setEnabled(ePlayer->CoefsHaveMeaning());
			}
		}
	}*/
}