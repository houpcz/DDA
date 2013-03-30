#include "GameMaze.h"
#include "EnvironmentAIBasic.h"
#include "Human.h"
#include "PlayerRandomAI.h"
#include "PlayerHillClimber.h"
#include "Qt\qcheckbox.h"

GameMaze::GameMaze(QWidget * _widget, bool _paint) : Game(_widget, _paint)
{
	minPlayerAI = 1;
	maxPlayerAI = 1;

	mazeWidth = 41;
	mazeHeight = 41; 
	visibleGoals = false;
	abstraction = true;
	stepsToGameOver = 1000;
	
	tileWidth = 10.0f;
	tileHeight = 10.0f;

	playerCount = 2;
	
	player = new IPlayer*[playerCount];
	player[ENVINRONMENT_AI] = new EnvironmentAIBasic(ENVINRONMENT_AI);
	if(paint)
	{
		player[PLAYER_AI] = new Human(PLAYER_AI);
	} else {
		player[PLAYER_AI] = new PlayerHillClimber(PLAYER_AI);
	}

	QObject::connect(player[PLAYER_AI], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));
	QObject::connect(player[ENVINRONMENT_AI], SIGNAL(ImReady(void)),
                        this, SLOT(PlayerIsReady(void)));

	currentState = new MazeState(1, stepsToGameOver, mazeWidth, mazeHeight, visibleGoals, abstraction, this);
}

void GameMaze::StartGame()
{
	Game::StartGame();

	if(currentState != NULL)
	{
		delete currentState;
	}

	currentState = new MazeState(PLAYER_AI, stepsToGameOver, mazeWidth, mazeHeight, visibleGoals, abstraction, this);
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
	bool gameOver = currentState->RealExplore(playerTurn);

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

	int viewportWidth = painter->viewport().width();
	int viewportHeight = painter->viewport().height();
	painter->fillRect(0, 0, viewportWidth, viewportHeight, QBrush(QColor(100, 120, 100)));
	tileWidth = viewportWidth / (float) mazeWidth;
	tileHeight =  viewportHeight / (float) mazeHeight;

	tileWidth = min(tileWidth, tileHeight);
	tileHeight = min(tileWidth, tileHeight);
	
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			int whatDraw = maze[loop1][loop2];
			if(!currentState->VisibleGoals() && 
			   !currentState->IsGameOver() &&
			   (currentState->GetTile(loop2 - 1, loop1) != TILE_EMPTY && currentState->GetTile(loop2 - 1, loop1) != TILE_DOOR) &&
			   (currentState->GetTile(loop2, loop1 - 1) != TILE_EMPTY && currentState->GetTile(loop2, loop1 - 1) != TILE_DOOR) &&
			   (currentState->GetTile(loop2 + 1, loop1) != TILE_EMPTY && currentState->GetTile(loop2 + 1, loop1) != TILE_DOOR) &&
			   (currentState->GetTile(loop2, loop1 + 1) != TILE_EMPTY && currentState->GetTile(loop2, loop1 + 1) != TILE_DOOR))/* &&
			   currentState->GetTile(loop2 - 1, loop1 - 1) != TILE_EMPTY &&
			   currentState->GetTile(loop2 + 1, loop1 - 1) != TILE_EMPTY &&
			   currentState->GetTile(loop2 + 1, loop1 + 1) != TILE_EMPTY &&
			   currentState->GetTile(loop2 - 1, loop1 + 1) != TILE_EMPTY)*/
			   whatDraw = TILE_UNDEFINED;

			switch(whatDraw)
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
				case TILE_DOOR :
					painter->setBrush(QBrush(QColor(180, 180, 160)));
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
			switch(currentState->GetDoorKind(x, y))
			{
				case DOOR_NORMAL :
					color = QColor(238, 154, 77); break;
				case DOOR_GOOD :
					color = QColor(77, 77, 200); break;
				case DOOR_BAD :
					color = QColor(200, 77, 77); break;
			}
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

	int score = currentState->GetPlayerStatus(1);
	sprintf(numberString, "%d", score);

	static bool wasIlegal = false;
	if(score == IGameState::ILLEGAL_GAME)
		wasIlegal = true;
	if(wasIlegal)
	{
		painter->setPen(Qt::white);
		painter->drawText(0, 100, painter->viewport().width() - 2, painter->viewport().height() / 8 + 52 , Qt::AlignCenter, QString("Ilegal"));
	}

	
	painter->setFont(QFont("Helvetica", 14, QFont::Bold));
	painter->setPen(Qt::black);
	painter->drawText(tileWidth * mazeWidth + 8, 0, tileWidth * mazeWidth + 200, painter->viewport().height() / 8 + 52 , Qt::AlignLeft, QString(numberString));
	painter->setPen(Qt::red);
	painter->drawText(tileWidth * mazeWidth + 10, 0, tileWidth * mazeWidth + 202, painter->viewport().height() / 8 + 54, Qt::AlignLeft, QString(numberString));
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

pair<QString,QString> GameMaze::GetSetupString()
{
	QString header = QString("Maze Width;Maze Height;Max Steps;");
	QString values = QString::number(mazeWidth) + QString(";") + QString::number(mazeHeight) + ";" + QString::number(stepsToGameOver);
	return pair<QString, QString>(header, values);
}

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
	spinBoxStep->setMinimum(10);
	spinBoxStep->setMaximum(10000);
	spinBoxStep->setValue(stepsToGameOver);
	widgets.push_back(pair<QWidget *, QString>(spinBoxStep, QString("Max steps")));
	connect(spinBoxStep, SIGNAL(valueChanged(int)), this, SLOT(SetStepsToGameOver(int)));

	QCheckBox * visibleGoalsBox = new QCheckBox();
	visibleGoalsBox->setChecked(visibleGoals);
	widgets.push_back(pair<QWidget *, QString>(visibleGoalsBox, QString("Visible bombs")));
	connect(visibleGoalsBox, SIGNAL(stateChanged(int)), this, SLOT(SetVisibleGoals(int)));

	QCheckBox * abstractionBox = new QCheckBox();
	abstractionBox->setChecked(abstraction);
	widgets.push_back(pair<QWidget *, QString>(abstractionBox, QString("State abstraction")));
	connect(abstractionBox, SIGNAL(stateChanged(int)), this, SLOT(SetAbstraction(int)));

	return widgets;
}

void GameMaze::SetAbstraction(int state)
{
	abstraction = state;
}

void GameMaze::SetVisibleGoals(int state)
{
	visibleGoals = state;
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
