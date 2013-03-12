
#include <algorithm>
#include <ctime>
#include "LostCitiesState.h"

using namespace std;

#define ZERO_PROB 0.0f;

LostCitiesState::LostCitiesState(int _handSize, bool _abstraction)
{
	InitGame(_handSize, _abstraction);
}

void LostCitiesState::InitGame(int _handSize, bool _abstraction)
{
	handSize = _handSize;
	activePlayerID = 1;
	lastRealPlayer = 1;
	cardsInDeckMaxTurn = 4;
	abstraction = _abstraction;

	int allCards[CARD_AMOUNT];
	for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
	{
		card[loop1] = IN_DECK;
		allCards[loop1] = loop1;

		probCard[0][loop1] = 1.0f / 60.0f;
		probCard[1][loop1] = 1.0f / 60.0f;
	}
	
	random_shuffle(allCards, allCards + CARD_AMOUNT);
	
	for(int loop1 = 0; loop1 < handSize; loop1++)
		card[allCards[loop1]] = PLAYER_1_HAND_HIDDEN;
	for(int loop1 = handSize; loop1 < 2 * handSize; loop1++)
		card[allCards[loop1]] = PLAYER_2_HAND_HIDDEN;

	cardsInDeck = GetInDeckCount();
	cardsInDeckTurn = 0;

	isGameOver = false;
	whoAskIDlast = NOBODY;
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
	abstraction = origin.abstraction;
	allChoises = origin.allChoises;
	drawFrom = origin.drawFrom;

	for(int loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
	{
		discardPileTopCardCode[loop1] = origin.discardPileTopCardCode[loop1];
		discardPileTopCardID[loop1] = origin.discardPileTopCardID[loop1];
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
		if(lastRealPlayer == 1)
		{
			card[allChoises[turn]] = PLAYER_1_HAND_HIDDEN;
			activePlayerID = 2;
			lastRealPlayer = 2;
		} else {
			card[allChoises[turn]] = PLAYER_2_HAND_HIDDEN;
			activePlayerID = 1;
			lastRealPlayer = 1;
		}

		UpdateProbCard(lastRealPlayer, allChoises[turn]);
		if(allChoises.size() == 1)
		{
			isGameOver = true;
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
	int allChoisesID = 0;
	for(int loop1 = 0; loop1 < allChoises.size(); loop1++)
	{
		if(allChoises[loop1] == playCardID && drawFrom[loop1] == drawSite)
		{
			allChoisesID = loop1;
			break;
		}
	}

	return allChoisesID;
}
void LostCitiesState::CountPlayerChoises(int whoAskID)
{
	allChoises.clear();
	drawFrom.clear();

	if(activePlayerID == ENVIRONMENTAL_AI)
	{
		vector<char> helpArray;
		for(int loop1 = 0; loop1 < CARD_AMOUNT; loop1++)
		{
			if(card[loop1] == IN_DECK)
			{
				helpArray.push_back(loop1);
			}

			if(loop1 % 4 == 3)
			{
				if(abstraction)
				{
					if(helpArray.size() > 0)
					{
						int rndNumber = rand() % helpArray.size();
						allChoises.push_back(helpArray[rndNumber]);
					}
				} else {
					allChoises.insert(allChoises.begin(), helpArray.begin(), helpArray.end());
				}
				helpArray.clear();
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

		for(char loop1 = 0; loop1 < COLOR_AMOUNT; loop1++)
		{
			bool canAddCardToExpedition = true;
			discardPileTopCardCode[loop1] = ON_DESK - 1;
			discardPileTopCardID[loop1] = -1;
			for(char loop2 = CARD_ONE_COLOR_AMOUNT - 1; loop2 >= 0; loop2--)
			{
				char cardID = loop1 * CARD_ONE_COLOR_AMOUNT + loop2;
				if(card[cardID] == playerHandKnown || card[cardID] == playerHandHidden)
				{

					allChoises.push_back(cardID);	// can discard card
					drawFrom.push_back(DRAW_FROM_DECK);

					if(canAddCardToExpedition)
					{
						allChoises.push_back(cardID + DISCARD_CARD_OFFSET); // may add to expedition
						drawFrom.push_back(DRAW_FROM_DECK);
					}
				} else if(canAddCardToExpedition && card[cardID] == playerOnDesk && loop2 > 2) // you can't add lower value card to existing expedition
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

		int oldChoises = allChoises.size();
		if(cardsInDeckTurn < cardsInDeckMaxTurn)
		{
			for(int loop1 = 0; loop1 < oldChoises; loop1++)
			{
				for(int loop2 = 0; loop2 < COLOR_AMOUNT; loop2++)
				{
					// if there is card on discard pile and if you dont play card of same color on discard pile, you can draw from thad pile
					if(discardPileTopCardID[loop2] >= 0 && (allChoises[loop1] >= DISCARD_CARD_OFFSET || loop2 != allChoises[loop1] / CARD_ONE_COLOR_AMOUNT))
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
int LostCitiesState::GetPlayerRank(int playerID, int whoAskID)
{
	int opponentID = (playerID == 1) ? 2 : 1;

	int me = GetPositivePlayerRank(playerID, whoAskID);
	int him = GetPositivePlayerRank(opponentID, whoAskID);
	return me - him;
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

	float scoreDeckBonus = avgDeckCard * inDeckCount * min(PREDICTED_DECK, maxTurnToTheEnd / 2);
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

	float rndNumber = rand() / (float) RAND_MAX;
	int cardToHandID = -1;
	float probSum;
	float probAll = 1.0;
	for(int loop1 = 0; loop1 < hiddenAmount; loop1++)
	{
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