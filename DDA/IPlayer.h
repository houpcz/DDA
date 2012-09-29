#ifndef _IPLAYER_H_
#define _IPLAYER_H_

#include <qobject.h>
#include "IGame.h"

class IPlayer : public QObject
{
Q_OBJECT

protected :
	IGame * game;
	int isReady;
	int myTurn;
public:
	virtual void StartGame(IGame * _game) {game = _game; isReady = false;};
	virtual bool IsReady();
	virtual int MakeTurn();
	virtual bool Think() = 0;
	virtual void HumanTurn(int turn) {};

signals:
	void ImReady();
};

#endif