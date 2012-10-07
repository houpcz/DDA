#include "MenschArgere.h"
#include "EnvironmentAIBasic.h"
#include "PlayerRandomAI.h"

const int MenschArgere::firstTile[MenschState::MAX_PLAYER] = { 0, 10, 20, 30 };
const QColor MenschArgere::playerColor[MenschState::MAX_PLAYER] = { Qt::blue, Qt::red, Qt::green, Qt::yellow };

MenschArgere::MenschArgere(QWidget * _widget) : Game(_widget)
{
	QColor normal = Qt::cyan;
	QColor player1 = playerColor[0];
	QColor player2 = playerColor[1];
	QColor player3 = playerColor[2];
	QColor player4 = playerColor[3];

	tileGame[0] = new MenschTile(4, 0, player1);
	tileGame[1] = new MenschTile(4, 1, normal);	
	tileGame[2] = new MenschTile(4, 2, normal);
	tileGame[3] = new MenschTile(4, 3, normal);
	tileGame[4] = new MenschTile(4, 4, normal);
	tileGame[5] = new MenschTile(3, 4, normal);
	tileGame[6] = new MenschTile(2, 4, normal);
	tileGame[7] = new MenschTile(1, 4, normal);
	tileGame[8] = new MenschTile(0, 4, normal);
	tileGame[9] = new MenschTile(0, 5, normal);
	tileGame[10] = new MenschTile(0, 6, player2);
	tileGame[11] = new MenschTile(1, 6, normal);
	tileGame[12] = new MenschTile(2, 6, normal);
	tileGame[13] = new MenschTile(3, 6, normal);
	tileGame[14] = new MenschTile(4, 6, normal);
	tileGame[15] = new MenschTile(4, 7, normal);
	tileGame[16] = new MenschTile(4, 8, normal);
	tileGame[17] = new MenschTile(4, 9, normal);
	tileGame[18] = new MenschTile(4, 10, normal);
	tileGame[19] = new MenschTile(5, 10, normal);
	tileGame[20] = new MenschTile(6, 10, player3);
	tileGame[21] = new MenschTile(6, 9, normal);
	tileGame[22] = new MenschTile(6, 8, normal);
	tileGame[23] = new MenschTile(6, 7, normal);
	tileGame[24] = new MenschTile(6, 6, normal);
	tileGame[25] = new MenschTile(7, 6, normal);
	tileGame[26] = new MenschTile(8, 6, normal);
	tileGame[27] = new MenschTile(9, 6, normal);
	tileGame[28] = new MenschTile(10, 6, normal);
	tileGame[29] = new MenschTile(10, 5, normal);
	tileGame[30] = new MenschTile(10, 4, player4);
	tileGame[31] = new MenschTile(9, 4, normal);
	tileGame[32] = new MenschTile(8, 4, normal);
	tileGame[33] = new MenschTile(7, 4, normal);
	tileGame[34] = new MenschTile(6, 4, normal);
	tileGame[35] = new MenschTile(6, 3, normal);
	tileGame[36] = new MenschTile(6, 2, normal);
	tileGame[37] = new MenschTile(6, 1, normal);
	tileGame[38] = new MenschTile(6, 0, normal);
	tileGame[39] = new MenschTile(5, 0, normal);

	tileGame[40] = new MenschTile(0, 0, player1);
	tileGame[41] = new MenschTile(1, 0, player1);
	tileGame[42] = new MenschTile(0, 1, player1);
	tileGame[43] = new MenschTile(1, 1, player1);
	tileGame[44] = new MenschTile(0, 10, player2);
	tileGame[45] = new MenschTile(0, 9, player2);
	tileGame[46] = new MenschTile(1, 9, player2);
	tileGame[47] = new MenschTile(1, 10, player2);
	tileGame[48] = new MenschTile(10, 10, player3);
	tileGame[49] = new MenschTile(9, 10, player3);
	tileGame[50] = new MenschTile(10, 9, player3);
	tileGame[51] = new MenschTile(9, 9, player3);
	tileGame[52] = new MenschTile(10, 0, player4);
	tileGame[53] = new MenschTile(9, 0, player4);
	tileGame[54] = new MenschTile(10, 1, player4);
	tileGame[55] = new MenschTile(9, 1, player4);

	tileGame[56] = new MenschTile(5, 1, player1);
	tileGame[57] = new MenschTile(5, 2, player1);
	tileGame[58] = new MenschTile(5, 3, player1);
	tileGame[59] = new MenschTile(5, 4, player1);
	tileGame[60] = new MenschTile(1, 5, player2);
	tileGame[61] = new MenschTile(2, 5, player2);
	tileGame[62] = new MenschTile(3, 5, player2);
	tileGame[63] = new MenschTile(4, 5, player2);
	tileGame[64] = new MenschTile(5, 9, player3);
	tileGame[65] = new MenschTile(5, 8, player3);
	tileGame[66] = new MenschTile(5, 7, player3);
	tileGame[67] = new MenschTile(5, 6, player3);
	tileGame[68] = new MenschTile(9, 5, player4);
	tileGame[69] = new MenschTile(8, 5, player4);
	tileGame[70] = new MenschTile(7, 5, player4);
	tileGame[71] = new MenschTile(6, 5, player4);

	currentState = NULL;
	isRunning = false;
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
	
	return currentState->MakeTurn(playerTurn) < 0;
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
		player[3] = new PlayerRandomAI();
		player[4] = new PlayerRandomAI();

		QObject::connect(player[0], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[1], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
	} 

	playerCount = 5;
	player[0]->StartGame(this);
	player[1]->StartGame(this);
	player[2]->StartGame(this);
	player[3]->StartGame(this);
	player[4]->StartGame(this);

	isRunning = true;
	if(player[0]->Think())
		NextTurn();
}

MenschTile::MenschTile(int _x, int _y, QColor _color)
{
	x = _x;
	y = _y;
	color = _color;
}

void MenschTile::Draw(QPainter * painter, int tickMillis)
{
	float boardHeight = painter->viewport().height();
	float boardWidth = painter->viewport().width();
	float tileWidth = boardWidth / 11.0f * 0.90f;
	float tileHeight = boardHeight / 11.0f * 0.90f;
	float borderWidth = boardWidth / 11.0f * 0.10f;
	float borderHeight = boardHeight / 11.0f * 0.10f;
	
	painter->setBrush(QBrush(color));
	painter->drawEllipse(borderWidth / 2.0f + x * tileWidth + x * borderWidth, boardHeight - (borderHeight + y * tileHeight + y * borderHeight) - tileHeight, tileWidth, tileHeight);
}

void MenschTile::DrawPlayer(QPainter * painter, const QColor & playerColor)
{
	float boardHeight = painter->viewport().height();
	float boardWidth = painter->viewport().width();
	float tileWidth = boardWidth / 11.0f * 0.60f;
	float tileHeight = boardHeight / 11.0f * 0.60f;
	float borderWidth = boardWidth / 11.0f * 0.40f;
	float borderHeight = boardHeight / 11.0f * 0.40f;
	
	painter->setBrush(QBrush(playerColor));
	painter->drawEllipse(borderWidth / 2.0f + x * tileWidth + x * borderWidth, boardHeight - (borderHeight + y * tileHeight + y * borderHeight) - boardHeight / 22.0f, tileWidth, tileHeight);
}

void MenschArgere::Draw(QPainter * painter, int tickMillis)
{
	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{
		tileGame[loop1]->Draw(painter, tickMillis);
	}

	if(!isRunning)
		return;

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
			tileGame[tileId]->DrawPlayer(painter, playerColor[loop1]);
		}
	}

	char numberString[2];
	sprintf(numberString, "%d", currentState->GetLastDice());
	painter->setFont(QFont("Helvetica", 32, QFont::Bold));
	painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height(), Qt::AlignCenter, QString(numberString));
}

IGameState * MenschArgere::GetCurrentState() const
{
	return currentState;
}