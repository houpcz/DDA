#ifndef _LOSTCITIESSTATE_H_
#define _LOSTCITIESSTATE_H_

#include <QWidget>
#include <vector>
#include "igamestate.h"

using namespace std;

enum CardPosition
{
	IN_DECK,
	PLAYER_1_HAND_KNOWN,
	PLAYER_2_HAND_KNOWN,
	PLAYER_1_HAND_HIDDEN,
	PLAYER_2_HAND_HIDDEN,
	PLAYER_1_ON_DESK,
	PLAYER_2_ON_DESK,
	ON_DESK
};

class LostCitiesState : public IGameState
{
public :
	static const int PREDICTED_ON_BOARD = 10;
	static const int PREDICTED_IN_HAND = 9;
	static const int PREDICTED_TOP_DISCARD = 5;
	static const int PREDICTED_DECK = 2;
	static const int NOBODY = -1;
	static const int DRAW_FROM_DECK = 7;
	static const int ENVIRONMENTAL_AI = 0;
	static const int PLAYER_AMOUNT = 2;
	static const int COLOR_AMOUNT = 5;
	static const int CARD_AMOUNT = 60;
	static const int CARD_ONE_COLOR_AMOUNT = CARD_AMOUNT / COLOR_AMOUNT;
	static const int DISCARD_CARD_OFFSET = 60;
private:
	char card[CARD_AMOUNT];
	vector<char> allChoises; // valid "playerChoises" values, ids of cards which can be played and where
	vector<char> drawFrom;   // ids to discardOnTop
	int activePlayerID;
	int lastRealPlayer; // not environmental AI
	int discardPileTopCardCode[COLOR_AMOUNT];
	int discardPileTopCardID[COLOR_AMOUNT];
	int whoAskIDlast;
	bool isGameOver;
	int handSize;
	void InitGame(int _handSize);
	void WhoAsked(int whoAskID);
	void CountPlayerChoises(int whoAskID);
public:
	LostCitiesState(int _handSize);
	virtual ~LostCitiesState(void);
	LostCitiesState(const LostCitiesState & origin);
	LostCitiesState& operator=(const LostCitiesState &origin);
	ISpecificStat * GetGameSpecificStat();
	void CopyToMe(const LostCitiesState & origin);

	char GetCard(int id) { return card[id]; }
	virtual int GetPlayerChoises(int whoAskID);
	virtual int GetActivePlayerID() const;
	int GetPositivePlayerScore(int playerID, int whoAskID);
	virtual int GetPlayerScore(int playerID, int whoAskID);
	int GetPlayerPoints(int playerID);
	virtual IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	bool MakeTurn(int turn);
	int GetTurnID(int playCardID, int drawSite);
	bool IsGameOver();
};

#endif

