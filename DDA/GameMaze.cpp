#include "GameMaze.h"
#include "EnvironmentAIBasic.h"
#include "Human.h"
#include "PlayerRandomAI.h"

GameMaze::GameMaze(QWidget * _widget) : Game(_widget)
{
	currentState = new MazeState(1);
	tileWidth = 10.0f;
	tileHeight = 10.0f;
}

void GameMaze::StartGame()
{
	if(currentState != NULL)
	{
		delete currentState;
	}

	if(player == NULL)
	{
		player = new IPlayer*[2];
		player[ENVINRONMENT_AI] = new EnvironmentAIBasic();
		player[PLAYER_AI] = new PlayerRandomAI();

		QObject::connect(player[PLAYER_AI], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
		QObject::connect(player[ENVINRONMENT_AI], SIGNAL(ImReady(void)),
                         this, SLOT(PlayerIsReady(void)));
	} 

	activePlayerID = 1;
	currentState = new MazeState(activePlayerID);
	playerCount = 2;
	player[ENVINRONMENT_AI]->StartGame(this);
	player[PLAYER_AI]->StartGame(this);
	isRunning = true;
	if(player[activePlayerID]->Think())
		NextTurn();
}

bool GameMaze::PlayerTurn()
{
	int playerTurn = player[activePlayerID]->MakeTurn();
	currentState->Explore(playerTurn);

	if(activePlayerID == ENVINRONMENT_AI && currentState->GetTileToExplore()->size() == 0)
		return false;

	return true;
}

GameMaze::~GameMaze()
{
	delete currentState;
}

void GameMaze::Draw(QPainter * painter, int tickMillis)
{
	int mazeWidth = currentState->GetMazeWidth();
	int mazeHeight = currentState->GetMazeHeight();
	MazeTile ** maze = currentState->GetMaze();
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
			}
			painter->fillRect(loop2 * tileWidth, loop1 * tileHeight, tileWidth + 1, tileHeight + 1, painter->brush());
		}
	}
	
	painter->setBrush(QBrush(QColor(200, 200, 10)));
	painter->drawEllipse(currentState->GetPlayerX() * tileWidth, 
			                currentState->GetPlayerY() * tileHeight, 
							tileWidth, tileHeight);

	painter->setBrush(QBrush(QColor(200, 10, 10, 100)));
	for(int loop1 = 0; loop1 < tileToExplore->size(); loop1++)
	{
		int x, y;
		currentState->Pos1Dto2D((*tileToExplore)[loop1], &x, &y);
		painter->drawRect(x * tileWidth + 1, 
			              y * tileHeight + 1, 
						  tileWidth - 2, tileHeight - 2);
	}

	int mouseXID = (int) (lastMouseX / tileWidth);
	int mouseYID = (int) (lastMouseY / tileHeight);
	painter->setBrush(QBrush(QColor(200, 200, 10, 100)));
	painter->drawRect(mouseXID * tileWidth, 
			             mouseYID * tileHeight, 
							tileWidth, tileHeight);
}

void GameMaze::MouseMoveEvent ( QMouseEvent * event )
{
	if(isRunning)
	{
		lastMouseX = event->x();
		lastMouseY = event->y();
	}
}
void GameMaze::MousePressEvent ( QMouseEvent * event )
{
	if(isRunning)
	{
		int mouseXID = (int) (lastMouseX / tileWidth);
		int mouseYID = (int) (lastMouseY / tileHeight);

		int playerTurn = currentState->FindTileToExplore(mouseXID, mouseYID);

		if(playerTurn >= 0)
		{
			player[activePlayerID]->HumanTurn(playerTurn);
		}
	}
}

int * GameMaze::GetPlayerChoises()
{
	int * a = new int;
	*a = currentState->GetPlayerChoises();
	return a;
}