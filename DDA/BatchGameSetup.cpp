#include "BatchGameSetup.h"
#include <QLabel>

BatchGameSetup::BatchGameSetup(IGame * _game, vector<IPlayer *> _playerAI, bool human, QWidget *parent) : QDialog(parent)
{
	game = _game;
	setWindowTitle(game->GetGameName());
	playerAI = _playerAI;

	playerCount = game->GetPlayerCount();

	gridLayout = new QGridLayout();
	playerList = new QComboBox*[playerCount];
	playerLevel = new QSpinBox*[playerCount - 1];

	playerList[0] = new QComboBox(this);
	playerList[0]->addItem(tr("Environmental AI"));
	gridLayout->addWidget(playerList[0], 0, 0);
	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		playerList[loop1] = new QComboBox(this);
		playerLevel[loop1-1] = new QSpinBox(this);
		playerLevel[loop1-1]->setRange(1, 100);
		playerLevel[loop1-1]->setValue(game->GetPlayer(loop1)->Level());

		for(int loop2 = (human) ? 0 : 1; loop2 < playerAI.size(); loop2++)
		{
			playerList[loop1]->addItem(playerAI[loop2]->GetAIName());
		}

		gridLayout->addWidget(playerList[loop1], loop1, 0);
		gridLayout->addWidget(playerLevel[loop1-1], loop1, 1);
	} 

	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		for(int loop2 = 1; loop2 < playerAI.size(); loop2++)
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
	for(int loop1 = 1; loop1 < playerCount; loop1++)
	{
		QString str1 = playerList[loop1]->currentText();
		for(int loop2 = 1; loop2 < playerAI.size(); loop2++)
		{		
			QString str2 = playerAI[loop2]->GetAIName();
			if(str1.compare(str2) == 0)
			{
				game->SetPlayer(loop1, playerAI[loop2]);
				game->GetPlayer(loop1)->SetLevel(playerLevel[loop1 - 1]->value());
				break;
			}
		}
	}

	this->close();
}