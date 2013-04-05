#pragma once
#include "game.h"
#include "IGameState.h"
#include "LudoState.h"
#include "LudoTile.h"

class Ludo :
	public Game
{
public :
	static const int MAX_TILE = 73;
	static const int PLAYER_1_START = 40;
	static const int PLAYER_1_END = 56;
	static const int firstTile[LudoState::MAX_PLAYER];

private:
	LudoState *currentState;
	LudoTile *tileGame[MAX_TILE];
	int activeHumanFigure;
	int boardX;
	int boardY;
	float tileWidth;
	float tileHeight;
	float boardWidth;
	float boardHeight;
	
	static const QColor playerColor[LudoState::MAX_PLAYER];
	int GetTileID(int player, int figure, int & atStart, bool nextState = false);
public:
	Ludo(QWidget * _widget, bool _paint = true);
	virtual ~Ludo(void);
	bool PlayerTurn();
	void StartGame();
	QString GetGameName() { return tr("Ludo"); };
	void Draw(QPainter * painter, int tickMillis = 0);
	IGameState * GetCurrentState() const;
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
	float GetTileWidth() { return tileWidth; }
	float GetTileHeight() { return tileHeight; }
	float GetBoardHeight() { return boardHeight; }
	virtual pair<QString,QString> GetSetupString();
	virtual vector<pair<QWidget *, QString> > GetSetupWidget(); 
	int GetGameID() { return GAME_LUDO_ID; };
	int BoardX() { return boardX; };
	int BoardY() { return boardY; };
	virtual IGame * Factory(QWidget * _widget, bool _paint = true) { 
		Ludo * game = new Ludo(_widget, _paint); 
		
		return game;
	};
};
