#ifndef _IPLAYER_H_
#define _IPLAYER_H_

#include <qobject.h>

class IGame;

class IPlayer : public QObject
{
Q_OBJECT

protected :
	IGame * game;
	int isReady;
	int myTurn;
	int myID;
public:
	IPlayer(int _myID) { myID = _myID; };
	virtual ~IPlayer() {};
	virtual void StartGame(IGame * _game) {game = _game; isReady = false;};
	virtual bool IsReady();
	virtual int MakeTurn();
	virtual bool Think() = 0;
	virtual void HumanTurn(int turn) {};
	virtual QString GetAINAme() = 0;
signals:
	void ImReady();
};

#endif