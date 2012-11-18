
#include <algorithm>
#include <ctime>
#include "LostCitiesState.h"

using namespace std;

LostCitiesState::LostCitiesState()
{
	InitGame(8);
}

void LostCitiesState::InitGame(int handSize)
{
	activePlayerID = 1;
	lastRealPlayer = 1;

	int allCards[CARD_AMOUNT];
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		card[loop1] = IN_DECK;
		allCards[loop1] = loop1;
	}
	
	srand(time(NULL)); 
	random_shuffle(allCards, allCards + CARD_AMOUNT);
	
	for(int loop1 = 0; loop1 < handSize; loop1++)
		card[allCards[loop1]] = PLAYER_1_HAND;
	for(int loop1 = handSize; loop1 < 2 * handSize; loop1++)
		card[allCards[loop1]] = PLAYER_2_HAND;

	CountPlayerChoises();
}

LostCitiesState::LostCitiesState(const LostCitiesState & origin)
{
	CopyToMe(origin);
}

LostCitiesState& LostCitiesState::operator=(const LostCitiesState &origin)
{
	if(&origin != this)
	{
		CopyToMe(origin);
	}

	return *this;
}

void LostCitiesState::CopyToMe(const LostCitiesState & origin)
{
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
		card[loop1] = origin.card[loop1];

	activePlayerID = origin.activePlayerID;
	lastRealPlayer = origin.lastRealPlayer;
	playerChoises = origin.playerChoises;

	for(int loop1 = 0; loop1 < playerChoises; loop1++)
	{
		allChoises[loop1] = origin.allChoises[loop1];
		drawFrom[loop1] = origin.allChoises[loop1];
	}

	for(int loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
	{
		discardPileTopCardCode[loop1] = origin.discardPileTopCardCode[loop1];
		discardPileTopCardID[loop1] = origin.discardPileTopCardID[loop1];
	}
}

LostCitiesState::~LostCitiesState(void)
{
}

bool LostCitiesState::MakeTurn(int turn)
{
	if(activePlayerID == ENVIRONMENTAL_AI)
	{
		if(lastRealPlayer == 1)
		{
			card[allChoises[turn]] = PLAYER_1_HAND;
			activePlayerID = 2;
			lastRealPlayer = 2;
		} else {
			card[allChoises[turn]] = PLAYER_2_HAND;
			activePlayerID = 1;
			lastRealPlayer = 1;
		}
		if(playerChoises == 1)
			return true;
	} else {
		int allChoisesID = turn;
		int cardID = (allChoises[allChoisesID] >= DISCARD_CARD_OFFSET) ? allChoises[allChoisesID] - DISCARD_CARD_OFFSET: allChoises[allChoisesID];
		int cardColorNumber;
		cardColorNumber = cardID / CARD_ONE_COLOR_AMOUNT;
		// Phase 1 - playing card //
		if(allChoises[allChoisesID] >= DISCARD_CARD_OFFSET) // put card in expedition
		{
			
			if(activePlayerID == 1)
				card[cardID] = PLAYER_1_ON_DESK;
			else
				card[cardID] = PLAYER_2_ON_DESK;
		} else // discard card
		{
			card[cardID] = discardPileTopCardCode[cardColorNumber] + 1;
		}
		// Phase 2 - drawing card
		if(drawFrom[turn] == DRAW_FROM_DECK) // draw from deck
		{
			activePlayerID = 0; // environment plays
		} else {
			if(activePlayerID == 1)
			{
				card[discardPileTopCardID[drawFrom[turn]]] = PLAYER_1_HAND;
				activePlayerID = 2;
				lastRealPlayer = 2;
			} else {
				card[discardPileTopCardID[drawFrom[turn]]] = PLAYER_2_HAND;
				activePlayerID = 1;
				lastRealPlayer = 1;
			}
		}
	}
	CountPlayerChoises();
	return false;
}

int LostCitiesState::GetTurnID(int playCardID, int drawSite)
{
	int allChoisesID = 0;
	for(int loop1 = 0; loop1 < playerChoises; loop1++)
	{
		if(allChoises[loop1] == playCardID && drawFrom[loop1] == drawSite)
		{
			allChoisesID = loop1;
			break;
		}
	}

	return allChoisesID;
}
void LostCitiesState::CountPlayerChoises()
{
	playerChoises = 0;
	if(activePlayerID == ENVIRONMENTAL_AI)
	{
		for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
		{
			if(card[loop1] == IN_DECK)
			{
				allChoises[playerChoises++] = loop1;
			}
		}
	} else {
		int playerHand;
		int playerOnDesk;
		if(activePlayerID == 1)
		{
			playerHand = PLAYER_1_HAND;
			playerOnDesk = PLAYER_1_ON_DESK;
		} else {
			playerHand = PLAYER_2_HAND;
			playerOnDesk = PLAYER_2_ON_DESK;
		}
		for(char loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
		{
			bool canAddCardToExpedition = true;
			discardPileTopCardCode[loop1] = ON_DESK - 1;
			discardPileTopCardID[loop1] = -1;
			for(char loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 0; loop2--)
			{
				char cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;
				if(card[cardID] == playerHand)
				{
					allChoises[playerChoises] = cardID;	// can discard card
					drawFrom[playerChoises] = DRAW_FROM_DECK;
					playerChoises++;
					if(canAddCardToExpedition)
					{
						allChoises[playerChoises] = cardID + DISCARD_CARD_OFFSET; // may add to expedition
						drawFrom[playerChoises] = DRAW_FROM_DECK;
						playerChoises++;
					}
				} else if(canAddCardToExpedition && card[cardID] == playerOnDesk) // you can't add lower value card to existing expedition
				{
					canAddCardToExpedition = false; 
				} else if(card[cardID] >= ON_DESK)
				{
					if(discardPileTopCardCode[loop1] < card[cardID])
					{
						discardPileTopCardCode[loop1] = card[cardID];
						discardPileTopCardID[loop1] = cardID;
					}
				}
			}
		}

		int oldChoises = playerChoises;
		for(int loop1 = 0; loop1 < oldChoises; loop1++)
		{
			for(int loop2 = 0; loop2 < COLOR_AMOUNT; loop2++)
			{
				// if there is card on discard pile and if you dont play card of same color on discard pile, you can draw from thad pile
				if(discardPileTopCardID[loop2] >= 0 && (allChoises[loop1] >= DISCARD_CARD_OFFSET || loop2 != allChoises[loop1] / CARD_ONE_COLOR_AMOUNT))
				{
					allChoises[playerChoises] = allChoises[loop1];
					drawFrom[playerChoises] = loop2;
					playerChoises++;
				}
			}
		}
	}
}
int LostCitiesState::GetPlayerChoises() const
{
	return playerChoises;
}
int LostCitiesState::GetActivePlayerID() const
{
	return activePlayerID;
}
int LostCitiesState::GetPlayerScore(int playerID) const
{
	return 0;
}
IGameState ** LostCitiesState::GetNextStates(int *outNumberNextStates) const
{
	int numberNextStates = GetPlayerChoises();
	IGameState ** nextState = new IGameState*[numberNextStates];
	LostCitiesState * lostCitiesState;
	for(int loop1 = 0; loop1 < numberNextStates; loop1++)
	{
		lostCitiesState = new LostCitiesState(*this);
		lostCitiesState->MakeTurn(loop1);
		nextState[loop1] = lostCitiesState;
	}

	*outNumberNextStates = numberNextStates;

	return nextState;
}