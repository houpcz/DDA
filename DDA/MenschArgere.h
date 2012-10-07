#pragma once
#include "game.h"
#include "IGameState.h"
#include "MenschState.h"

class MenschTile
{
	int x;
	int y;
	QColor color;
public:
	MenschTile(int _x, int _y, QColor _color);
	void Draw(QPainter * painter, int tickMillis = 0);
	void DrawPlayer(QPainter * painter, const QColor & playerColor);
};

class MenschArgere :
	public Game
{
public :
	static const int MAX_TILE = 72;
	static const int PLAYER_1_START = 40;
	static const int PLAYER_1_END = 56;
	static const int firstTile[MenschState::MAX_PLAYER];

private:
	MenschState *currentState;
	MenschTile *tileGame[MAX_TILE];
	
	static const QColor playerColor[MenschState::MAX_PLAYER];
public:
	MenschArgere(QWidget * _widget);
	~MenschArgere(void);
	bool PlayerTurn();
	void StartGame();
	void Draw(QPainter * painter, int tickMillis = 0);
	IGameState * GetCurrentState() const;
};
