#include "Ludo.h"
#include "EnvironmentAIBasic.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"
#include "Human.h"
#include <qcheckbox.h>

const int Ludo::firstTile[LudoState::MAX_PLAYER] = { 0, 10, 20, 30 };
const QColor Ludo::playerColor[LudoState::MAX_PLAYER] = { Qt::blue, Qt::red, Qt::green, Qt::yellow };

Ludo::Ludo(QWidget * _widget, bool _paint) : Game(_widget, _paint)
{
	minPlayerAI = 2;
	maxPlayerAI = 4;
	onlyTwoPlayers = false;

	QColor normal = Qt::cyan;
	QColor safe = Qt::darkCyan;
	QColor player1 = Qt::blue;
	QColor player2 = Qt::red;
	QColor player3 = Qt::green;
	QColor player4 = Qt::yellow;

	tileGame[0] = new LudoTile(4, 0, player1, this);
	tileGame[1] = new LudoTile(4, 1, normal, this);	
	tileGame[2] = new LudoTile(4, 2, normal, this);
	tileGame[3] = new LudoTile(4, 3, normal, this);
	tileGame[4] = new LudoTile(4, 4, normal, this);
	tileGame[5] = new LudoTile(3, 4, normal, this);
	tileGame[6] = new LudoTile(2, 4, normal, this);
	tileGame[7] = new LudoTile(1, 4, safe, this);
	tileGame[8] = new LudoTile(0, 4, normal, this);
	tileGame[9] = new LudoTile(0, 5, normal, this);
	tileGame[10] = new LudoTile(0, 6, player2, this);
	tileGame[11] = new LudoTile(1, 6, normal, this);
	tileGame[12] = new LudoTile(2, 6, normal, this);
	tileGame[13] = new LudoTile(3, 6, safe, this);
	tileGame[14] = new LudoTile(4, 6, normal, this);
	tileGame[15] = new LudoTile(4, 7, normal, this);
	tileGame[16] = new LudoTile(4, 8, normal, this);
	tileGame[17] = new LudoTile(4, 9, safe, this);
	tileGame[18] = new LudoTile(4, 10, normal, this);
	tileGame[19] = new LudoTile(5, 10, normal, this);
	tileGame[20] = new LudoTile(6, 10, player3, this);
	tileGame[21] = new LudoTile(6, 9, normal, this);
	tileGame[22] = new LudoTile(6, 8, normal, this);
	tileGame[23] = new LudoTile(6, 7, safe, this);
	tileGame[24] = new LudoTile(6, 6, normal, this);
	tileGame[25] = new LudoTile(7, 6, normal, this);
	tileGame[26] = new LudoTile(8, 6, normal, this);
	tileGame[27] = new LudoTile(9, 6, safe, this);
	tileGame[28] = new LudoTile(10, 6, normal, this);
	tileGame[29] = new LudoTile(10, 5, normal, this);
	tileGame[30] = new LudoTile(10, 4, player4, this);
	tileGame[31] = new LudoTile(9, 4, normal, this);
	tileGame[32] = new LudoTile(8, 4, normal, this);
	tileGame[33] = new LudoTile(7, 4, safe, this);
	tileGame[34] = new LudoTile(6, 4, normal, this);
	tileGame[35] = new LudoTile(6, 3, normal, this);
	tileGame[36] = new LudoTile(6, 2, normal, this);
	tileGame[37] = new LudoTile(6, 1, safe, this);
	tileGame[38] = new LudoTile(6, 0, normal, this);
	tileGame[39] = new LudoTile(5, 0, normal, this);

	// home start
	tileGame[40] = new LudoTile(0, 0, player1, this);
	tileGame[41] = new LudoTile(1, 0, player1, this);
	tileGame[42] = new LudoTile(0, 1, player1, this);
	tileGame[43] = new LudoTile(1, 1, player1, this);
	tileGame[44] = new LudoTile(0, 10, player2, this);
	tileGame[45] = new LudoTile(0, 9, player2, this);
	tileGame[46] = new LudoTile(1, 9, player2, this);
	tileGame[47] = new LudoTile(1, 10, player2, this);
	tileGame[48] = new LudoTile(10, 10, player3, this);
	tileGame[49] = new LudoTile(9, 10, player3, this);
	tileGame[50] = new LudoTile(10, 9, player3, this);
	tileGame[51] = new LudoTile(9, 9, player3, this);
	tileGame[52] = new LudoTile(10, 0, player4, this);
	tileGame[53] = new LudoTile(9, 0, player4, this);
	tileGame[54] = new LudoTile(10, 1, player4, this);
	tileGame[55] = new LudoTile(9, 1, player4, this);

	// home end
	tileGame[56] = new LudoTile(5, 1, player1, this);
	tileGame[57] = new LudoTile(5, 2, player1, this);
	tileGame[58] = new LudoTile(5, 3, player1, this);
	tileGame[59] = new LudoTile(5, 4, player1, this);
	tileGame[60] = new LudoTile(1, 5, player2, this);
	tileGame[61] = new LudoTile(2, 5, player2, this);
	tileGame[62] = new LudoTile(3, 5, player2, this);
	tileGame[63] = new LudoTile(4, 5, player2, this);
	tileGame[64] = new LudoTile(5, 9, player3, this);
	tileGame[65] = new LudoTile(5, 8, player3, this);
	tileGame[66] = new LudoTile(5, 7, player3, this);
	tileGame[67] = new LudoTile(5, 6, player3, this);
	tileGame[68] = new LudoTile(9, 5, player4, this);
	tileGame[69] = new LudoTile(8, 5, player4, this);
	tileGame[70] = new LudoTile(7, 5, player4, this);
	tileGame[71] = new LudoTile(6, 5, player4, this);
	boardX = 0;
	boardY = 0;

	for(int loop1 = 0; loop1 < LudoState::FIRST_HOME_TILE; loop1++)
	{
		if(LudoState::IsTileSafe(loop1))
		{
			tileGame[loop1]->SetColor(safe);
		}
	}

	// center cube number
	tileGame[72] = new LudoTile(5, 5, normal, this);

	player = new IPlayer*[5];
	EnvironmentAI * eai = new EnvironmentAIBasic(0);
	float coefs[COEF_COUNT] = { 10, 1000, 10, 10, 10, 3, 1, 0 };
	eai->SetMetricCoefs(coefs);
	player[0] = eai;
	if(paint)
	{
		player[1] = new Human(1);
	} else {
		player[1] = new PlayerHillClimber(1);
	}
	player[2] = new PlayerHillClimber(2);
	player[3] = new PlayerHillClimber(3);
	player[4] = new PlayerHillClimber(4);

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

	playerCount = 5;

	currentState = NULL;
	state = STATE_STOPPED;
}


Ludo::~Ludo(void)
{
	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{
		delete tileGame[loop1];
	}
}

bool Ludo::PlayerTurn()
{
	int playerTurn = player[currentState->GetActivePlayerID()]->MakeTurn();
	
	return currentState->MakeTurn(playerTurn) >= 0;
}

void Ludo::StartGame()
{
	Game::StartGame();

	if(currentState != NULL)
		delete currentState;

	currentState = new LudoState(this, onlyTwoPlayers);

	player[0]->StartGame(this);
	player[1]->StartGame(this);
	player[2]->StartGame(this);
	player[3]->StartGame(this);
	player[4]->StartGame(this);

	state = STATE_RUNNING;
	activeHumanFigure = -1;
	if(player[0]->Think())
		NextTurn();
}

void Ludo::Draw(QPainter * painter, int tickMillis)
{
	boardHeight = painter->viewport().height();
	boardWidth = painter->viewport().width();
	tileWidth = boardWidth / 11.0f;
	tileHeight = boardHeight / 11.0f;
	tileWidth = min(tileWidth, tileHeight);
	tileHeight = min(tileWidth, tileHeight);
	boardX = (painter->viewport().width() - tileWidth * 11) / 2;
	boardY = (painter->viewport().height() - tileHeight * 11) / 2;

	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{	
		tileGame[loop1]->Draw(painter, tickMillis);
	}

	switch(state)
	{
		case STATE_STOPPED :
			return;
	}

	for(int loop1 = 0; loop1 < LudoState::MAX_PLAYER; loop1++)
	{
		int atStart = 0;
		for(int loop2 = 0; loop2 < LudoState::MAX_FIGURE; loop2++)
		{
			int tileId = currentState->GetFigure(loop1, loop2);
			if(tileId < 0)
			{
				tileId = atStart + loop1 * LudoState::MAX_FIGURE + PLAYER_1_START;
				atStart++;
			} else if(tileId >= PLAYER_1_START)
			{
				tileId = tileId + 16 + loop1 * LudoState::MAX_FIGURE;
			} else {
				tileId = (tileId + firstTile[loop1]) % 40;
			}

			if(activeHumanFigure < 0 && loop1 + 1 == currentState->GetActivePlayerID() && (currentState->GetFigureNextState(loop2) >= 0 || currentState->GetFigureNextState(loop2 + LudoState::MAX_FIGURE) >= 0))
				tileGame[tileId]->Draw(painter, Qt::white, tickMillis);

			tileGame[tileId]->DrawPlayer(painter, playerColor[loop1]);
		}
	}

	for(int loop1 = 0; loop1 < MAX_TILE; loop1++)
	{
		int temp;
		if(activeHumanFigure >= 0 && state == STATE_RUNNING && 
			(GetTileID(currentState->GetActivePlayerID() - 1, activeHumanFigure, temp, true) == loop1 || GetTileID(currentState->GetActivePlayerID() - 1, activeHumanFigure + LudoState::MAX_FIGURE, temp, true) == loop1))
			tileGame[loop1]->DrawPlayer(painter, Qt::white);
	}

	char numberString[10];
	sprintf(numberString, "%d", currentState->GetLastDice());
	painter->setFont(QFont("Helvetica", 28, QFont::Bold));
	painter->setPen(Qt::black);
	painter->drawText(0, 0, painter->viewport().width() - 2, painter->viewport().height() - 2, Qt::AlignCenter, QString(numberString));
	painter->setPen(playerColor[(currentState->GetActivePlayerID() - 1 < 0) ? 0 : currentState->GetActivePlayerID() - 1]);
	painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height(), Qt::AlignCenter, QString(numberString));

	int x, y;
	for(int loop1 = 0; loop1 < LudoState::MAX_PLAYER; loop1++)
	{
		sprintf(numberString, "%d", currentState->GetPlayerRank(loop1 + 1, 0));
		switch(loop1)
		{
			case 0 :
				x = painter->viewport().width() / 7;
				y = painter->viewport().height() - painter->viewport().height() / 3;
				break;
			case 1 :
				x = painter->viewport().width() / 7;
				y = painter->viewport().height() / 5;
				break;
			case 2 :
				x = painter->viewport().width() - painter->viewport().width() / 4;
				y = painter->viewport().height() / 5;
				break;
			case 3 :
				x = painter->viewport().width() - painter->viewport().width() / 4;
				y = painter->viewport().height() - painter->viewport().height() / 3;
				break;
		}

		painter->setPen(Qt::black);
		//painter->drawText(x - 1, y - 1, 110, 110, Qt::AlignCenter, QString(numberString));
		painter->setPen(playerColor[loop1]);
		//painter->drawText(x, y, 110, 110, Qt::AlignCenter, QString(numberString));
	}
}

IGameState * Ludo::GetCurrentState() const
{
	return currentState;
}

int Ludo::GetTileID(int player, int figure, int & atStart, bool nextState)
{
	int tileId;
	
	if(nextState)
		tileId = currentState->GetFigureNextState(figure);
	else
		tileId = currentState->GetFigure(player, figure);
	if(tileId < 0)
	{
		tileId = atStart + player * LudoState::MAX_FIGURE + PLAYER_1_START;
		atStart++;
	} else if(tileId >= PLAYER_1_START)
	{
		tileId = tileId + 16 + player * LudoState::MAX_FIGURE;
	} else {
		tileId = (tileId + firstTile[player]) % 40;
	}

	return tileId;
}

void Ludo::MouseMoveEvent ( int xMouse, int yMouse )
{

}

void Ludo::MousePressEvent ( int xMouse, int yMouse )
{
	if(state != STATE_RUNNING)
		return;

	int activePlayerID = GetCurrentState()->GetActivePlayerID();

	bool noTurn = true;
	int atStart = 0;
	int action = 0;

	for(int loop2 = 0; loop2 < LudoState::MAX_FIGURE; loop2++)
	{
		int tileId = GetTileID(activePlayerID - 1, loop2, atStart);
		if(currentState->GetFigureNextState(loop2) >= 0 || currentState->GetFigureNextState(loop2 + LudoState::MAX_FIGURE) >= 0)
		{
			noTurn = false;
			if(tileGame[tileId]->Contain(xMouse, yMouse))
			{
				if(activeHumanFigure == loop2)
					activeHumanFigure = -1;
				else
					activeHumanFigure = loop2;
				break;
			}
		}

	}
	
	if(activeHumanFigure >= 0) 
	{
		for(int loop2 = 0; loop2 < LudoState::MAX_CHOISES; loop2++)
		{
			if(currentState->GetFigureNextState(loop2) >= 0)
			{
				noTurn = false;
				if(loop2 % LudoState::MAX_FIGURE == activeHumanFigure && tileGame[GetTileID(activePlayerID - 1, loop2, atStart, true)]->Contain(xMouse, yMouse))
				{
					player[activePlayerID]->HumanTurn(action);
					activeHumanFigure = -1;
					break;
				}
				action++;
			}
		}
	}

	if(noTurn)
	{
		player[activePlayerID]->HumanTurn(0);
	}
}

#include <QSpinBox>

pair<QString,QString> Ludo::GetSetupString()
{
	QString header = "";
	QString values = "";
	return pair<QString, QString>(header, values);
}

vector<pair<QWidget *, QString> > Ludo::GetSetupWidget()
{
	vector<pair<QWidget *, QString> > widgets;

	QCheckBox * twoPlayersBox = new QCheckBox();
	twoPlayersBox->setChecked(onlyTwoPlayers);
	widgets.push_back(pair<QWidget *, QString>(twoPlayersBox, QString::fromLocal8Bit("Pouze dva hr·Ëi")));
	connect(twoPlayersBox, SIGNAL(stateChanged(int)), this, SLOT(SetOnlyTwoPlayers(int)));

	return widgets;
}

void Ludo::SetOnlyTwoPlayers(int twoPlayers)
{
	onlyTwoPlayers = twoPlayers != 0;
}