#pragma once
#include "game.h"
#include "IGameState.h"
#include "MenschState.h"
#include "MenschTile.h"

class MenschArgere :
	public Game
{
public :
	static const int MAX_TILE = 73;
	static const int PLAYER_1_START = 40;
	static const int PLAYER_1_END = 56;
	static const int firstTile[MenschState::MAX_PLAYER];

private:
	MenschState *currentState;
	MenschTile *tileGame[MAX_TILE];
	float tileWidth;
	float tileHeight;
	float boardWidth;
	float boardHeight;
	
	static const QColor playerColor[MenschState::MAX_PLAYER];
	int GetTileID(int player, int figure, int & atStart);
public:
	MenschArgere(QWidget * _widget);
	virtual ~MenschArgere(void);
	bool PlayerTurn();
	void StartGame();
	void Draw(QPainter * painter, int tickMillis = 0);
	IGameState * GetCurrentState() const;
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
	float GetTileWidth() { return tileWidth; }
	float GetTileHeight() { return tileHeight; }
	float GetBoardHeight() { return boardHeight; }
};
