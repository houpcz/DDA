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
	GameMaze(QWidget * _widget, bool _paint = true);
	virtual ~GameMaze();
	void StartGame();
	bool PlayerTurn();
	QString GetGameName() { return tr("Maze"); };
	IGameState * GetCurrentState() const;
	void Draw(QPainter * painter, int tickMillis = 0);
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
	virtual vector<pair<QWidget *, QString> > GetSetupWidget(); 
	public slots:
		void SetMazeWidth(int width);
		void SetMazeHeight(int height);
};

#endif