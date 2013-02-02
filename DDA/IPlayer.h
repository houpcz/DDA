#ifndef _IPLAYER_H_
#define _IPLAYER_H_

#include <qobject.h>

class IGame;

typedef std::pair<int,int> valueIndex;
bool comparator ( const valueIndex& l, const valueIndex& r);

class IPlayer : public QObject
{
Q_OBJECT

protected :
	IGame * game;
	int level;
	int isReady;
	int myTurn;
	int myID;
public:
	IPlayer(int _myID);
	virtual ~IPlayer() {};
	virtual void StartGame(IGame * _game) {game = _game; isReady = false;};
	virtual void SetGame(IGame * _game) {game = _game;};
	virtual bool IsReady();
	virtual int MakeTurn();
	virtual bool Think() = 0;
	virtual void HumanTurn(int turn) {};
	virtual QString GetAIName() = 0;
	void SetLevel(int _level) { level = _level; };
	int Level() { return level; };
	virtual bool IsScalable() = 0;
	virtual IPlayer * Factory(int playerID) = 0;
signals:
	void ImReady();
};

#endif