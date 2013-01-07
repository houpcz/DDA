#include "BatchGameSetup.h"
#include <QLabel>

BatchGameSetup::BatchGameSetup(IGame * _game, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, bool human, QWidget *parent) : QDialog(parent)
{
	game = _game;
	setWindowTitle(game->GetGameName());
	playerAIList = _playerAIList;
	environmentAIList = _environmentAIList;

	playerCount = game->GetPlayerCount();

	gridLayout = new QGridLayout();
	playerList = new QComboBox*[playerCount];
	playerLevel = new QSpinBox*[playerCount - 1];

	playerList[0] = new QComboBox(this);
	for(int loop1 = 0; loop1 < environmentAIList.size(); loop1++)
		playerList[0]->addItem(environmentAIList[loop1]->GetAIName());

	gridLayout->addWidget(playerList[0], 0, 0);
	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		playerList[loop1] = new QComboBox(this);
		playerLevel[loop1-1] = new QSpinBox(this);
		playerLevel[loop1-1]->setRange(1, 100);
		playerLevel[loop1-1]->setValue(game->GetPlayer(loop1)->Level());

		for(int loop2 = (human) ? 0 : 1; loop2 < playerAIList.size(); loop2++)
		{
			playerList[loop1]->addItem(playerAIList[loop2]->GetAIName());
		}

		gridLayout->addWidget(playerList[loop1], loop1, 0);
		gridLayout->addWidget(playerLevel[loop1-1], loop1, 1);
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

	okButton = new QPushButton(tr("Save"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(SaveSetup()));
	gridLayout->addWidget(okButton, 0, 1);

	vector<pair<QWidget *, QString> > gSS = game->GetSetupWidget();
	for(int loop1 = 0; loop1 < gSS.size(); loop1++)
	{
		gridLayout->addWidget(new QLabel(gSS[loop1].second), playerCount + loop1, 0);
		gridLayout->addWidget(gSS[loop1].first, playerCount + loop1, 1);
	}

	setLayout(gridLayout);
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
}

void BatchGameSetup::SaveSetup()
{
	int playerCount = game->GetPlayerCount();

	QString str1 = playerList[0]->currentText();
	for(int loop1 = 1; loop1 < environmentAIList.size(); loop1++)
	{		
		QString str2 = environmentAIList[loop1]->GetAIName();
		if(str1.compare(str2) == 0)
		{
			game->SetPlayer(loop1, environmentAIList[loop1]);
			break;
		}
	}


	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		str1 = playerList[loop1]->currentText();
		for(int loop2 = 1; loop2 < playerAIList.size(); loop2++)
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