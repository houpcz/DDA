#include "GameMaze.h"
#include "EnvironmentAIBasic.h"
#include "Human.h"
#include "PlayerRandomAI.h"

GameMaze::GameMaze(QWidget * _widget, bool _paint) : Game(_widget, _paint)
{
	minPlayerAI = 1;
	maxPlayerAI = 1;

	mazeWidth = 41;
	mazeHeight = 41;
	stepsToGameOver = 2000;
	currentState = new MazeState(1, stepsToGameOver, mazeWidth, mazeHeight);
	tileWidth = 10.0f;
	tileHeight = 10.0f;

	playerCount = 2;
	player = new IPlayer*[playerCount];
	player[ENVINRONMENT_AI] = new EnvironmentAIBasic(ENVINRONMENT_AI);
	if(paint)
	{
		player[PLAYER_AI] = new Human(PLAYER_AI);
	} else {
		player[PLAYER_AI] = new PlayerRandomAI(PLAYER_AI);
	}

	QObject::connect(player[PLAYER_AI], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[ENVINRONMENT_AI], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
}

void GameMaze::StartGame()
{
	Game::StartGame();

	if(currentState != NULL)
	{
		delete currentState;
	}

	currentState = new MazeState(PLAYER_AI, stepsToGameOver, mazeWidth, mazeHeight);
	playerCount = 2;
	player[ENVINRONMENT_AI]->StartGame(this);
	player[PLAYER_AI]->StartGame(this);
	state = STATE_RUNNING;
	if(player[PLAYER_AI]->Think())
		NextTurn();
}

bool GameMaze::PlayerTurn()
{
	int playerTurn = player[currentState->GetActivePlayerID()]->MakeTurn();
	bool gameOver = currentState->Explore(playerTurn);

	return gameOver;
}

GameMaze::~GameMaze()
{
	delete currentState;
}

void GameMaze::Draw(QPainter * painter, int tickMillis)
{
	int mazeWidth = currentState->GetMazeWidth();
	int mazeHeight = currentState->GetMazeHeight();
	char ** maze = currentState->GetMaze();
	const vector<int>* tileToExplore = currentState->GetTileToExplore();

	tileWidth = painter->viewport().width() / (float) mazeWidth;
	tileHeight =  painter->viewport().height() / (float) mazeHeight;

	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			switch(maze[loop1][loop2])
			{
				case TILE_UNDEFINED :
					painter->setBrush(QBrush(QColor(0, 0, 0)));
					break;
				case TILE_EMPTY :
					painter->setBrush(QBrush(QColor(230, 230, 255)));
					break;
				case TILE_WALL :
					painter->setBrush(QBrush(QColor(100, 120, 100)));
					break;
				case TILE_GOAL :
					painter->setBrush(QBrush(QColor(20, 250, 50)));
					break;
			}
			painter->fillRect(loop2 * tileWidth, loop1 * tileHeight, tileWidth + 1, tileHeight + 1, painter->brush());
		}
	}

	for(int loop1 = 0; loop1 < tileToExplore->size(); loop1++)
	{
		int x, y;
		currentState->Pos1Dto2D((*tileToExplore)[loop1], &x, &y);

		QColor color;
		if(maze[y][x] == TILE_GOAL)
		{
			color = Qt::darkCyan;
		} else {
			color = QColor(238, 154, 77);
		}
		painter->fillRect(x * tileWidth + 1, 
			              y * tileHeight + 1, 
						  tileWidth - 2, tileHeight - 2, QBrush(color));
	}

	
	painter->setBrush(QBrush(QColor(200, 200, 10)));
	painter->drawEllipse(currentState->GetPlayerX() * tileWidth + 1, 
			                currentState->GetPlayerY() * tileHeight + 1, 
							tileWidth - 2, tileHeight - 2);

	int mouseXID = (int) (lastMouseX / tileWidth);
	int mouseYID = (int) (lastMouseY / tileHeight);
	painter->setBrush(QBrush(QColor(200, 200, 10, 100)));
	painter->drawRect(mouseXID * tileWidth + 1, 
			             mouseYID * tileHeight + 1, 
							tileWidth - 2, tileHeight - 2);



	char numberString[6];
	sprintf(numberString, "%d", currentState->GetStepsToGameOver());
	painter->setFont(QFont("Helvetica", 14, QFont::Bold));
	painter->setPen(Qt::black);
	painter->drawText(0, 0, painter->viewport().width() - 2, painter->viewport().height() / 8 - 2, Qt::AlignCenter, QString(numberString));
	painter->setPen(Qt::cyan);
	painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height() / 8, Qt::AlignCenter, QString(numberString));

	int score = currentState->GetPlayerScore(0, 0);
	sprintf(numberString, "%d", score);

	static bool wasIlegal = false;
	if(score == IGameState::ILLEGAL_GAME)
		wasIlegal = true;
	if(wasIlegal)
	{
		painter->setPen(Qt::white);
		painter->drawText(0, 100, painter->viewport().width() - 2, painter->viewport().height() / 8 + 52 , Qt::AlignCenter, QString("Ilegal"));
	}

	/*
	painter->setFont(QFont("Helvetica", 14, QFont::Bold));
	painter->setPen(Qt::black);
	painter->drawText(0, 0, painter->viewport().width() - 2, painter->viewport().height() / 8 + 52 , Qt::AlignCenter, QString(numberString));
	painter->setPen(Qt::red);
	painter->drawText(0, 0, painter->viewport().width(), painter->viewport().height() / 8 + 54, Qt::AlignCenter, QString(numberString));
	*/
}

void GameMaze::MouseMoveEvent ( int xMouse, int yMouse )
{
	if(state != STATE_STOPPED)
	{
		lastMouseX = xMouse;
		lastMouseY = yMouse;
	}
}
void GameMaze::MousePressEvent ( int xMouse, int yMouse )
{
	if(state == STATE_RUNNING)
	{
		int mouseXID = (int) (xMouse / tileWidth);
		int mouseYID = (int) (yMouse / tileHeight);

		int playerTurn = currentState->FindTileToExplore(mouseXID, mouseYID);

		if(playerTurn >= 0)
		{
			player[GetCurrentState()->GetActivePlayerID()]->HumanTurn(playerTurn);
		}
	}
}

IGameState * GameMaze::GetCurrentState() const
{
	return currentState;
}

#include <QSpinBox>

vector<pair<QWidget *, QString> > GameMaze::GetSetupWidget()
{
	vector<pair<QWidget *, QString> > widgets;

	QSpinBox * spinBoxWidth = new QSpinBox();
	spinBoxWidth->setMinimum(5);
	spinBoxWidth->setMaximum(100);
	spinBoxWidth->setValue(mazeWidth);
	widgets.push_back(pair<QWidget *, QString>(spinBoxWidth, QString("Maze width")));
	connect(spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(SetMazeWidth(int)));

	QSpinBox * spinBoxHeight = new QSpinBox();
	spinBoxHeight->setMinimum(5);
	spinBoxHeight->setMaximum(100);
	spinBoxHeight->setValue(mazeHeight);
	widgets.push_back(pair<QWidget *, QString>(spinBoxHeight, QString("Maze height")));
	connect(spinBoxHeight, SIGNAL(valueChanged(int)), this, SLOT(SetMazeHeight(int)));

	QSpinBox * spinBoxStep = new QSpinBox();
	spinBoxStep->setMinimum(100);
	spinBoxStep->setMaximum(10000);
	spinBoxStep->setValue(stepsToGameOver);
	widgets.push_back(pair<QWidget *, QString>(spinBoxStep, QString("Max steps")));
	connect(spinBoxStep, SIGNAL(valueChanged(int)), this, SLOT(SetStepsToGameOver(int)));

	return widgets;
}

void GameMaze::SetMazeWidth(int width)
{
	mazeWidth = width;
}

void GameMaze::SetMazeHeight(int height)
{
	mazeHeight = height;
}

void GameMaze::SetStepsToGameOver(int steps)
{
	stepsToGameOver = steps;
}
