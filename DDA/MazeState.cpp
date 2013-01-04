
#include <queue>
#include <QDebug>
#include "MazeState.h"
#include "MatrixFactory.h"

using namespace std;

MazeState::MazeState(int _activePlayerID, int _stepsToGameOver, int mWidth, int mHeight)
{
	mazeWidth = mWidth;
	mazeHeight = mHeight;
	activePlayerID = _activePlayerID;
	stepsToGameOver = _stepsToGameOver;

	possibleWayToGoal = true;
	maze = MatrixFactory::Inst()->GetMatrix(mazeWidth, mazeHeight);
	mazeClosedList = MatrixFactory::Inst()->GetMatrix(mazeWidth, mazeHeight);
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			maze[loop1][loop2] = TILE_UNDEFINED;
		}
	}

	int dx, dy;
	switch(rand() % 4)
	{
		case 0 :
			playerX = 0;
			playerY = 0;
			dx = 1;
			dy = 1;
			break;
		case 1 :
			playerX = mazeWidth - 1;
			playerY = mazeHeight - 1;
			dx = -1;
			dy = -1;
			break;
		case 2 :
			playerX = mazeWidth - 1;
			playerY = 0;
			dx = -1;
			dy = 1;
			break;
		case 3 :
			playerX = 0;
			playerY = mazeHeight - 1;
			dx = 1;
			dy = -1;
			break;
	}
	maze[playerY][playerX] = TILE_EMPTY;
	maze[playerY + dy][playerX] = TILE_EMPTY;
	maze[playerY + dy][playerX + dx] = TILE_WALL;
	maze[playerY][playerX + dx] = TILE_EMPTY;

	//goalX = mazeHeight / 2;
	//goalY = mazeWidth / 2;
	goalX = mazeWidth - playerX - 1;
	goalY = mazeHeight - playerY - 1;
	maze[goalY][goalX] = TILE_GOAL;

	tileToExplore.push_back(Pos2Dto1D(playerX + dx, playerY));
	tileToExplore.push_back(Pos2Dto1D(playerX, playerY + dy));
	hallSize = 0;

	CountScore();
}

MazeState::MazeState(const MazeState & origin)
{
	CopyToMe(origin);
}

MazeState& MazeState::operator=(const MazeState &origin)
{
	if(&origin != this)
	{
		ClearMe();
		CopyToMe(origin);
	}

	return *this;
}

void MazeState::CopyToMe(const MazeState & origin)
{
	mazeWidth = origin.mazeWidth;
	mazeHeight = origin.mazeHeight;
	activePlayerID = origin.activePlayerID;
	playerX = origin.playerX;
	playerY = origin.playerY;
	goalX = origin.goalX;
	goalY = origin.goalY;
	hallSize = origin.hallSize;
	stepsToGameOver = origin.stepsToGameOver;
	possibleWayToGoal = origin.possibleWayToGoal;

	maze = MatrixFactory::Inst()->GetMatrix(mazeWidth, mazeHeight);
	mazeClosedList = MatrixFactory::Inst()->GetMatrix(mazeWidth, mazeHeight);

	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			mazeClosedList[loop1][loop2] = false;
			maze[loop1][loop2] = origin.maze[loop1][loop2];
		}
	}

	tileToExplore = origin.tileToExplore;
	nonRedundantTurns = origin.nonRedundantTurns;
}


MazeState::~MazeState(void)
{
	ClearMe();
}

void MazeState::ClearMe()
{
	MatrixFactory::Inst()->ReturnMatrix(maze, mazeWidth, mazeHeight);
	MatrixFactory::Inst()->ReturnMatrix(mazeClosedList, mazeWidth, mazeHeight);
	/*
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		delete[] mazeClosedList[loop1];
		delete[] maze[loop1];
	}
	delete[] mazeClosedList;
	delete[] maze;
	*/
	tileToExplore.clear();
}

IGameState ** MazeState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	IGameState ** nextState;
	MazeState * mazeState;

	if(activePlayerID == ENVINRONMENT_AI)
	{
		nextState = new IGameState*[nonRedundantTurns.size()];
		for(int loop1 = 0; loop1 < nonRedundantTurns.size(); loop1++)
		{
			mazeState = new MazeState(*this);
			mazeState->Explore(loop1);
			nextState[loop1] = mazeState;
		}
		*outNumberNextStates = nonRedundantTurns.size();
	} else {	
		int numberNextStates = GetPlayerChoises(whoAskID);
		nextState = new IGameState*[numberNextStates];
		for(int loop1 = 0; loop1 < numberNextStates; loop1++)
		{
			mazeState = new MazeState(*this);
			mazeState->Explore(loop1);
			nextState[loop1] = mazeState;
		}
		*outNumberNextStates = numberNextStates;
	}

	return nextState;
}

int MazeState::FindTileToExplore(int x, int y)
{
	int tempX, tempY;

	for(int loop1 = 0; loop1 < tileToExplore.size(); loop1++)
	{
		Pos1Dto2D(tileToExplore[loop1], &tempX, &tempY);
		if(x == tempX && y == tempY)
		{
			return loop1;
		}
	}

	return -1;
}

bool MazeState::IsStateLegal()
{
	return GetDistanceBetween(playerX, playerY, goalX, goalY, true) >= 0;
}

bool MazeState::Explore(int tileToExploreID)
{
	bool gameOver = false;

	switch(activePlayerID)
	{
		case PLAYER_AI :
			gameOver = ExplorePlayer(tileToExploreID);
			FindNonRedundantTurns();
			break;
		case ENVINRONMENT_AI :
			gameOver = ExploreEnvironment(nonRedundantTurns[tileToExploreID]);
			break;
	}

	possibleWayToGoal = IsStateLegal();

	activePlayerID++;
	if(activePlayerID > 1)
		activePlayerID = 0;

	CountScore();

	return gameOver;
}

int MazeState::GetDistanceBetween(int pos1X, int pos1Y, int pos2X, int pos2Y, bool undefined)
{
	priority_queue<QueueNode, vector<QueueNode>, CompareNode> q;
	QueueNode tempNode = QueueNode(pos1X, pos1Y, 0, pos2X, pos2Y);
	q.push(tempNode);

	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			mazeClosedList[loop1][loop2] = OPEN;
		}
	}

	while(!q.empty())
	{
		tempNode = q.top();
		q.pop();
		int x = tempNode.x;
		int y = tempNode.y;
		int depth = tempNode.g;
		if(x == pos2X && y == pos2Y)
		{
			return depth;
		}

		if(x > 0 && (maze[y][x-1] == TILE_EMPTY || maze[y][x-1] == TILE_GOAL || (maze[y][x-1] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y][x-1])
		{
			mazeClosedList[y][x-1] = CLOSE;
			q.push(QueueNode(x - 1, y, depth + 1, pos2X, pos2Y));
		}

		if(x < mazeWidth - 1 && (maze[y][x+1] == TILE_EMPTY || maze[y][x+1] == TILE_GOAL || (maze[y][x+1] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y][x+1])
		{
			mazeClosedList[y][x+1] = CLOSE;
			q.push(QueueNode(x + 1, y, depth + 1, pos2X, pos2Y));
		}
		if(y > 0 && (maze[y-1][x] == TILE_EMPTY || maze[y-1][x] == TILE_GOAL || (maze[y-1][x] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y - 1][x])
		{
			mazeClosedList[y - 1][x] = CLOSE;
			q.push(QueueNode(x, y - 1, depth + 1, pos2X, pos2Y));
		}

		if(y < mazeHeight - 1 && (maze[y+1][x] == TILE_EMPTY || maze[y+1][x] == TILE_GOAL || (maze[y+1][x] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y + 1][x])
		{
			mazeClosedList[y + 1][x] = CLOSE;
			q.push(QueueNode(x, y + 1, depth + 1, pos2X, pos2Y));
		}
	}

	return -1;
}

bool MazeState::ExplorePlayer(int tileToExploreID)
{
	int newPlayerX, newPlayerY;
	Pos1Dto2D(tileToExplore[tileToExploreID], &newPlayerX, &newPlayerY);
	int distance = GetDistanceBetween(playerX, playerY, newPlayerX, newPlayerY);
	stepsToGameOver -= distance;
	if(stepsToGameOver <= 0)
		return true;

	playerX = newPlayerX;
	playerY = newPlayerY;
	tileToExplore.erase(tileToExplore.begin() + tileToExploreID, tileToExplore.begin() + tileToExploreID + 1);
	if(GetTile(playerX, playerY) == TILE_GOAL)
		return true;

	int dx = 0, dy = 0;
	if(GetTile(playerX - 1, playerY) == TILE_UNDEFINED)
	{
		dx = -1;
	} else if(GetTile(playerX + 1, playerY) == TILE_UNDEFINED)
	{
		dx = 1;
	} else if(GetTile(playerX, playerY - 1) == TILE_UNDEFINED)
	{
		dy = -1;
	} else if(GetTile(playerX, playerY + 1) == TILE_UNDEFINED)
	{
		dy = 1;
	}

	int x = playerX + dx, y = playerY + dy;
	hallSize = 0;
	while(GetTile(x, y) == TILE_UNDEFINED)
	{
		hallSize++;
		x += dx;
		y += dy;
	}

	int pseudoRnd = playerX + playerY + hallSize;
	/*
	if(hallSize > 7)
	{
		hallSize = 7 - (pseudoRnd % 5);
	}*/

	return false;
}

void MazeState::FindNonRedundantTurns()
{
	int dx = 0, dy = 0;
	int holeX = 0, holeY = 0;
	
	nonRedundantTurns.clear();

	for(int loop1 = 0; loop1 < hallSize; loop1++)
	{
		nonRedundantTurns.push_back(loop1);
	}

	if(hallSize == 1)
	{
		nonRedundantTurns.push_back(0 + hallSize);
		nonRedundantTurns.push_back(1 + hallSize);
	}

	if(GetTile(playerX - 1, playerY) == TILE_UNDEFINED)
	{
		dx = -1;
		holeY = 1;
	} else
	if(GetTile(playerX + 1, playerY) == TILE_UNDEFINED)
	{
		dx = 1;
		holeY = 1;
	} else
	if(GetTile(playerX, playerY - 1) == TILE_UNDEFINED)
	{
		dy = -1;
		holeX = 1;
	} else
	if(GetTile(playerX, playerY + 1) == TILE_UNDEFINED)
	{
		dy = 1;
		holeX = 1;
	}

	int hallSizePlus1 = hallSize + 1;
	int maxTurn = hallSizePlus1 * hallSizePlus1;
	for(int loop1 = 0; loop1 < hallSizePlus1; loop1++)
	{
		int hole1 = loop1 - 1;
		int h1X = playerX + (1 + hole1) * dx + holeX;
		int h1Y = playerY + (1 + hole1) * dy + holeY;

		if((hole1 >= 0 && GetTile(h1X, h1Y) == TILE_UNDEFINED) || hole1 < 0)
		{
			for(int loop2 = 0; loop2 < hallSizePlus1; loop2++)
			{
				int hole2 = loop2 - 1;
				int h2X = playerX + (1 + hole2) * dx - holeX;
				int h2Y = playerY + (1 + hole2) * dy - holeY;
				if(((hole2 >= 0 && GetTile(h2X, h2Y) == TILE_UNDEFINED) || hole2 < 0))
				{
					int turn = loop2 + loop1 * hallSizePlus1;
					nonRedundantTurns.push_back(turn + hallSize);
				}
			}
		}
	}
}

void MazeState::ExploreHallSize1(int dx, int dy, int holeX, int holeY, int turn)
{
	if(GetTile(playerX + dx, playerY + dy) == TILE_UNDEFINED)
	{
		if(GetTile(playerX + holeX + dx, playerY + holeY + dy) == TILE_UNDEFINED &&
			GetTile(playerX - holeX + dx, playerY - holeY + dy) == TILE_UNDEFINED)
		{
			switch(turn % 2)
			{
				case 0 :
					maze[playerY - holeY + dy][playerX - holeX + dx] = TILE_WALL;
					break;
				case 1 :
					maze[playerY + holeY + dy][playerX + holeX + dx] = TILE_WALL;
					break;
			}
		}

		SetTileEmpty(playerX + dx, playerY + dy);
		if(GetTile(playerX + holeX + dx, playerY + holeY + dy) == TILE_UNDEFINED ||
			GetTile(playerX - holeX + dx, playerY - holeY + dy) == TILE_UNDEFINED)
		{
			tileToExplore.push_back(Pos2Dto1D(playerX + dx, playerY + dy));
		}
		RemoveNonviableTileToExplore();
	}
}

bool MazeState::ExploreEnvironment(int turn)
{
	int dx = 0, dy = 0;
	int holeX = 0, holeY = 0;
	bool isOneUndefinedTile = false;
	if(GetTile(playerX - 1, playerY) == TILE_UNDEFINED)
	{
		dx = -1;
		holeY = 1;
		isOneUndefinedTile = true;
	} 
	
	if(GetTile(playerX + 1, playerY) == TILE_UNDEFINED)
	{
		if(!isOneUndefinedTile)
		{
			dx = 1;
			holeY = 1;
			isOneUndefinedTile = true;
		} else {
			SetTileEmpty(playerX + 1, playerY);
			tileToExplore.push_back(Pos2Dto1D(playerX + 1, playerY));
		}
	} 
	
	if(GetTile(playerX, playerY - 1) == TILE_UNDEFINED)
	{
		if(!isOneUndefinedTile)
		{
			dy = -1;
			holeX = 1;
			isOneUndefinedTile= true;
		} else {
			SetTileEmpty(playerX, playerY - 1);
			tileToExplore.push_back(Pos2Dto1D(playerX, playerY - 1));
		}
	} 
	
	if(GetTile(playerX, playerY + 1) == TILE_UNDEFINED)
	{
		if(!isOneUndefinedTile)
		{
			dy = 1;
			holeX = 1;
			isOneUndefinedTile = true;
		} else {
			SetTileEmpty(playerX, playerY + 1);
			tileToExplore.push_back(Pos2Dto1D(playerX, playerY + 1));
		}
	}

	int hole1;
	int hole2;
	int realHallSize;
	if(turn < hallSize)
	{
		hole1 = -1;
		hole2 = -1;
		realHallSize = min(turn + 2, hallSize);
	} else {
		turn -= hallSize;
		hole1 = turn / (hallSize + 1) - 1;
		hole2 = turn % (hallSize + 1) - 1;
		realHallSize = min(max(hole1, hole2) + 2, hallSize);
	}
	
	//hallSize = min(max(hole1, hole2) + 2, hallSize);

	if(hallSize == 1)
	{
		ExploreHallSize1(dx, dy, holeX, holeY, turn);
		return false;
	}

	int sign;
	for(int loop1 = 0; loop1 < 3; loop1++)
	{
		int x = playerX + dx;
		int y = playerY + dy;
		int hX, hY;
		int hole;
		if(loop1 == 0)
		{
			hole = hole1;
			sign = 1;
		} else if(loop1 == 2)
		{
			hole = hole2;
			sign = -1;
		} else {
			hole = 0;
			sign = 0;
		}
		hX = playerX + (1 + hole) * dx + holeX * sign;
		hY = playerY + (1 + hole) * dy + holeY * sign;
		x += holeX * sign;
		y += holeY * sign;
		for(int loop2 = 0; loop2 < realHallSize; loop2++)
		{
			if(loop1 == 1)
			{
				if(loop2 == realHallSize -1 && loop2 != hallSize - 1 && GetTile(x + dx, y + dy) == TILE_UNDEFINED)
					maze[y][x] = TILE_WALL;
				else {
					SetTileEmpty(x, y);
				}
			} else {
				if(GetTile(x, y) == TILE_UNDEFINED)
				{
					if(x == hX && y == hY && 
						(loop2 != realHallSize - 1 || loop2 == hallSize - 1 || GetTile(x + dx, y + dy) == TILE_NO))
					{
						SetTileEmpty(x, y);
						//if(GetTile(x + holeX * sign, y + holeY * sign) == TILE_UNDEFINED)
						{
							tileToExplore.push_back(Pos2Dto1D(x, y));
						}
					} else {
						maze[y][x] = TILE_WALL;
					}
				} else if(GetTile(x, y) == TILE_GOAL)
				{
					SetGoalNeightboursWall(x, y);
					tileToExplore.push_back(Pos2Dto1D(x, y));
				}
			}
			x += dx;
			y += dy;
		}
	}

	RemoveNonviableTileToExplore();

	return false;
}

void MazeState::SetGoalNeightboursWall(int x, int y)
{
	if(GetTile(x + 1, y) == TILE_UNDEFINED) maze[y][x + 1] = TILE_WALL;
	if(GetTile(x - 1, y) == TILE_UNDEFINED) maze[y][x - 1] = TILE_WALL;
	if(GetTile(x, y + 1) == TILE_UNDEFINED) maze[y + 1][x] = TILE_WALL;
	if(GetTile(x, y - 1) == TILE_UNDEFINED) maze[y - 1][x] = TILE_WALL;
	if(GetTile(x + 1, y + 1) == TILE_UNDEFINED) maze[y + 1][x + 1] = TILE_WALL;
	if(GetTile(x - 1, y - 1) == TILE_UNDEFINED) maze[y - 1][x - 1] = TILE_WALL;
	if(GetTile(x - 1, y + 1) == TILE_UNDEFINED) maze[y + 1][x - 1] = TILE_WALL;
	if(GetTile(x + 1, y - 1) == TILE_UNDEFINED) maze[y - 1][x + 1] = TILE_WALL;
}

void MazeState::SetTileEmpty(int x, int y)
{
	maze[y][x] = TILE_EMPTY;
	if(GetTile(x - 1, y) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x - 1, y);
		tileToExplore.push_back(Pos2Dto1D(x - 1, y));
	}
	if(GetTile(x + 1, y) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x + 1, y);
		tileToExplore.push_back(Pos2Dto1D(x + 1, y));
	}
	if(GetTile(x, y - 1) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x, y - 1);
		tileToExplore.push_back(Pos2Dto1D(x, y - 1));
	}
	if(GetTile(x, y + 1) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x, y + 1);
		tileToExplore.push_back(Pos2Dto1D(x, y + 1));
	}

}

void MazeState::RemoveNonviableTileToExplore()
{
	for(int loop1 = 0; loop1 < tileToExplore.size(); loop1++)
	{
		int tile = tileToExplore[loop1];
		int x, y;
		Pos1Dto2D(tile, &x, &y);
		if(GetTile(x + 1, y) != TILE_UNDEFINED &&
		   GetTile(x - 1, y) != TILE_UNDEFINED &&
		   GetTile(x, y + 1) != TILE_UNDEFINED &&
		   GetTile(x, y - 1) != TILE_UNDEFINED &&
		   GetTile(x, y) != TILE_GOAL)
		{
			tileToExplore.erase(tileToExplore.begin() + loop1, tileToExplore.begin() + loop1 + 1);
			loop1--;
		} else {
			if(GetTile(x + 1, y) == TILE_GOAL ||
			   GetTile(x - 1, y) == TILE_GOAL ||
			   GetTile(x, y + 1) == TILE_GOAL ||
			   GetTile(x, y - 1) == TILE_GOAL)
			{
				SetGoalNeightboursWall(x, y);
				tileToExplore.erase(tileToExplore.begin() + loop1, tileToExplore.begin() + loop1 + 1);
				loop1--;
			}
		}
	}
}

char MazeState::GetTile(int x, int y)
{
	if(x >= 0 && x < mazeWidth &&
	   y >= 0 && y < mazeHeight)
	{
		return maze[y][x];
	}

	return TILE_NO;
}

int MazeState::Pos2Dto1D(int x, int y)
{
	return y * mazeWidth + x;
}

void MazeState::Pos1Dto2D(int d1, int * x, int * y)
{
	*x = d1 % mazeWidth;
	*y = d1 / mazeWidth;
}

int MazeState::GetPlayerScore(int playerID, int whoAskID)
{
	return playerScore;
}
void MazeState::CountScore()
{
	if(!possibleWayToGoal || (tileToExplore.size() == 0 && activePlayerID == PLAYER_AI))
	{
		playerScore = IGameState::ILLEGAL_GAME;
		return;
	} 

	int manDistToGoal = -GetDistanceBetween(goalX, goalY, playerX, playerY, true);
	playerScore = manDistToGoal * 10 + stepsToGameOver;
}

bool MazeState::IsGameOver()
{
	if(goalX == playerX && goalY == playerY)
		return true;
	if(stepsToGameOver <= 0)
		return true;
	if(activePlayerID == PLAYER_AI && tileToExplore.size() == 0)
		return true;

	return false;
}

void MazeState::PrintToFile(const char * firstLine)
{
	FILE *fw;
	fw = fopen("log.txt", "a");

	fprintf(fw, "%s %d\n\n", firstLine, tileToExplore.size());
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			int c = '@';
			switch((MazeTile) maze[loop1][loop2])
			{
				case MazeTile::TILE_EMPTY : c = '.'; break;
				case MazeTile::TILE_GOAL : c = 'G'; break;
				case MazeTile::TILE_NO : c = 'N'; break;
				case MazeTile::TILE_UNDEFINED : c = '?'; break;
				case MazeTile::TILE_WALL : c = '#'; break;
			}
			putc(c, fw);
		}
		fprintf(fw, "\n");
	}

	fclose(fw);
}
