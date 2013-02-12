#ifndef _GAME_H_
#define _GAME_H_

#include "igame.h"
#include "GameStat.h"
#include "IPlayer.h"
#include <qobject.h>
#include <qwidget.h>

typedef enum State {
	STATE_STOPPED,
	STATE_RUNNING,
	STATE_GAME_OVER
};

class Game : public IGame
{
	Q_OBJECT

protected:
	static const int ENVIRONMENTAL_AI_ID = 0;
	bool paint;

	State state;
	int playerCount;
	int playerLeader;
	int * currentPlayerScore;
	IPlayer ** player;
	int * currentPlayerLevel;
	GameStat * gameStat;

	int minPlayerAI;
	int maxPlayerAI;

	int turnNumber;

	QWidget * widget;

	vector<IGameState *> gameState;
	void ClearAllGameStates();
	bool SaveAllGameStates() { return paint; }
	int GetLeaderID(int * outScoreDifference);
public:
	Game(QWidget * _widget, bool paint = true);
	virtual ~Game(void);
	virtual IGameState * GetCurrentState() const = 0;
	virtual bool PlayerTurn() = 0; /// Game specific turn
	virtual void NextTurn();       /// Common for all games
	virtual void SetPlayer(int playerID, IPlayer * _player);
	virtual void Draw(QPainter * painter, int tickMillis = 0) = 0;
	virtual IGame * Factory(QWidget * _widget, bool _paint = true) = 0;
	virtual pair<QString,QString> GetSetupString() = 0;
	virtual vector<pair<QWidget *, QString> > GetSetupWidget() = 0; 
	virtual QString GetGameName() = 0;
	virtual void Paint(QPainter * painter);
	virtual void StartGame();


	IPlayer * GetPlayer(int ID) { return player[ID]; };
	int GetMinPlayerAI() { return minPlayerAI; };
	int GetMaxPlayerAI() { return maxPlayerAI; };
	int GetPlayerCount() { return playerCount;};
	GameStat GetGameStat() { return *gameStat; }
	vector<IGameState *> GetAllGameState() { return gameState;} ;
	int GetPlayerLevel(int playerID) { return currentPlayerLevel[playerID]; };
};

#endif
