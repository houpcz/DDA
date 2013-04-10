
#include <algorithm>
#include <ctime>
#include "LostCitiesState.h"
#include "Utility.h"

using namespace std;

#define ZERO_PROB 0.0f;

LostCitiesState::LostCitiesState(int _handSize, bool _abstraction, Game * _game) : IGameState(_game)
{
	InitGame(_handSize, _abstraction);
	UpdateGameStat();
}

void LostCitiesState::InitGame(int _handSize, bool _domination)
{
	playerCount = 3;
	handSize = _handSize;
	activePlayerID = 1;
	lastRealPlayer = 1;
	cardsInDeckMaxTurn = 4;
	domination = _domination;

	int allCards[CARD_AMOUNT];
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		card[loop1] = IN_DECK;
		allCards[loop1] = loop1;

		probCard[0][loop1] = 1.0f / 60.0f;
		probCard[1][loop1] = 1.0f / 60.0f;
	}
	
	random_shuffle(allCards, allCards + CARD_AMOUNT);
	
	for(int loop1 = 0; loop1 < PLAYER_AMOUNT; loop1++)
	{
		for(int loop2 = 0; loop2 < COLOR_AMOUNT; loop2++)
		{
			colorCredibility[loop1][loop2] = 0;
		}

		for(int loop2 = 0; loop2 < VALUE_KINDS; loop2++)
		{
			valueCredibility[loop1][loop2] = 0;
		}
	}

	for(int loop1 = 0; loop1 < handSize; loop1++)
	{
		int cardNumber = (allCards[loop1] % 12) / 3;
		int cardColorNumber = allCards[loop1] / 12;
		card[allCards[loop1]] = PLAYER_1_HAND_HIDDEN;
		colorCredibility[0][cardColorNumber]++;
		valueCredibility[0][cardNumber]++;
	}
	for(int loop1 = handSize; loop1 < 2 * handSize; loop1++)
	{
		int cardNumber = (allCards[loop1] % 12) / 3;
		int cardColorNumber = allCards[loop1] / 12;
		card[allCards[loop1]] = PLAYER_2_HAND_HIDDEN;
		colorCredibility[1][cardColorNumber]++;
		valueCredibility[1][cardNumber]++;
	}

	cardsInDeck = GetInDeckCount();
	cardsInDeckTurn = 0;

	isGameOver = false;
	whoAskIDlast = NOBODY;
}

LostCitiesState::LostCitiesState(const LostCitiesState & origin)
{
	IGameState::Init(origin.game, &origin, origin.gameStat);
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
	lastPlayerID = origin.lastPlayerID;
	leaderTime = origin.leaderTime;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
		pStatus[loop1] = origin.pStatus[loop1];

	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		card[loop1] = origin.card[loop1];
		probCard[0][loop1] = origin.probCard[0][loop1];
		probCard[1][loop1] = origin.probCard[1][loop1];
	}

	activePlayerID = origin.activePlayerID;
	lastRealPlayer = origin.lastRealPlayer;
	isGameOver = origin.isGameOver;
	whoAskIDlast = origin.whoAskIDlast;
	handSize = origin.handSize;
	cardsInDeck = origin.cardsInDeck;
	cardsInDeckTurn = origin.cardsInDeckTurn;
	domination = origin.domination;
	allChoises = origin.allChoises;
	drawFrom = origin.drawFrom;

	for(int loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
	{
		discardPileTopCardCode[loop1] = origin.discardPileTopCardCode[loop1];
		discardPileTopCardID[loop1] = origin.discardPileTopCardID[loop1];
	}

	for(int loop1 = 0; loop1 < PLAYER_AMOUNT; loop1++)
	{
		for(int loop2 = 0; loop2 < COLOR_AMOUNT; loop2++)
		{
			colorCredibility[loop1][loop2] = origin.colorCredibility[loop1][loop2];
		}

		for(int loop2 = 0; loop2 < VALUE_KINDS; loop2++)
		{
			valueCredibility[loop1][loop2] = origin.valueCredibility[loop1][loop2];
		}
	}
}

LostCitiesState::~LostCitiesState(void)
{
}

bool LostCitiesState::IsGameOver()
{
	return isGameOver;
}
bool LostCitiesState::MakeTurn(int turn)
{
	if(activePlayerID == ENVIRONMENTAL_AI)
	{
		int cardNumber = (allChoises[turn] % 12) / 3;
		int cardColorNumber = allChoises[turn] / 12;
		if(lastRealPlayer == 1)
		{
			colorCredibility[0][cardColorNumber]++;
			valueCredibility[0][cardNumber]++;
			card[allChoises[turn]] = PLAYER_1_HAND_HIDDEN;
			activePlayerID = 2;
			lastRealPlayer = 2;
		} else {
			colorCredibility[1][cardColorNumber]++;
			valueCredibility[1][cardNumber]++;
			card[allChoises[turn]] = PLAYER_2_HAND_HIDDEN;
			activePlayerID = 1;
			lastRealPlayer = 1;
		}

		UpdateProbCard(lastRealPlayer, allChoises[turn]);
		if(allChoises.size() == 1)
		{
			isGameOver = true;
			UpdateGameStat();
			return true;
		}
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
			/* BUG RESOLVED
			for(int loop1 = cardColorNumber * CARD_ONE_COLOR_AMOUNT; loop1 < (cardColorNumber + 1) * CARD_ONE_COLOR_AMOUNT; loop1++)
			{
				if(discardPileTopCardCode[cardColorNumber] + 1 == card[loop1] && cardID != loop1)
				{
					printf("cosestalo");
					CountPlayerChoises(0);
				}
			} BUG RESOLVED
			*/
			card[cardID] = discardPileTopCardCode[cardColorNumber] + 1;
		}
		UpdateProbCard(activePlayerID, cardID);

		// Phase 2 - drawing card
		if(drawFrom[turn] == DRAW_FROM_DECK) // draw from deck
		{
			activePlayerID = 0; // environment plays
		} else {
			if(activePlayerID == 1)
			{
				card[discardPileTopCardID[drawFrom[turn]]] = PLAYER_1_HAND_KNOWN;
				UpdateProbCard(activePlayerID, discardPileTopCardID[drawFrom[turn]]);
				activePlayerID = 2;
				lastRealPlayer = 2;
			} else {
				card[discardPileTopCardID[drawFrom[turn]]] = PLAYER_2_HAND_KNOWN;
				UpdateProbCard(activePlayerID, discardPileTopCardID[drawFrom[turn]]);
				activePlayerID = 1;
				lastRealPlayer = 1;
			}
		}
	}
	
	int temp = GetInDeckCount();
	if(temp == cardsInDeck)
	{
		cardsInDeckTurn++;
	} else {
		cardsInDeck = temp;
		cardsInDeckTurn = 0;
	}

	whoAskIDlast = NOBODY;

	UpdateGameStat();

	return false;
}

void LostCitiesState::UpdateProbCard(int playerID, int cardPlayedID)
{
	int color = cardPlayedID / CARD_ONE_COLOR_AMOUNT;
	int kind = cardPlayedID % CARD_ONE_COLOR_AMOUNT;
	int probID = playerID - 1;
	int colorStart = color * CARD_ONE_COLOR_AMOUNT;
	int colorEnd = colorStart + CARD_ONE_COLOR_AMOUNT;
	int handHidden;
	if(playerID == 1)
	{
		handHidden = PLAYER_1_HAND_HIDDEN;
	} else {
		handHidden = PLAYER_2_HAND_HIDDEN;
	}


	switch(card[cardPlayedID])
	{
		case PLAYER_1_ON_DESK :
		case PLAYER_2_ON_DESK :
			for(int loop1 = colorStart; loop1 < colorEnd; loop1++)
			{
				if(loop1 < cardPlayedID)
				{
					probCard[probID][loop1] = ZERO_PROB;
				}
				else
				{
					probCard[probID][loop1] += 0.1;
				}
			}
			break;

		case PLAYER_1_HAND_HIDDEN :
		case PLAYER_2_HAND_HIDDEN :
			for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
			{
				probCard[probID][loop1] += 1.0f / 60.0f;
			}
			break;

		case PLAYER_1_HAND_KNOWN :
		case PLAYER_2_HAND_KNOWN :
			for(int loop1 = colorStart; loop1 < colorEnd; loop1++)
			{
				probCard[probID][loop1] += 0.05;
			}
			break;

		default : // he discarded card
			for(int loop1 = colorStart; loop1 < cardPlayedID; loop1++)
			{
				probCard[probID][loop1] = ZERO_PROB;
			}
			break;
	}

	float probSum = ZERO_PROB;
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		if(card[loop1] != handHidden && card[loop1] != IN_DECK)
		{
			probCard[probID][loop1] = ZERO_PROB;
		} else
			probSum += probCard[probID][loop1];
	}

	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		probCard[probID][loop1] /= probSum;
	}

	int a = 2 + 3;

}

int LostCitiesState::GetTurnID(int playCardID, int drawSite)
{
	WhoAsked(1);
	int allChoisesID = -1;
	for(int loop1 = 0; loop1 < allChoises.size(); loop1++)
	{
		if(allChoises[loop1] == playCardID && drawFrom[loop1] == drawSite)
		{
			allChoisesID = loop1;
			break;
		}
	}

	if(allChoisesID < 0)
	{
		allChoises.push_back(playCardID);
		drawFrom.push_back(drawSite);
		allChoisesID = allChoises.size() - 1;
	}

	return allChoisesID;
}
void LostCitiesState::CountPlayerChoises(int whoAskID)
{
	allChoises.clear();
	drawFrom.clear();

	if(activePlayerID == ENVIRONMENTAL_AI)
	{
		for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
		{
			if(card[loop1] == IN_DECK)
			{
				allChoises.push_back(loop1);
			}
		}
	} else {
		int playerHandKnown;		// known
		int playerHandHidden;		// hidden
		int playerOnDesk;
		int enemyID;
		if(activePlayerID == 1)
		{
			enemyID = 2;
			playerHandKnown = PLAYER_1_HAND_KNOWN;
			playerHandHidden = PLAYER_1_HAND_HIDDEN;
			playerOnDesk = PLAYER_1_ON_DESK;
		} else {
			enemyID = 1;
			playerHandKnown = PLAYER_2_HAND_KNOWN;
			playerHandHidden = PLAYER_2_HAND_HIDDEN;
			playerOnDesk = PLAYER_2_ON_DESK;
		}

		int highestCardInExpedition;
		bool domDiscarded;
		bool domPlayed;
		bool domTakeDiscard[COLOR_AMOUNT];

		int inDeckCount = GetInDeckCount();
		for(char loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
		{
			bool canAddCardToExpedition = true;
			discardPileTopCardCode[loop1] = ON_DESK - 1;
			discardPileTopCardID[loop1] = -1;
			highestCardInExpedition = 0;
			domDiscarded = false;
			domPlayed = false;
			domTakeDiscard[loop1] = false;
			for(char loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 3; loop2--)
			{
				char cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;

				if(card[cardID] == PLAYER_1_ON_DESK || card[cardID] == PLAYER_2_ON_DESK)
					domTakeDiscard[loop1] = true;

				if(card[cardID] == playerOnDesk && loop2 > highestCardInExpedition) // you can't add lower value card to existing expedition
				{
					highestCardInExpedition = loop2;
				}
			}
			for(char loop2 = 0; loop2 < CARD_ONE_COLOR_AMOUNT; loop2++)
			{
				char cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;
				if(card[cardID] == playerHandKnown || card[cardID] == playerHandHidden)
				{
					if(!domDiscarded && domination && loop2 > 7 && allChoises.size() > 0)
						domDiscarded = true;

					if(!domDiscarded || !domination)
					{
						allChoises.push_back(cardID);	// can discard card
						drawFrom.push_back(DRAW_FROM_DECK);
						domDiscarded = true;
					}

					if(loop2 >= highestCardInExpedition && (!domPlayed || !domination))
					{
						allChoises.push_back(cardID + DISCARD_CARD_OFFSET); // may add to expedition
						drawFrom.push_back(DRAW_FROM_DECK);
						if(loop2 > 2 && inDeckCount > 5)
							domPlayed = true;
					}
				}  else if(card[cardID] >= ON_DESK)
				{
					if(discardPileTopCardCode[loop1] < card[cardID])
					{
						discardPileTopCardCode[loop1] = card[cardID];
						discardPileTopCardID[loop1] = cardID;
					}
				}
			}
		}

		int oldChoises = allChoises.size();
		if(oldChoises == 0)
			oldChoises++;
		if(cardsInDeckTurn < cardsInDeckMaxTurn)
		{
			for(int loop1 = 0; loop1 < oldChoises; loop1++)
			{
				for(int loop2 = 0; loop2 < COLOR_AMOUNT; loop2++)
				{
					bool domIf = !domination || domTakeDiscard[loop2];
					// if there is card on discard pile and if you dont play card of same color on discard pile, you can draw from thad pile
					if(domIf && discardPileTopCardID[loop2] >= 0 && (allChoises[loop1] >= DISCARD_CARD_OFFSET || loop2 != allChoises[loop1] / CARD_ONE_COLOR_AMOUNT))
					{
						allChoises.push_back(allChoises[loop1]);
						drawFrom.push_back(loop2);
					}
				}
			}
		}
	} 
}

void LostCitiesState::WhoAsked(int whoAskID)
{
	if(whoAskID != whoAskIDlast)
	{
		whoAskIDlast = whoAskID;
		CountPlayerChoises(whoAskID);
	}
}

int LostCitiesState::GetPlayerChoises(int whoAskID)
{
	WhoAsked(whoAskID);
	return allChoises.size();
}
int LostCitiesState::GetActivePlayerID() const
{
	return activePlayerID;
}

int LostCitiesState::CountRanks(char * c, int playerID, int whoAskID)
{
	int score[PLAYER_AMOUNT];
	int handKnown[PLAYER_AMOUNT];
	int handHidden[PLAYER_AMOUNT];
	int discardTop;
	int discardTopValue;
	int discard[PLAYER_AMOUNT];
	int deck[PLAYER_AMOUNT];

	int inDeckCards = 0;
	int expedition[PLAYER_AMOUNT];
	bool cExpedition[PLAYER_AMOUNT];
	int cScore[PLAYER_AMOUNT];
	int cBonus[PLAYER_AMOUNT];
	int cOnDeskCount[PLAYER_AMOUNT];
	int cHandKnown[PLAYER_AMOUNT];
	int cHandHidden[PLAYER_AMOUNT];
	int cDeck[PLAYER_AMOUNT];			// same for both players
	int cTopValue[PLAYER_AMOUNT];
	bool cCanBePlayed[PLAYER_AMOUNT];   // you cant play 3 if you have 4 on table

	for(int loop1 = 0; loop1 < PLAYER_AMOUNT; loop1++)
	{
		score[loop1] = 0;
		handKnown[loop1] = 0;
		handHidden[loop1] = 0;
		discard[loop1] = 0;
		deck[loop1] = 0;
		expedition[loop1] = 0;
	}

	for(int loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
	{
		discardTop = ON_DESK - 1;
		discardTopValue = 0;


		for(int loop2 = 0; loop2 < PLAYER_AMOUNT; loop2++)
		{
			cDeck[loop2] = 0;
			cScore[loop2] = 0;
			cBonus[loop2] = 1; 
			cOnDeskCount[loop2] = 0;
			cHandKnown[loop2] = 0;
			cHandHidden[loop2] = 0;
			cTopValue[loop2] = 0;
			cCanBePlayed[loop2] = true;
			cExpedition[loop2] = false;
		}

		for(int loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 0; loop2--)
		{
			int cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;

			if(loop2 > 2)
			{
				int cardValue = loop2 - 1;
				switch(c[cardID])
				{
					case IN_DECK : 
						inDeckCards++;
						if(cCanBePlayed[0])
							cDeck[0] += cardValue;
						if(cCanBePlayed[1])
							cDeck[1] += cardValue;
						break;

					case PLAYER_1_HAND_KNOWN :
						if(cCanBePlayed[0])
							cHandKnown[0] += cardValue;
						break;

					case PLAYER_1_HAND_HIDDEN :
						if(cCanBePlayed[0])
							cHandHidden[0] += cardValue;
						break;

					case PLAYER_1_ON_DESK :
						cOnDeskCount[0]++;
						cScore[0] += cardValue;
						cExpedition[0] = true;

						if(cCanBePlayed[0])
						{
							cCanBePlayed[0] = false;
							cTopValue[0] = loop2;
						}
						break;

					case PLAYER_2_HAND_KNOWN :
						if(cCanBePlayed[1])
							cHandKnown[1] += cardValue;
						break;

					case PLAYER_2_HAND_HIDDEN :
						if(cCanBePlayed[1])
							cHandHidden[1] += cardValue;
						break;

					case PLAYER_2_ON_DESK :
						cOnDeskCount[1]++;
						cScore[1] += cardValue;
						
						if(cCanBePlayed[1])
						{
							cCanBePlayed[1] = false;
							cTopValue[1] = loop2;
						}
						break;

					default :
						if(c[cardID] > discardTop)
						{
							discardTop = c[cardID];
							discardTopValue = cardValue;
						}
						break;
				}
			} else { // bonuses
				if(c[cardID] == PLAYER_1_ON_DESK)
				{
					cOnDeskCount[0]++;
					cBonus[0]++;
				} else if(c[cardID] == PLAYER_2_ON_DESK)
				{
					cOnDeskCount[1]++;
					cBonus[1]++;
				} else if(c[cardID] >= ON_DESK && c[cardID] > discardTop)
				{
					discardTop = c[cardID];
					discardTopValue = 0;
				}
			}
		}

		for(int loop2 = 0; loop2 < 2; loop2++)
		{
			if(cOnDeskCount[loop2] > 0)
			{
				score[loop2] += (cScore[loop2] - 20) * cBonus[loop2];
				if(cOnDeskCount[loop2] >= 8)
					score[loop2] += 20;

				handHidden[loop2] += cHandHidden[loop2];
				handKnown[loop2] += cHandKnown[loop2];
				deck[loop2] += cDeck[loop2];
				if(cTopValue[loop2] <= discardTopValue)
					discard[loop2] += discardTopValue;

				if(cExpedition[loop2])
					expedition[loop2]++;
			}
		}
	}

	int rank[PLAYER_AMOUNT];
	
	for(int loop2 = 0; loop2 < 2; loop2++)
	{
		int minus = 0;
		int predictedDeck = (expedition[loop2] > 3) ? ((expedition[loop2] > 4) ? 0 : PREDICTED_DECK_1) : PREDICTED_DECK_2;
		
		int multTemp = min(PREDICTED_IN_HAND, inDeckCards * 2);
		float mult = multTemp / (float) PREDICTED_IN_HAND;
		mult = 1.0;

		if(inDeckCards == 0)
			rank[loop2] = (int) (score[loop2] * PREDICTED_ON_BOARD);
		else
			rank[loop2] = (int) (
			          handKnown[loop2] * PREDICTED_IN_HAND * mult + 
					  handHidden[loop2] * PREDICTED_IN_HAND * mult +
					  score[loop2] * PREDICTED_ON_BOARD + 
					  discard[loop2] * PREDICTED_TOP_DISCARD * mult +
					  deck[loop2] * predictedDeck
					  );
	}

	int opponentID = (playerID == 1) ? 2 : 1;
	return rank[playerID - 1] - rank[opponentID - 1];
}

int LostCitiesState::GetPlayerRank(int playerID, int whoAskID)
{
	return CountRanks(playerID, whoAskID);
	/*
	int opponentID = (playerID == 1) ? 2 : 1;

	int me = GetPositivePlayerRank(playerID, whoAskID);
	int him = GetPositivePlayerRank(opponentID, whoAskID);
	return me - him;*/
}

int LostCitiesState::GetPlayerStatus(int playerID)
{
	return GetPlayerRank(playerID, 0);
}

int LostCitiesState::GetPositivePlayerRank(int playerID, int whoAskID)
{
	int score = 0;
	int handKnown;
	int handHidden;
	int onDesk;
	int handKnownOther;
	int handHiddenOther;
	int onDeskOther;

	switch(playerID)
	{
		case 1 :
			onDesk = PLAYER_1_ON_DESK;
			handKnown = PLAYER_1_HAND_KNOWN;
			handHidden = PLAYER_1_HAND_HIDDEN;
			onDeskOther = PLAYER_2_ON_DESK;
			handKnownOther = PLAYER_2_HAND_KNOWN;
			handHiddenOther = PLAYER_2_HAND_HIDDEN;
			break;
		case 2 :
			onDesk = PLAYER_2_ON_DESK;
			handKnown = PLAYER_2_HAND_KNOWN;
			handHidden = PLAYER_2_HAND_HIDDEN;
			onDeskOther = PLAYER_1_ON_DESK;
			handKnownOther = PLAYER_1_HAND_KNOWN;
			handHiddenOther = PLAYER_1_HAND_HIDDEN;
			break;
	}

	bool expeditionExist = false;
	int predictedRank = 0;
	int topCardOrder = ON_DESK;
	int topCardId = 0;
	int deckRank = 0;
	int bonuses;
	int cardType;
	int handUnknownCount = 0;
	int inDeckCount = 0;
	int scoreNotExpeditionKnown = 0;
	int scoreNotExpeditionUnknown = 0;
	int scoreNotExpeditionUnknownDeckCount = 0;
	int scoreNotExpeditionUnknownHandCount = 0;
	int deckRankColor;
	int handUnknownCountColor; 
	int inDeckCountColor;
	int maxTurnToTheEnd = 0;
	int knownCards = CARD_AMOUNT;
	int realHandSize = 0;
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		if(card[loop1] == IN_DECK)
			maxTurnToTheEnd++;

		if(card[loop1] == handHiddenOther ||
		   card[loop1] == IN_DECK)
		{
			knownCards--;
		}

		if(card[loop1] == handKnown ||
		   card[loop1] == handHidden)
		{
			realHandSize++;
		}
	}
	for(int loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
	{
		expeditionExist = false;
		predictedRank = 0;
		topCardOrder = ON_DESK - 1;
		topCardId = 0;
		bonuses = 1;
		deckRankColor = 0;
		handUnknownCountColor = 0;
		inDeckCountColor = 0;

		for(int loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 0; loop2--)
		{
			int cardID = loop2 + loop1 * CARD_ONE_COLOR_AMOUNT;
			cardType = cardID % CARD_ONE_COLOR_AMOUNT;

			if(card[cardID] == onDesk)
			{
				expeditionExist = true;
				
				for(int loop3 = 0; loop3 < 3; loop3++)
				{
					if(card[loop1 * CARD_ONE_COLOR_AMOUNT + loop3] == onDesk)
					{
						bonuses++;
					}
				}
				break;
			}
			else
			if(card[cardID] >= ON_DESK)
			{
				if(card[cardID] > topCardOrder)
				{
					topCardOrder = card[cardID];
					topCardId = cardType;
				}
				continue;
			}
			else
			if(card[cardID] == handKnown || 
			  (card[cardID] == handHidden && (whoAskID == playerID || whoAskID == 0)))
			{
				if(cardType > 2)
					predictedRank += (cardType - 1);
			}
			else
			if(card[cardID] == onDeskOther || card[cardID] == handKnownOther ||
				(card[cardID] == handHiddenOther && (whoAskID == playerID || whoAskID == 0)))
			{
				continue;
			}
			else { // card in deck or hidden to player - possibly in deck
				if(card[cardID] == handHidden)
					handUnknownCountColor++;
				else
					inDeckCountColor++;
				if(cardType > 2)
					deckRankColor += (cardType - 1);
			}
		}

		if(expeditionExist)
		{
			predictedRank *= min(PREDICTED_IN_HAND, maxTurnToTheEnd / 2);
			if(topCardOrder >= ON_DESK && topCardId > 2)
			{
				predictedRank += min(PREDICTED_TOP_DISCARD, maxTurnToTheEnd / 2) * (topCardId - 1);
			}
			score += predictedRank * bonuses;
			deckRank += deckRankColor * bonuses;
			handUnknownCount += handUnknownCountColor;
			inDeckCount += inDeckCountColor;
		} else {
			scoreNotExpeditionKnown += predictedRank;
			scoreNotExpeditionUnknown += deckRankColor;
			scoreNotExpeditionUnknownHandCount += handUnknownCountColor;
			scoreNotExpeditionUnknownDeckCount += inDeckCountColor;
		}
	}

	float avgDeckCard = 0.0f;
	if(inDeckCount + handUnknownCount > 0)
		avgDeckCard = deckRank / (float) (inDeckCount + handUnknownCount);

	float scoreNotExpedition = scoreNotExpeditionKnown;
	float avgNotExpedition = 0.0f;
	if(scoreNotExpeditionUnknownHandCount > 0)
	{
		avgNotExpedition = scoreNotExpeditionUnknown / (float) (scoreNotExpeditionUnknownHandCount + scoreNotExpeditionUnknownDeckCount);
		scoreNotExpedition += avgNotExpedition * scoreNotExpeditionUnknownHandCount;
	}

	if(realHandSize < handSize)
	{
		if(avgNotExpedition + avgDeckCard < 2.0)
			scoreNotExpedition += 7;
		else
			scoreNotExpedition += (avgNotExpedition + avgDeckCard) / 2.0f;
	}

	float scoreDeckBonus = avgDeckCard * inDeckCount * min(PREDICTED_DECK_1, maxTurnToTheEnd / 2);
	float scoreHiddenHandBonus = avgDeckCard * handUnknownCount * min(PREDICTED_IN_HAND, maxTurnToTheEnd / 2);
	float scoreReal = GetPlayerPoints(playerID) * PREDICTED_ON_BOARD;
	scoreNotExpedition = min(10, maxTurnToTheEnd) / 10.0f * scoreNotExpedition;
	return score + /*scoreNotExpedition*/ +scoreDeckBonus + scoreHiddenHandBonus + scoreReal;
}

int LostCitiesState::GetPlayerPoints(int playerID)
{
	int score = 0;
	int onDesk;
	switch(playerID)
	{
		case 1 :
			onDesk = PLAYER_1_ON_DESK;
			break;
		case 2 :
			onDesk = PLAYER_2_ON_DESK;
			break;
	}

	bool firstOfColorOnDesk = 0;
	int premiumBonus = 0;
	int cardBonus = 0;
	int journey = 0;
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		if(loop1 % CARD_ONE_COLOR_AMOUNT == 0)
		{
			firstOfColorOnDesk = true;
			cardBonus = 0;
			premiumBonus = 1;
			journey = 0;
		}

		if(card[loop1] == onDesk)
		{
			if(firstOfColorOnDesk)
			{
				journey -= 20;
				firstOfColorOnDesk = false;
			}
			cardBonus++;
			int cardType = loop1 % CARD_ONE_COLOR_AMOUNT;

			if(cardType < 3)
				premiumBonus++;
			else
				journey += cardType - 1;
		}

		if(loop1 % CARD_ONE_COLOR_AMOUNT == CARD_ONE_COLOR_AMOUNT - 1)
		{
			journey *= premiumBonus;
			if(cardBonus >= 8)
				journey += 20;
			score += journey;
		}
	}
	return score;
}

int LostCitiesState::GetInDeckCount()
{
	int cardNumber = 0;
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
		if(card[loop1] == IN_DECK)
			cardNumber++;
	return cardNumber;
}

IGameState ** LostCitiesState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	WhoAsked(whoAskID);
	int numberNextStates = GetPlayerChoises(whoAskID);
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

IGameState * LostCitiesState::SimulateToTheEnd(int whoAskID)
{
	LostCitiesState * state = new LostCitiesState(*this);

	while(!state->IsGameOver())
	{
		state->MakeTurn(rand() % GetPlayerChoises(whoAskID));
	}
	
	return state;
}

IGameState * LostCitiesState::GetRandomNextState(int whoAskID, int * outStateID)
{
	WhoAsked(whoAskID);
	int numberNextStates = GetPlayerChoises(whoAskID);
	int turn;

	turn = rand() % numberNextStates;
	
	LostCitiesState * lostCitiesState = new LostCitiesState(*this);
	lostCitiesState->MakeTurn(turn);

	*outStateID = turn;
	return lostCitiesState;
}

void LostCitiesState::ChangeCardInsideInformSet(int whoAskID)
{
	char hiddenHand;
	if(whoAskID == 1)
	{
		hiddenHand = PLAYER_2_HAND_HIDDEN;
	} else {
		hiddenHand = PLAYER_1_HAND_HIDDEN;
	}

	vector<char> unknownCards;
	vector<float> probCards;

	int hiddenAmount = 0;
	for(char loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		if(card[loop1] == IN_DECK || card[loop1] == hiddenHand)
		{
			if(card[loop1] == hiddenHand)
			{
				hiddenAmount++;
			}
			unknownCards.push_back(loop1);
			probCards.push_back(probCard[whoAskID - 1][loop1]);
			card[loop1] = IN_DECK;
		}
	}

	float rndNumber;
	int cardToHandID = -1;
	float probSum;
	float probAll = 1.0;
	for(int loop1 = 0; loop1 < hiddenAmount; loop1++)
	{
		rndNumber = rand() / (float) RAND_MAX;
		probSum = 0.0f;
		cardToHandID = probCards.size() - 1;
		for(int loop2 = 0; loop2 < probCards.size(); loop2++)
		{
			probSum += probCards[loop2] / probAll;
			if(rndNumber <= probSum)
			{
				cardToHandID = loop2;
				break;
			}
		}

		card[unknownCards[cardToHandID]] = hiddenHand;
		probAll -= probCards[cardToHandID];
		probCards.erase(probCards.begin() + cardToHandID, probCards.begin() + cardToHandID + 1);
		unknownCards.erase(unknownCards.begin() + cardToHandID, unknownCards.begin() + cardToHandID + 1);
	}
}

float LostCitiesState::GetCredibility()
{
	float credSum = 0.0f;
	for(int loop1 = 0; loop1 < PLAYER_AMOUNT; loop1++)
	{
		credSum += Utility::Inst()->Credibility(colorCredibility[loop1], COLOR_AMOUNT) / 2;
		credSum += Utility::Inst()->Credibility(valueCredibility[loop1], VALUE_KINDS);
	}

	return credSum;
}

IGameState * LostCitiesState::GetStateFromSameInformSet(int whoAskID)
{
	LostCitiesState * result = new LostCitiesState(*this);

	result->ChangeCardInsideInformSet(whoAskID);

	return result;
}

ISpecificStat * LostCitiesState::GetGameSpecificStat()
{
	return NULL;
}