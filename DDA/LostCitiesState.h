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
	static const int ENVIRONMENTAL_AI = 0;
	static const int PLAYER_AMOUNT = 2;
	static const int COLOR_AMOUNT = 5;
	static const int CARD_AMOUNT = 60;
	static const int CARD_ONE_COLOR_AMOUNT = CARD_AMOUNT / COLOR_AMOUNT;
	static const int DISCARD_CARD_OFFSET = 60;
private:
	char card[CARD_AMOUNT];
	char allChoises[CARD_AMOUNT]; // valid "playerChoises" values, ids of cards which can be played
	char allChoisesPhase2[COLOR_AMOUNT];
	int activePlayerID;
	int lastRealPlayer; // not environmental AI
	int playerChoises;
	int playerChoisesPhase1;
	int playerChoisesPhase2;		// drawing card phase
	int discardPileTopCard[COLOR_AMOUNT];

	void InitGame(int handSize);
	void CountPlayerChoises();
public:
	LostCitiesState();
	~LostCitiesState(void);
	char GetCard(int id) { return card[id]; }
	int GetPlayerChoises() const;
	int GetActivePlayerID() const;
	int GetPlayerScore(int playerID) const;
	IGameState ** GetNextStates(int *outNumberNextStates) const;
	bool MakeTurn(int turn);
};

#endif

