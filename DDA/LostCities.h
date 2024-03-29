#ifndef _LOSTCITIES_H_
#define _LOSTCITIES_H_

#include <QObject>
#include "LostCitiesState.h"
#include "game.h"
#include "IPlayer.h"
#include "ClickableArea.h"

class LostCities :
	public Game
{
	Q_OBJECT

private :
	static const int HAND_SIZE = 8;
	LostCitiesState * currentState;
	int cardWidth, cardHeight;
	int realHandSize;
	bool domination;
	static const int CLICKABLE_MAX_ACTIVE = 4;
	static const int CLICKABLE_HOVER = 0;
	static const int CLICKABLE_PLAY_FROM = 1;
	static const int CLICKABLE_PLAY_TO = 2;
	static const int CLICKABLE_DRAW_FROM = 3;
	static const int CLICKABLE_AREAS = 19;
	int highlightClickableAreas[CLICKABLE_MAX_ACTIVE];
	ClickableArea clickableArea[CLICKABLE_AREAS];

	int GetCardColor(int cardID) { return cardID / LostCitiesState::CARD_ONE_COLOR_AMOUNT; };
	int GetCardValue(int cardID) { return cardID % LostCitiesState::CARD_ONE_COLOR_AMOUNT; };
	void UpdateActiveClickableAres();
public:
	LostCities(QWidget * _widget, bool _paint = true);
	~LostCities(void);
	bool PlayerTurn();
	void StartGame();
	void Draw(QPainter * painter, int tickMillis = 0);
	void DrawCard(QPainter * painter, int cardID, int x, int y);
	QString GetGameName() { return QString::fromLocal8Bit("Ztracen� m�sta"); };
	IGameState * GetCurrentState() const;
	void MouseMoveEvent ( int xMouse, int yMouse );
	void MousePressEvent ( int xMouse, int yMouse );
	virtual pair<QString,QString> GetSetupString();
	virtual vector<pair<QWidget *, QString> > GetSetupWidget(); 
	int GetGameID() { return GAME_LC_ID; };
	virtual IGame * Factory(QWidget * _widget, bool _paint = true) { 
		LostCities * game =  new LostCities(_widget, _paint); 
		game->realHandSize = realHandSize;
		game->domination = domination;
		return game;
	};
public slots:
	void SetHandSize(int _handSize);
	void SetDomination(int state);
};

#endif

