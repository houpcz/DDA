#include "BatchGameSetup.h"

BatchGameSetup::BatchGameSetup(IGame * _game, vector<IPlayer *> _playerAI, QWidget *parent) : QDialog(parent)
{
	game = _game;
	playerAI = _playerAI;

	int playerCount = game->GetPlayerCount();

	gridLayout = new QGridLayout();
	playerList = new QComboBox*[playerCount];
	playerLevel = new QSpinBox*[playerCount];
	for(int loop1 = 0; loop1 < playerCount; loop1++)
	{
		playerList[loop1] = new QComboBox(this);
		playerLevel[loop1] = new QSpinBox(this);
		playerLevel[loop1]->setRange(1, 100);

		for(int loop2 = 1; loop2 < playerAI.size(); loop2++)
		{
			playerList[loop1]->addItem(playerAI[loop2]->GetAINAme());
		}

		gridLayout->addWidget(playerList[loop1], loop1, 0);
		gridLayout->addWidget(playerLevel[loop1], loop1, 1);
	} 
	setLayout(gridLayout);
}


BatchGameSetup::~BatchGameSetup(void)
{
	for(int loop1 = 0; loop1 < game->GetPlayerCount(); loop1++)
	{
		delete playerList[loop1];
		delete playerLevel[loop1];
	}
	delete [] playerList;
	delete [] playerLevel;
}
