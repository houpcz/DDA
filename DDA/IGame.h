#ifndef _IGAME_H_
#define _IGAME_H_

#include <qpainter.h>
#include <QMouseEvent>
#include <qobject.h>
#include <vector>
#include "IGameState.h"
#include "IPlayer.h"
#include "GameStat.h"

using namespace std;

class IGame : public QObject
{
Q_OBJECT

public:
	IGame() {};
	virtual ~IGame() {};
	virtual void NextTurn() = 0;

	/// Execute player turn
	/**
	  The active player has decided what to do. This method ask him for his choise and make turn

	  \return True if next state is game over
	*/
	virtual bool PlayerTurn() = 0;

	virtual void StartGame() = 0;
	virtual IGameState * GetCurrentState() const = 0;
	virtual void Paint(QPainter * painter) = 0;
	virtual void MouseMoveEvent ( int xMouse, int yMouse ) {};
	virtual void MousePressEvent ( int xMouse, int yMouse ) {};

	virtual void SetPlayer(int playerID, int aiID) = 0;
	virtual GameStat GetGameStat() = 0;
	virtual int GetMinPlayerAI() = 0;
	virtual int GetMaxPlayerAI() = 0;
	virtual int GetPlayerCount() = 0;
	virtual IPlayer * GetPlayer(int ID) = 0;
	virtual QString GetGameName() = 0;
	virtual vector<pair<QWidget *, QString> > GetSetupWidget() = 0; 
public slots:
	void PlayerIsReady();
};

#endif
