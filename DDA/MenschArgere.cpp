#include "MenschArgere.h"
#include "EnvironmentAIBasic.h"
#include "PlayerRandomAI.h"
#include "Human.h"

const int MenschArgere::firstTile[MenschState::MAX_PLAYER] = { 0, 10, 20, 30 };
const QColor MenschArgere::playerColor[MenschState::MAX_PLAYER] = { Qt::blue, Qt::red, Qt::green, Qt::yellow };

MenschArgere::MenschArgere(QWidget * _widget) : Game(_widget)
{
	minEnvironmentalAI = 1;
	maxEnvironmentalAI = 1;
	minPlayerAI = 2;
	maxPlayerAI = 4;

	QColor normal = Qt::cyan;
	QColor player1 = playerColor[0];
	QColor player2 = playerColor[1];
	QColor player3 = playerColor[2];
	QColor player4 = playerColor[3];

	tileGame[0] = new MenschTile(4, 0, player1, this);
	tileGame[1] = new MenschTile(4, 1, normal, this);	
	tileGame[2] = new MenschTile(4, 2, normal, this);
	tileGame[3] = new MenschTile(4, 3, normal, this);
	tileGame[4] = new MenschTile(4, 4, normal, this);
	tileGame[5] = new MenschTile(3, 4, normal, this);
	tileGame[6] = new MenschTile(2, 4, normal, this);
	tileGame[7] = new MenschTile(1, 4, normal, this);
	tileGame[8] = new MenschTile(0, 4, normal, this);
	tileGame[9] = new MenschTile(0, 5, normal, this);
	tileGame[10] = new MenschTile(0, 6, player2, this);
	tileGame[11] = new MenschTile(1, 6, normal, this);
	tileGame[12] = new MenschTile(2, 6, normal, this);
	tileGame[13] = new MenschTile(3, 6, normal, this);
	tileGame[14] = new MenschTile(4, 6, normal, this);
	tileGame[15] = new MenschTile(4, 7, normal, this);
	tileGame[16] = new MenschTile(4, 8, normal, this);
	tileGame[17] = new MenschTile(4, 9, normal, this);
	tileGame[18] = new MenschTile(4, 10, normal, this);
	tileGame[19] = new MenschTile(5, 10, normal, this);
	tileGame[20] = new MenschTile(6, 10, player3, this);
	tileGame[21] = new MenschTile(6, 9, normal, this);
	tileGame[22] = new MenschTile(6, 8, normal, this);
	tileGame[23] = new MenschTile(6, 7, normal, this);
	tileGame[24] = new MenschTile(6, 6, normal, this);
	tileGame[25] = new MenschTile(7, 6, normal, this);
	tileGame[26] = new MenschTile(8, 6, normal, this);
	tileGame[27] = new MenschTile(9, 6, normal, this);
	tileGame[28] = new MenschTile(10, 6, normal, this);
	tileGame[29] = new MenschTile(10, 5, normal, this);
	tileGame[30] = new MenschTile(10, 4, player4, this);
	tileGame[31] = new MenschTile(9, 4, normal, this);
	tileGame[32] = new MenschTile(8, 4, normal, this);
	tileGame[33] = new MenschTile(7, 4, normal, this);
	tileGame[34] = new MenschTile(6, 4, normal, this);
	tileGame[35] = new MenschTile(6, 3, normal, this);
	tileGame[36] = new MenschTile(6, 2, normal, this);
	tileGame[37] = new MenschTile(6, 1, normal, this);
	tileGame[38] = new MenschTile(6, 0, normal, this);
	tileGame[39] = new MenschTile(5, 0, normal, this);

	tileGame[40] = new MenschTile(0, 0, player1, this);
	tileGame[41] = new MenschTile(1, 0, player1, this);
	tileGame[42] = new MenschTile(0, 1, player1, this);
	tileGame[43] = new MenschTile(1, 1, player1, this);
	tileGame[44] = new MenschTile(0, 10, player2, this);
	tileGame[45] = new MenschTile(0, 9, player2, this);
	tileGame[46] = new MenschTile(1, 9, player2, this);
	tileGame[47] = new MenschTile(1, 10, player2, this);
	tileGame[48] = new MenschTile(10, 10, player3, this);
	tileGame[49] = new MenschTile(9, 10, player3, this);
	tileGame[50] = new MenschTile(10, 9, player3, this);
	tileGame[51] = new MenschTile(9, 9, player3, this);
	tileGame[52] = new MenschTile(10, 0, player4, this);
	tileGame[53] = new MenschTile(9, 0, player4, this);
	tileGame[54] = new MenschTile(10, 1, player4, this);
	tileGame[55] = new MenschTile(9, 1, player4, this);

	tileGame[56] = new MenschTile(5, 1, player1, this);
	tileGame[57] = new MenschTile(5, 2, player1, this);
	tileGame[58] = new MenschTile(5, 3, player1, this);
	tileGame[59] = new MenschTile(5, 4, player1, this);
	tileGame[60] = new MenschTile(1, 5, player2, this);
	tileGame[61] = new MenschTile(2, 5, player2, this);
	tileGame[62] = new MenschTile(3, 5, player2, this);
	tileGame[63] = new MenschTile(4, 5, player2, this);
	tileGame[64] = new MenschTile(5, 9, player3, this);
	tileGame[65] = new MenschTile(5, 8, player3, this);
	tileGame[66] = new MenschTile(5, 7, player3, this);
	tileGame[67] = new MenschTile(5, 6, player3, this);
	tileGame[68] = new MenschTile(9, 5, player4, this);
	tileGame[69] = new MenschTile(8, 5, player4, this);
	tileGame[70] = new MenschTile(7, 5, player4, this);
	tileGame[71] = new MenschTile(6, 5, player4, this);

	tileGame[72] = new MenschTile(5, 5, normal, this);

	currentState = NULL;
	state = STATE_STOPPED;
}


MenschArgere::~MenschArgere(void)
{
	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{
		delete tileGame[loop1];
	}
}

bool MenschArgere::PlayerTurn()
{
	int playerTurn = player[currentState->GetActivePlayerID()]->MakeTurn();
	
	return currentState->MakeTurn(playerTurn) >= 0;
}

void MenschArgere::StartGame()
{
	if(currentState != NULL)
		delete currentState;

	currentState = new MenschState();

	if(player == NULL)
	{
		player = new IPlayer*[5];
		player[0] = new EnvironmentAIBasic();
		player[1] = new PlayerRandomAI();
		player[2] = new PlayerRandomAI();
		player[3] = new Human();
		player[4] = new Human();

		QObject::connect(player[0], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[1], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[2], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[3], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[4], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
	} 

	playerCount = 5;
	player[0]->StartGame(this);
	player[1]->StartGame(this);
	player[2]->StartGame(this);
	player[3]->StartGame(this);
	player[4]->StartGame(this);

	state = STATE_RUNNING;
	if(player[0]->Think())
		NextTurn();
}

void MenschArgere::Draw(QPainter * painter, int tickMillis)
{
	boardHeight = painter->viewport().height();
	boardWidth = painter->viewport().width();
	tileWidth = boardWidth / 11.0f;
	tileHeight = boardHeight / 11.0f;

	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{
		tileGame[loop1]->Draw(painter, tickMillis);
	}

	switch(state)
	{
		case STATE_STOPPED :
			return;
	}

	for(int loop1 = 0; loop1 < MenschState::MAX_PLAYER; loop1++)
	{
		int atStart = 0;
		for(int loop2 = 0; loop2 < MenschState::MAX_FIGURE; loop2++)
		{
			int tileId = currentState->GetFigure(loop1, loop2);
			if(tileId < 0)
			{
				tileId = atStart + loop1 * MenschState::MAX_FIGURE + PLAYER_1_START;
				atStart++;
			} else if(tileId >= PLAYER_1_START)
			{
				tileId = tileId + 16 + loop1 * MenschState::MAX_FIGURE;
			} else {
				tileId = (tileId + firstTile[loop1]) % 40;
			}

			if(loop1 + 1 == currentState->GetActivePlayerID() && currentState->GetFigureNextState(loop2) >= 0)
				tileGame[tileId]->Draw(painter, Qt::white, tickMillis);

			tileGame[tileId]->DrawPlayer(painter, playerColor[loop1]);
		}
	}

	char numberString[2];
	sprintf(numberString, "%d", currentState->GetLastDice());
	painter->setFont(QFont("Helvetica", 28, QFont::Bold));
	painter->setPen(Qt::black);
	painter->drawText(0, 0, painter->viewport().width() - 2, painter->viewport().height() - 2, Qt::AlignCenter, QString(numberString));
	painter->setPen(playerColor[(currentState->GetActivePlayerID() - 1 < 0) ? 0 : currentState->GetActivePlayerID() - 1]);
	painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height(), Qt::AlignCenter, QString(numberString));
}

IGameState * MenschArgere::GetCurrentState() const
{
	return currentState;
}

int MenschArgere::GetTileID(int player, int figure, int & atStart)
{
	int tileId = currentState->GetFigure(player, figure);
	if(tileId < 0)
	{
		tileId = atStart + player * MenschState::MAX_FIGURE + PLAYER_1_START;
		atStart++;
	} else if(tileId >= PLAYER_1_START)
	{
		tileId = tileId + 16 + player * MenschState::MAX_FIGURE;
	} else {
		tileId = (tileId + firstTile[player]) % 40;
	}

	return tileId;
}

void MenschArgere::MouseMoveEvent ( int xMouse, int yMouse )
{

}

void MenschArgere::MousePressEvent ( int xMouse, int yMouse )
{
	if(state != STATE_RUNNING)
		return;

	int activePlayerID = GetCurrentState()->GetActivePlayerID();

	bool noTurn = true;
	int atStart = 0;
	int action = 0;
	for(int loop2 = 0; loop2 < MenschState::MAX_FIGURE; loop2++)
	{
		int tileId = GetTileID(activePlayerID - 1, loop2, atStart);

		if(currentState->GetFigureNextState(loop2) >= 0)
		{
			noTurn = false;
			if(tileGame[tileId]->Contain(xMouse, yMouse))
			{
				player[activePlayerID]->HumanTurn(action);
			}
			action++;
		}
	}

	if(noTurn)
	{
		player[activePlayerID]->HumanTurn(0);
	}
}