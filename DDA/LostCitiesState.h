#ifndef _LOSTCITIESSTATE_H_
#define _LOSTCITIESSTATE_H_

#include <QWidget>
#include "igamestate.h"

enum CardPosition
{
	IN_DECK,
	PLAYER_1_HAND,
	PLAYER_2_HAND,
	PLAYER_1_ON_DESK,
	PLAYER_2_ON_DESK,
	ON_DESK
};

class LostCitiesState : public IGameState
{
public :
	static const int DRAW_FROM_DECK = 5;
	static const int MAX_CHOISES = 88;		// 8 cards, each 2 option max (16 play options), first 8 play option has max 6 draw options, last 8 max 5 draw options
	static const int ENVIRONMENTAL_AI = 0;
	static const int PLAYER_AMOUNT = 2;
	static const int COLOR_AMOUNT = 5;
	static const int CARD_AMOUNT = 60;
	static const int CARD_ONE_COLOR_AMOUNT = CARD_AMOUNT / COLOR_AMOUNT;
	static const int DISCARD_CARD_OFFSET = 60;
private:
	char card[CARD_AMOUNT];
	char allChoises[MAX_CHOISES]; // valid "playerChoises" values, ids of cards which can be played and where
	char drawFrom[MAX_CHOISES];   // ids to discardOnTop
	int activePlayerID;
	int lastRealPlayer; // not environmental AI
	int playerChoises;
	int discardPileTopCardCode[COLOR_AMOUNT];
	int discardPileTopCardID[COLOR_AMOUNT];

	void InitGame(int handSize);
	void CountPlayerChoises();
public:
	LostCitiesState();
	virtual ~LostCitiesState(void);
	LostCitiesState(const LostCitiesState & origin);
	LostCitiesState& operator=(const LostCitiesState &origin);
	void CopyToMe(const LostCitiesState & origin);

	char GetCard(int id) { return card[id]; }
	virtual int GetPlayerChoises() const;
	virtual int GetActivePlayerID() const;
	virtual int GetPlayerScore(int playerID) const;
	virtual IGameState ** GetNextStates(int *outNumberNextStates) const;
	bool MakeTurn(int turn);
	int GetTurnID(int playCardID, int drawSite);
};

#endif

