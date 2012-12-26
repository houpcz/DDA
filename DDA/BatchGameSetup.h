#ifndef _BATCH_GAME_SETUP_H_
#define _BATCH_GAME_SETUP_H_

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QGridLayout>
#include <vector>
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
	vector<IPlayer *> playerAI;
public:
	BatchGameSetup(IGame * _game, vector<IPlayer *> _playerAI, QWidget *parent = 0);
	~BatchGameSetup(void);
};

#endif