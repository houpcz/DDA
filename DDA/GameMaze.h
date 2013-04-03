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

	int mazeWidth;
	int mazeHeight;
	int stepsToGameOver;
	bool visibleGoals;
	bool abstraction;

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
	virtual pair<QString,QString> GetSetupString();
	int GetGameID() { return GAME_MAZE_ID; };
	virtual IGame * Factory(QWidget * _widget, bool _paint = true) { 
		GameMaze * game = new GameMaze(_widget, _paint); 
		game->mazeWidth = mazeWidth;
		game->mazeHeight = mazeHeight;
		game->stepsToGameOver = stepsToGameOver;
		game->abstraction = abstraction;
		game->visibleGoals = visibleGoals;
		return game;
	};

	public slots:
		void SetMazeWidth(int width);
		void SetMazeHeight(int height);
		void SetStepsToGameOver(int steps);
		void SetVisibleGoals(int state);
		void SetAbstraction(int state);
};

#endif