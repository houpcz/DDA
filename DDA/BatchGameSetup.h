#ifndef _BATCH_GAME_SETUP_H_
#define _BATCH_GAME_SETUP_H_

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include "EnvironmentAI.h"
#include "IGame.h"

using namespace std;

class BatchGameSetup : public QDialog
{
	Q_OBJECT

private :
	IGame * game;
	QGridLayout * gridLayout;
	QComboBox ** playerList;
	QSpinBox ** playerLevel;
	QPushButton * okButton;
	vector<IEnvironmentAI *> environmentAIList;
	vector<IPlayer *> playerAIList;
	int playerCount;
public:
	BatchGameSetup(IGame * _game, vector<IEnvironmentAI *> _environmentAIList, vector<IPlayer *> _playerAIList, bool human = false, QWidget *parent = 0);
	~BatchGameSetup(void);
public slots:
		void SaveSetup();
};

#endif