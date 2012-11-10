#ifndef _LOSTCITIES_H_
#define _LOSTCITIES_H_

#include "LostCitiesState.h"
#include "game.h"
#include "IPlayer.h"

class LostCities :
	public Game
{
private :
	LostCitiesState * currentState;
	int cardWidth, cardHeight;
public:
	LostCities(QWidget * _widget);
	~LostCities(void);
	bool PlayerTurn();
	void StartGame();
	void Draw(QPainter * painter, int tickMillis = 0);
	void DrawCard(QPainter * painter, int cardID, int x, int y);
	IGameState * GetCurrentState() const;
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
};

#endif

