#ifndef _IGAMEMAZE_H_
#define _IGAMEMAZE_H_

#include <qpainter.h>
#include "Game.h"
#include "MazeState.h"

class GameMaze : public Game
{
	Q_OBJECT

private :
	MazeState * currentState;

	int lastMouseX;
	int lastMouseY;

	float tileWidth;
	float tileHeight;
public :
	GameMaze(QWidget * _widget);
	~GameMaze();
	void StartGame();
	bool PlayerTurn();
	int * GetPlayerChoises();
	void Draw(QPainter * painter, int tickMillis = 0);
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
};

#endif