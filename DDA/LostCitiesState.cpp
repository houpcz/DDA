
#include <algorithm>
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
	
	random_shuffle(allCards, allCards + CARD_AMOUNT);
	
	for(int loop1 = 0; loop1 < handSize; loop1++)
		card[allCards[loop1]] = PLAYER_1_HAND;
	for(int loop1 = handSize; loop1 < 2 * handSize; loop1++)
		card[allCards[loop1]] = PLAYER_2_HAND;

	CountPlayerChoises();
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
		int allChoisesID = turn % playerChoisesPhase1;
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
			card[cardID] = discardPileTopCard[cardColorNumber] + 1;
		}
		// Phase 2 - drawing card
		int drawSite = turn / playerChoisesPhase1;
		if(drawSite == 0) // draw from deck
		{
			activePlayerID = 0; // environment plays
		} else {
			if(activePlayerID == 1)
			{
				card[allChoisesPhase2[drawSite - 1]] = PLAYER_1_HAND;
				activePlayerID = 2;
				lastRealPlayer = 2;
			} else {
				card[allChoisesPhase2[drawSite - 1]] = PLAYER_2_HAND;
				activePlayerID = 1;
				lastRealPlayer = 1;
			}
		}
	}
	CountPlayerChoises();
	return false;
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
		playerChoisesPhase1 = 0;
		playerChoisesPhase2 = 1;	// can draw from main pile
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
			bool wasCountDiscardPileForThisColor = false;
			discardPileTopCard[loop1] = ON_DESK;
			for(char loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 0; loop2--)
			{
				char cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;
				if(card[cardID] == playerHand)
				{
					allChoises[playerChoisesPhase1++] = cardID;	// can discard card
					if(canAddCardToExpedition)
					{
						allChoises[playerChoisesPhase1++] = cardID + DISCARD_CARD_OFFSET; // may add to expedition
					}
				} else if(canAddCardToExpedition && card[cardID] == playerOnDesk) // you can't add lower value card to existing expedition
				{
					canAddCardToExpedition = false; 
				} else if(card[cardID] >= ON_DESK)
				{
					if(!wasCountDiscardPileForThisColor)
					{
						playerChoisesPhase2++;			// can draw from discard pile
						wasCountDiscardPileForThisColor = true;
					}

					if(discardPileTopCard[loop1] < card[cardID])
					{
						discardPileTopCard[loop1] = card[cardID];
						allChoisesPhase2[playerChoisesPhase2 - 2] = cardID; // constat 2 - 1 is for drawing from deck, 2 because we added new Choise for this color before
					}
				}
			}
		}

		playerChoises = playerChoisesPhase2 * playerChoisesPhase1;
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