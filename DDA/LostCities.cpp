#include "LostCities.h"
#include "EnvironmentAIBasic.h"
#include "PlayerRandomAI.h"
#include "Human.h"

LostCities::LostCities(QWidget * _widget) : Game(_widget)
{
	minEnvironmentalAI = 1;
	maxEnvironmentalAI = 1;
	minPlayerAI = 2;
	maxPlayerAI = 2;

	player = new IPlayer*[3];
	player[0] = new EnvironmentAIBasic();
	player[1] = new PlayerRandomAI();
	player[2] = new PlayerRandomAI();

	QObject::connect(player[0], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[1], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[2], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));

	playerCount = 3;

	cardWidth = 60;
	cardHeight = 60;
	currentState = new LostCitiesState();
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
	if(currentState != NULL)
		delete currentState;
	currentState = new LostCitiesState();

	player[0]->StartGame(this);
	player[1]->StartGame(this);
	player[2]->StartGame(this);

	state = STATE_RUNNING;
	if(player[1]->Think())
		NextTurn();
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
	for(int loop1 = 0; loop1 < LostCitiesState::CARD_AMOUNT; loop1++)
	{
		int cardNumber = loop1 % 12 + 2;
		int cardColorNumber = loop1 / 12;
		
		switch(currentState->GetCard(loop1))
		{
			case PLAYER_1_HAND :
				DrawCard(painter, loop1, cardWidth / 2 + cardHandID * (cardWidth / 3), 400);
				cardHandID++;
				break;
			case PLAYER_1_ON_DESK :
				DrawCard(painter, loop1, cardWidth / 2 + cardColorNumber * (1.5f * cardWidth), 250 + (cardHeight / 3) * expedition[0][cardColorNumber]);
				expedition[0][cardColorNumber]++;
				break;
			case PLAYER_2_ON_DESK :
				DrawCard(painter, loop1, cardWidth / 2 + cardColorNumber * (1.5f * cardWidth), 50 + (cardHeight / 3) * expedition[1][cardColorNumber]);
				expedition[1][cardColorNumber]++;
				break;
			case IN_DECK :
				inDeck++;
				break;
		}	
	}
	char cardInDeckString[24];
	sprintf(cardInDeckString, "Cards in the deck : %d", inDeck);
	painter->drawText(20, 20, 500, 500, 0, cardInDeckString);
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
			cardColor = Qt::blue;
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
	painter->drawText(x + 5, y + 5, cardWidth, cardHeight, 0, symbolString);
}

IGameState * LostCities::GetCurrentState() const
{
	return currentState;
}
void LostCities::MouseMoveEvent ( int xMouse, int yMouse )
{
}
void LostCities::MousePressEvent ( int xMouse, int yMouse )
{
}
