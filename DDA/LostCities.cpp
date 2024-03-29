#include <Qt\qcheckbox.h>
#include "LostCities.h"
#include "EnvironmentAIBasic.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimberIS.h"
#include "Human.h"

LostCities::LostCities(QWidget * _widget, bool _paint) : Game(_widget, _paint)
{
	cardWidth = 80;
	cardHeight = 80;
	
	for(int loop1 = 0; loop1 < CLICKABLE_AREAS; loop1++)
	{
		clickableArea[loop1].SetCardID(-1);
		clickableArea[loop1].SetSize(cardWidth, cardHeight);
	}

	for(int loop1 = 0; loop1 < HAND_SIZE; loop1++)
	{
		clickableArea[loop1].MoveTo(cardWidth / 4 + loop1 * (cardWidth / 2), 600 - ((cardHeight * 4) / 3));
	}

	for(int loop1 = 0; loop1 < LostCitiesState::COLOR_AMOUNT; loop1++)
	{
		clickableArea[loop1 + ClickableArea::EXPEDITION].MoveTo(cardWidth / 4 + loop1 * (cardWidth + 20), 290);
		clickableArea[loop1 + ClickableArea::DISCARD].MoveTo(cardWidth / 4 + loop1 * (cardWidth + 20), 200);
	}
	clickableArea[ClickableArea::DECK].MoveTo(600 - cardWidth * 4 / 3, 600 - (cardHeight * 4 / 3));
				
	for(int loop1 = 0; loop1 < CLICKABLE_MAX_ACTIVE; loop1++)
	{
		highlightClickableAreas[loop1] = -1;
	}

	minPlayerAI = 2;
	maxPlayerAI = 2;

	player = new IPlayer*[3];

	EnvironmentAI * eai = new EnvironmentAIBasic(0);
	float coefs[COEF_COUNT] = { 100, 1000, 100, 100, 100, 0, 10, 0 };
	eai->SetMetricCoefs(coefs);

	player[0] = eai;
	if(paint)
	{
		player[1] = new Human(1);
	} else {
		player[1] = new PlayerHillClimberIS(1);
	}
	player[2] = new PlayerHillClimberIS(2);

	QObject::connect(player[0], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[1], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[2], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));

	playerCount = 3;
	realHandSize = 8;
	domination = true;
	currentState = new LostCitiesState(realHandSize, domination, this);
	state = STATE_STOPPED;
}


LostCities::~LostCities(void)
{
	if(currentState != NULL)
		delete currentState;
}

bool LostCities::PlayerTurn()
{
	int playerTurn = player[currentState->GetActivePlayerID()]->MakeTurn();

	return currentState->MakeTurn(playerTurn);
}

void LostCities::StartGame()
{
	Game::StartGame();

	if(currentState != NULL)
		delete currentState;
	currentState = new LostCitiesState(realHandSize, domination, this);

	player[0]->StartGame(this);
	player[1]->StartGame(this);
	player[2]->StartGame(this);

	UpdateActiveClickableAres();

	state = STATE_RUNNING;
	if(player[1]->Think())
		NextTurn();
}

void LostCities::UpdateActiveClickableAres()
{
	for(int loop1 = 0; loop1 < CLICKABLE_AREAS; loop1++)
	{
		clickableArea[loop1].SetActive(false);
	}

	if(highlightClickableAreas[CLICKABLE_PLAY_FROM] == -1)
	{
		for(int loop1 = 0; loop1 < realHandSize; loop1++)
		{
			clickableArea[loop1].SetActive(true);
		}
	} else {
		if(highlightClickableAreas[CLICKABLE_PLAY_TO] == -1)
		{
			int cardID1 = clickableArea[highlightClickableAreas[CLICKABLE_PLAY_FROM]].CardID();
			int colorNumber = GetCardColor(cardID1);
			int cardID2 = clickableArea[colorNumber + ClickableArea::EXPEDITION].CardID();
			
			clickableArea[colorNumber + ClickableArea::DISCARD].SetActive(true);
			if(cardID2 <= cardID1 || (cardID1 % LostCitiesState::CARD_ONE_COLOR_AMOUNT < 3 && cardID2 % LostCitiesState::CARD_ONE_COLOR_AMOUNT < 3))
			{
				clickableArea[colorNumber + ClickableArea::EXPEDITION].SetActive(true);
			}
		} else if(highlightClickableAreas[CLICKABLE_DRAW_FROM] == -1) {
			for(int loop1 = ClickableArea::DISCARD; loop1 < ClickableArea::DECK; loop1++)
			{
				if(clickableArea[loop1].CardID() >= 0)
				{
					clickableArea[loop1].SetActive(true);
				}
			}
			clickableArea[ClickableArea::DECK].SetActive(true);
		}
	}
}

void LostCities::Draw(QPainter * painter, int tickMillis)
{
	if(currentState == NULL)
		return;

	int cardHandID = 0;
	int expedition[LostCitiesState::PLAYER_AMOUNT][LostCitiesState::COLOR_AMOUNT];
	for(int loop1 = 0; loop1 < LostCitiesState::PLAYER_AMOUNT; loop1++)
	{
		for(int loop2 = 0; loop2 < LostCitiesState::COLOR_AMOUNT; loop2++)
		{
			expedition[loop1][loop2] = 0;
		}
	}

	int inDeck = 0;
	int cardHand2ID = 0;
	int discardPileMax = 0;
	int discardPile[LostCitiesState::CARD_ONE_COLOR_AMOUNT];

	for(int loop1 = 0; loop1 < CLICKABLE_AREAS; loop1++)
	{
		clickableArea[loop1].SetCardID(-1);
	}

	bool newCollumn;
	for(int loop1 = 0; loop1 < LostCitiesState::CARD_AMOUNT; loop1++)
	{
		int cardNumber = loop1 % 12 + 2;
		int cardColorNumber = loop1 / 12;
		
		int cardSite = currentState->GetCard(loop1);
		
		switch(cardSite)
		{
			case PLAYER_1_HAND_HIDDEN :
			case PLAYER_1_HAND_KNOWN :
				break;

			case PLAYER_2_HAND_HIDDEN :
			case PLAYER_2_HAND_KNOWN :
				break;

			case PLAYER_1_ON_DESK :
				newCollumn = expedition[0][cardColorNumber] >= 6;
				DrawCard(painter, loop1, cardWidth / 4 + cardColorNumber * (20 + cardWidth) + newCollumn * 13, 290 + (cardHeight / 4) * expedition[0][cardColorNumber] - newCollumn * 6 * (cardHeight / 4));
				
				clickableArea[ClickableArea::EXPEDITION + cardColorNumber].SetCardID(loop1);

				expedition[0][cardColorNumber]++;
				break;
			case PLAYER_2_ON_DESK :
				newCollumn = expedition[1][cardColorNumber] >= 6;
				DrawCard(painter, loop1, cardWidth / 4 + cardColorNumber * (20 + cardWidth) - newCollumn * 13, 110 - (cardHeight / 4) * expedition[1][cardColorNumber] + newCollumn * 6 * (cardHeight / 4));
				expedition[1][cardColorNumber]++;
				break;
			case IN_DECK :
				inDeck++;
				break;
			default :
				discardPile[cardSite-ON_DESK] = loop1;
				discardPileMax++;
				break;
		}	

		painter->setBrush(Qt::gray);
		painter->drawRoundRect(cardWidth / 4 + cardColorNumber * (20 + cardWidth), 200, cardWidth, cardHeight);

		if(loop1 % LostCitiesState::CARD_ONE_COLOR_AMOUNT == LostCitiesState::CARD_ONE_COLOR_AMOUNT - 1)
		{
			if(discardPileMax == 0)
				clickableArea[ClickableArea::DISCARD + cardColorNumber].SetCardID(-1);
			else
				clickableArea[ClickableArea::DISCARD + cardColorNumber].SetCardID(discardPile[discardPileMax - 1]);
			for(int loop2 = 0; loop2 < discardPileMax; loop2++)
			{
				DrawCard(painter, discardPile[loop2], cardWidth / 4 + cardColorNumber * (20 + cardWidth), 200);
			}
			discardPileMax = 0;
		}
	}
	
	for(int loop1 = 0; loop1 < LostCitiesState::CARD_AMOUNT; loop1++)
	{
		int cardNumber = loop1 % 12 + 2;
		int cardColorNumber = loop1 / 12;
		
		int cardSite = currentState->GetCard(loop1);
		
		switch(cardSite)
		{
			case PLAYER_1_HAND_HIDDEN :
			case PLAYER_1_HAND_KNOWN :
				DrawCard(painter, loop1, clickableArea[cardHandID].X(), clickableArea[cardHandID].Y());
				clickableArea[cardHandID].SetCardID(loop1);
				cardHandID++;
				break;

			case PLAYER_2_HAND_HIDDEN :
			case PLAYER_2_HAND_KNOWN : 
				//DrawCard(painter, loop1, cardHand2ID * 30 + 500, 100);
				cardHand2ID++;
				break;
		}	
	}

	for(int loop2 = 0; loop2 < LostCitiesState::COLOR_AMOUNT; loop2++)
	{
		int expeditionHeight = expedition[0][loop2]- 1;
		if(expeditionHeight > 5)
			expeditionHeight = 5;
		clickableArea[ClickableArea::EXPEDITION + loop2].SetHeight((cardHeight / 4) * expeditionHeight + cardHeight);
	}

	char cardInDeckString[24];
	DrawCard(painter, 61, clickableArea[ClickableArea::DECK].X(), clickableArea[ClickableArea::DECK].Y());
	sprintf(cardInDeckString, "%d", inDeck);
	painter->drawText(clickableArea[ClickableArea::DECK].X() + 5, clickableArea[ClickableArea::DECK].Y() + 5, 500, 500, 0, cardInDeckString);


	if(highlightClickableAreas[CLICKABLE_HOVER] >= 0 && highlightClickableAreas[CLICKABLE_HOVER] < realHandSize)
		DrawCard(painter, clickableArea[highlightClickableAreas[CLICKABLE_HOVER]].CardID(), clickableArea[highlightClickableAreas[CLICKABLE_HOVER]].X(), clickableArea[highlightClickableAreas[CLICKABLE_HOVER]].Y());

	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->setPen(Qt::lightGray);
	for(int loop1 = ClickableArea::EXPEDITION; loop1 < CLICKABLE_AREAS; loop1++)
	{
		if(clickableArea[loop1].Active())
		{
			painter->drawRoundRect(clickableArea[loop1].Area());
		}
	}

	painter->setPen(Qt::darkMagenta);
	for(int loop1 = 0; loop1 < CLICKABLE_MAX_ACTIVE; loop1++)
	{
		if(highlightClickableAreas[loop1] >= 0)
			painter->drawRoundRect(clickableArea[highlightClickableAreas[loop1]].Area());
		painter->setPen(Qt::lightGray);
	}

	painter->setPen(Qt::darkMagenta);
	painter->setFont(QFont("SansSerif", 20, 3, false));
	char temp[25];
	sprintf(temp, "%d", currentState->GetPlayerPoints(1));
	painter->drawText(525, 280, 500, 500, 0, temp);
	sprintf(temp, "%d", currentState->GetPlayerPoints(2));
	painter->drawText(525, 155, 500, 500, 0, temp);

	/*
	painter->setPen(Qt::black);
	painter->setFont(QFont("SansSerif", 15, 3, false));
	sprintf(temp, "%4d %4d Env Sees 1", currentState->GetPlayerRank(1, 0), currentState->CountRanks(1, 0));
	painter->drawText(510, 330, 500, 500, 0, temp);
	sprintf(temp, "%4d %4d 1 seems himself", currentState->GetPlayerRank(1, 1), currentState->CountRanks(1, 1));
	painter->drawText(510, 350, 500, 500, 0, temp);
	sprintf(temp, "%4d %4d 2 seams 1", currentState->GetPlayerRank(1, 2), currentState->CountRanks(1, 2));
	painter->drawText(510, 370, 500, 500, 0, temp);
	*/
}

void LostCities::DrawCard(QPainter * painter, int cardID, int x, int y)
{
	int cardNumber = cardID % 12;
	int cardColorNumber = cardID / 12;
	QColor cardColor;
	switch(cardColorNumber)
	{
		case 0 :
			cardColor = Qt::yellow;
			break;
		case 1 :
			cardColor = QColor(125,125,255);
			break;
		case 2 :
			cardColor = Qt::white;
			break;
		case 3 :
			cardColor = Qt::green;
			break;
		case 4 :
			cardColor = Qt::red;
			break;
		default :
			cardColor = Qt::darkGray;
			break;
	}

	char symbolString[3];
	symbolString[1] = 0;
	if(cardNumber == 11)
	{
		symbolString[0] = '1';
		symbolString[1] = '0';
		symbolString[2] = 0;
	} else
	if(cardNumber > 2)
		symbolString[0] = (cardNumber - 1) + '0';
	else
		symbolString[0] = '$';

	painter->setBrush(cardColor);
	painter->drawRoundRect(x, y, cardWidth, cardHeight);
	if(cardID < LostCitiesState::CARD_AMOUNT)
	{
		painter->drawText(x + 5, y + 5, cardWidth, cardHeight, 0, symbolString);
		if(symbolString[1] == '0')
			painter->drawText(x + cardWidth - 15, y + cardHeight - 15, cardWidth, cardHeight, 0, symbolString);
		else
			painter->drawText(x + cardWidth - 10, y + cardHeight - 15, cardWidth, cardHeight, 0, symbolString);
	}
}

IGameState * LostCities::GetCurrentState() const
{
	return currentState;
}
void LostCities::MouseMoveEvent ( int xMouse, int yMouse )
{
	if(state != STATE_RUNNING)
		return;

	highlightClickableAreas[CLICKABLE_HOVER] = -1;
	for(int loop1 = 0; loop1 < CLICKABLE_AREAS; loop1++)
	{
		if(loop1 >= realHandSize && loop1 < HAND_SIZE)
		{
			continue;
		}
		if(clickableArea[loop1].Area().contains(xMouse, yMouse))
		{
			highlightClickableAreas[CLICKABLE_HOVER] = loop1;
		}
	}
}
void LostCities::MousePressEvent ( int xMouse, int yMouse )
{
	if(state != STATE_RUNNING)
		return;

	int drawSite = -1;
	for(int loop1 = CLICKABLE_AREAS - 1; loop1 >= 0; loop1--)
	{
		if(clickableArea[loop1].Area().contains(xMouse, yMouse))
		{
			if(loop1 < realHandSize)
			{
				if(highlightClickableAreas[CLICKABLE_PLAY_FROM] == loop1)
				{
					highlightClickableAreas[CLICKABLE_PLAY_FROM] = -1;
					highlightClickableAreas[CLICKABLE_PLAY_TO] = -1;
					highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
				}
				else {
					highlightClickableAreas[CLICKABLE_PLAY_FROM] = loop1;
					highlightClickableAreas[CLICKABLE_PLAY_TO] = -1;
					highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
				}
			} else if(loop1 >= HAND_SIZE && highlightClickableAreas[CLICKABLE_PLAY_FROM] >= 0)
			{
				if(loop1 < ClickableArea::DISCARD)
				{
					if(highlightClickableAreas[CLICKABLE_PLAY_TO] == loop1)
					{
						highlightClickableAreas[CLICKABLE_PLAY_TO] = -1;
						highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
					} else if(clickableArea[loop1].Active()) {
						highlightClickableAreas[CLICKABLE_PLAY_TO] = loop1;
					}
					
				} else if(loop1 < ClickableArea::DECK)
				{
					if(highlightClickableAreas[CLICKABLE_PLAY_TO] >= 0)
					{
						if(highlightClickableAreas[CLICKABLE_PLAY_TO] == loop1)
						{
							highlightClickableAreas[CLICKABLE_PLAY_TO] = -1;
							highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
						} else {
							if(highlightClickableAreas[CLICKABLE_DRAW_FROM] == loop1)
							{
								highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
							}
							else if(clickableArea[loop1].Active())
							{
								drawSite = loop1 - ClickableArea::DISCARD;
								highlightClickableAreas[CLICKABLE_DRAW_FROM] = loop1;
							}
						}
					} else {
						if(clickableArea[loop1].Active())
							highlightClickableAreas[CLICKABLE_PLAY_TO] = loop1;
					}
				} else {
					if(highlightClickableAreas[CLICKABLE_DRAW_FROM] == loop1)
					{
						highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
					}
					else if(clickableArea[loop1].Active()) {
						drawSite = LostCitiesState::DRAW_FROM_DECK;
						highlightClickableAreas[CLICKABLE_DRAW_FROM] = loop1;
					}
				}
			}
			break;
		}
	}

	if(highlightClickableAreas[CLICKABLE_DRAW_FROM] >= 0)
	{
		int drawCardID;
		if(highlightClickableAreas[CLICKABLE_DRAW_FROM] == ClickableArea::DECK)
		{
			drawCardID = -1;
		} else
			drawCardID = clickableArea[highlightClickableAreas[CLICKABLE_DRAW_FROM]].CardID();

		int playCardID = clickableArea[highlightClickableAreas[CLICKABLE_PLAY_FROM]].CardID();

		if(highlightClickableAreas[CLICKABLE_PLAY_TO] < ClickableArea::DISCARD)
			playCardID += LostCitiesState::DISCARD_CARD_OFFSET;

		player[currentState->GetActivePlayerID()]->HumanTurn(currentState->GetTurnID(playCardID, drawSite));

		highlightClickableAreas[CLICKABLE_PLAY_FROM] = -1;
		highlightClickableAreas[CLICKABLE_PLAY_TO] = -1;
		highlightClickableAreas[CLICKABLE_DRAW_FROM] = -1;
	}
	UpdateActiveClickableAres();
}

#include <QSpinBox>
pair<QString,QString> LostCities::GetSetupString()
{
	QString header = "Hand Size";
	QString values = QString::number(realHandSize);
	return pair<QString, QString>(header, values);
}

vector<pair<QWidget *, QString> > LostCities::GetSetupWidget()
{
	vector<pair<QWidget *, QString> > widgets;
	QSpinBox * spinBox = new QSpinBox();
	spinBox->setMinimum(2);
	spinBox->setMaximum(8);
	spinBox->setValue(realHandSize);
	widgets.push_back(pair<QWidget *, QString>(spinBox, QString::fromLocal8Bit("Karet v ruce")));
	bool ok = connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(SetHandSize(int)));
	
	QCheckBox * dominationBox = new QCheckBox();
	dominationBox->setChecked(domination);
	widgets.push_back(pair<QWidget *, QString>(dominationBox, QString::fromLocal8Bit("Dominance stav�")));
	connect(dominationBox, SIGNAL(stateChanged(int)), this, SLOT(SetDomination(int)));

	return widgets;
}

void LostCities::SetDomination(int state)
{
	domination = state;
}

void LostCities::SetHandSize(int _handSize)
{
	realHandSize = _handSize;
}