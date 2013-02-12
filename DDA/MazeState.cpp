
#include <queue>
#include <QDebug>
#include "MazeState.h"
#include "MazeStat.h"
#include "MatrixFactory.h"

using namespace std;

MazeState::MazeState(int _activePlayerID, int _stepsToGameOver, int mWidth, int mHeight)
{
	mazeWidth = mWidth;
	mazeHeight = mHeight;
	activePlayerID = _activePlayerID;
	stepsToGameOver = _stepsToGameOver;

	setupOpenHallEnds = OPEN_HALL_ENDS_ALWAYS;
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

	playerX = mazeWidth / 2;
	playerY = mazeHeight / 2;
	startX = playerX;
	startY = playerY;
	maze[playerY][playerX] = TILE_EMPTY;
	maze[playerY + 1][playerX] = TILE_EMPTY;
	maze[playerY - 1][playerX] = TILE_EMPTY;
	maze[playerY][playerX + 1] = TILE_EMPTY;
	maze[playerY][playerX - 1] = TILE_EMPTY;
	maze[playerY + 1][playerX + 1] = TILE_WALL;
	maze[playerY - 1][playerX - 1] = TILE_WALL;
	maze[playerY - 1][playerX + 1] = TILE_WALL;
	maze[playerY + 1][playerX - 1] = TILE_WALL;
	AddCloseDoor(playerX + 1, playerY);
	AddCloseDoor(playerX - 1, playerY);
	AddCloseDoor(playerX, playerY + 1);
	AddCloseDoor(playerX, playerY - 1);

	goalStart = GOAL_MAX;
	goalAmount = goalStart;
	int tempX;
	int tempY;
	for(int loop1 = 0; loop1 < goalAmount; loop1++)
	{
		int minDist;
		int MAX_TRIES = 150;
		int tries = 0;
		do {
			tempX = rand() % (mazeWidth / 2) + ((tries % 4  == 0 || tries % 4 == 1) ? 0 : (mazeWidth / 2));
			tempY = rand() % (mazeHeight / 2) + ((tries % 4  == 1 || tries % 4 == 2) ? 0 : (mazeHeight / 2));

			minDist = min(abs(playerX - tempX) + 1, abs(playerY - tempY) + 1);
			for(int loop2 = 0; loop2 < loop1; loop2++)
			{
				int temp;
				temp = abs(goalX[loop2] - tempX);
				if(temp < minDist)
					minDist = temp;
				temp = abs(goalY[loop2] - tempY);
				if(temp < minDist)
					minDist = temp;
			}
			tries++;
			if(tries > MAX_TRIES)
				break;
		} while(maze[tempY][tempX] != TILE_UNDEFINED || minDist <= 4);

		if(tries > MAX_TRIES)
		{
			goalAmount = goalStart = loop1;
			break;
		}

		goalX[loop1] = tempX;
		goalY[loop1] = tempY;
		maze[tempY][tempX] = TILE_GOAL;

		/*
		maze[tempY + 1][tempX + 1] = TILE_WALL;
		maze[tempY - 1][tempX + 1] = TILE_WALL;
		maze[tempY + 1][tempX - 1] = TILE_WALL;
		maze[tempY - 1][tempX - 1] = TILE_WALL;
		maze[tempY + 1][tempX] = TILE_WALL;
		maze[tempY - 1][tempX] = TILE_WALL;
		maze[tempY][tempX + 1] = TILE_WALL;
		maze[tempY][tempX - 1] = TILE_WALL;

		switch(rand() % 4)
		{
			case 0 : maze[tempY + 1][tempX] = TILE_UNDEFINED; break;
			case 1 : maze[tempY - 1][tempX] = TILE_UNDEFINED; break;
			case 2 : maze[tempY][tempX + 1] = TILE_UNDEFINED; break;
			case 3 : maze[tempY][tempX - 1] = TILE_UNDEFINED; break;
		}
		*/
	}
	
	hallSize = 0;

	isRankUpToDate = false;
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

	for(int loop1 = 0; loop1 < GOAL_MAX; loop1++)
	{
		goalX[loop1] = origin.goalX[loop1];
		goalY[loop1] = origin.goalY[loop1];
	}
	goalAmount = origin.goalAmount;
	goalStart = origin.goalStart;
	startX = origin.startX;
	startY = origin.startY;
	hallSize = origin.hallSize;
	playerRank = origin.playerRank;
	stepsToGameOver = origin.stepsToGameOver;
	possibleWayToGoal = origin.possibleWayToGoal;
	setupOpenHallEnds = origin.setupOpenHallEnds;
	isRankUpToDate = origin.isRankUpToDate;
	goalAmount = origin.goalAmount;

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
	tileToExplore.clear();
	nonRedundantTurns.clear();
}

IGameState ** MazeState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	IGameState ** nextState;
	MazeState * mazeState;

	int choises = GetPlayerChoises(whoAskID);

	if(activePlayerID == ENVINRONMENT_AI)
	{
		vector<IGameState *> tempState;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			mazeState = new MazeState(*this);
			mazeState->Explore(loop1);
			if(!mazeState->IsPossibleWayToGoal())
			{
				delete mazeState;
				nonRedundantTurns.erase(nonRedundantTurns.begin() + loop1, nonRedundantTurns.begin() + (loop1 + 1));
				loop1--;
				choises--;
			} else {
				tempState.push_back(mazeState);
			}
		}
		choises = tempState.size();
		nextState = new IGameState*[choises];
		for(int loop1 = 0; loop1 < choises; loop1++)
			nextState[loop1] = tempState[loop1];
		tempState.clear();
	} else {
		nextState = new IGameState*[choises];
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			mazeState = new MazeState(*this);
			mazeState->Explore(loop1);
			nextState[loop1] = mazeState;
		}
	}
	*outNumberNextStates = choises;
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
	priority_queue<QueueNode, vector<QueueNode>, CompareNode> q;
	int pos1X;
	int pos1Y;

	for(int loop1 = 0; loop1 < goalStart; loop1++)
	{
		if(maze[goalY[loop1]][goalX[loop1]] == TILE_EMPTY)
			continue;
		while(!q.empty())
			q.pop();

		QueueNode tempNode(0, 0, 0, 0, 0);

		for(int loop2 = 0; loop2 < tileToExplore.size(); loop2++)
		{
			int pos = tileToExplore[loop2];
			Pos1Dto2D(pos, &pos1X, &pos1Y);
			tempNode = QueueNode(pos1X, pos1Y, 0, goalX[loop1], goalY[loop1]);
			q.push(tempNode);
		}

		for(int loop3 = 0; loop3 < mazeHeight; loop3++)
		{
			for(int loop2 = 0; loop2 < mazeWidth; loop2++)
			{
				mazeClosedList[loop3][loop2] = OPEN;
			}
		}

		bool isLegal = false;
		while(!q.empty())
		{
			tempNode = q.top();
			q.pop();
			int x = tempNode.x;
			int y = tempNode.y;
			int depth = tempNode.g;
			if(x == goalX[loop1] && y == goalY[loop1])
			{
				isLegal = true;
				break;
			}

			if(x > 0 && (maze[y][x-1] == TILE_UNDEFINED || maze[y][x-1] == TILE_GOAL) && OPEN == mazeClosedList[y][x-1])
			{
				mazeClosedList[y][x-1] = CLOSE;
				q.push(QueueNode(x - 1, y, 0, goalX[loop1], goalY[loop1]));
			}

			if(x < mazeWidth - 1 && (maze[y][x+1] == TILE_UNDEFINED || maze[y][x+1] == TILE_GOAL) && OPEN == mazeClosedList[y][x+1])
			{
				mazeClosedList[y][x+1] = CLOSE;
				q.push(QueueNode(x + 1, y, 0, goalX[loop1], goalY[loop1]));
			}
			if(y > 0 && (maze[y-1][x] == TILE_GOAL || maze[y-1][x] == TILE_UNDEFINED) && OPEN == mazeClosedList[y - 1][x])
			{
				mazeClosedList[y - 1][x] = CLOSE;
				q.push(QueueNode(x, y - 1, 0, goalX[loop1], goalY[loop1]));
			}

			if(y < mazeHeight - 1 && (maze[y+1][x] == TILE_GOAL || maze[y+1][x] == TILE_UNDEFINED) && OPEN == mazeClosedList[y + 1][x])
			{
				mazeClosedList[y + 1][x] = CLOSE;
				q.push(QueueNode(x, y + 1, 0, goalX[loop1], goalY[loop1]));
			}
		}

		if(!isLegal)
			return false;
	}

	return true;
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
			possibleWayToGoal = IsStateLegal();
			break;
	}

	if(GetTile(playerX, playerY) == TILE_GOAL)
	{
		maze[playerY][playerX] = TILE_EMPTY;
	} else {
		activePlayerID++;
		if(activePlayerID > 1)
			activePlayerID = 0;
	}

	isRankUpToDate = false;

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
	{
		goalAmount--;
		if(goalAmount == 0)
			return true;
		else
			return false;
	}

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
		nonRedundantTurns.push_back(2 + hallSize);
		nonRedundantTurns.push_back(3 + hallSize);
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
		SetTileEmpty(playerX + dx, playerY + dy);
		if(GetTile(playerX + holeX + dx, playerY + holeY + dy) == TILE_UNDEFINED)
		{
			if(turn % 2 == 0)
			{
				SetTileEmpty(playerX + holeX + dx, playerY + holeY + dy);
				AddCloseDoor(playerX + holeX + dx, playerY + holeY + dy);
			} else {
				maze[playerY + holeY + dy][playerX + holeX + dx] = TILE_WALL;
			}
		}
		if(GetTile(playerX - holeX + dx, playerY - holeY + dy) == TILE_UNDEFINED)
		{
			if(turn > 1)
			{
				SetTileEmpty(playerX - holeX + dx, playerY - holeY + dy);
				AddCloseDoor(playerX - holeX + dx, playerY - holeY + dy);
			} else {
				maze[playerY - holeY + dy][playerX - holeX + dx] = TILE_WALL;
			}
		}

		SetTileEmpty(playerX + dx, playerY + dy);

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
			AddCloseDoor(playerX + 1, playerY);
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
			AddCloseDoor(playerX, playerY - 1);
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
			AddCloseDoor(playerX, playerY + 1);
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
				{
					int rndNumber = stepsToGameOver + hole1 + hole2;
					if(setupOpenHallEnds == OPEN_HALL_ENDS_ALWAYS ||
					   (setupOpenHallEnds == OPEN_HALL_ENDS_SOMETIMES && (rndNumber & 1) == 1))
					{
						SetTileEmpty(x, y);
						AddCloseDoor(x, y);
					} else {
						maze[y][x] = TILE_WALL;
					}
				}
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
						AddCloseDoor(x, y);
					} else {
						maze[y][x] = TILE_WALL;
					}
				} else if(GetTile(x, y) == TILE_GOAL)
				{
					SetGoalNeightboursWall(x, y);
					AddCloseDoor(x, y);
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
		AddCloseDoor(x - 1, y);
	}
	if(GetTile(x + 1, y) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x + 1, y);
		AddCloseDoor(x + 1, y);
	}
	if(GetTile(x, y - 1) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x, y - 1);
		AddCloseDoor(x, y - 1);
	}
	if(GetTile(x, y + 1) == TILE_GOAL)
	{
		SetGoalNeightboursWall(x, y + 1);
		AddCloseDoor(x, y + 1);
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

int MazeState::GetPlayerRank(int playerID, int whoAskID)
{
	if(!isRankUpToDate)
		CountRank();

	return playerRank;
}
void MazeState::CountRank()
{
	if(!possibleWayToGoal || (tileToExplore.size() == 0 && activePlayerID == PLAYER_AI))
	{
		playerRank = IGameState::ILLEGAL_GAME;
		return;
	} 

	int length = 1000000;
	for(int loop1 = 0; loop1 < goalStart; loop1++)
	{
		if(goalY[loop1] == playerY && goalX[loop1] == playerX)
		{
			length = 0;
			break;
		}
		if(maze[goalY[loop1]][goalX[loop1]] == TILE_EMPTY)
			continue;

		int temp = abs(playerX - goalX[loop1]) + abs(playerY - goalY[loop1]);

		if(temp < length)
			length = temp;
	}
	if(goalAmount == 0)
		length = 0;
	//int manDistToGoal = -GetDistanceBetween(goalX, goalY, playerX, playerY, true);
	playerRank = -length * 10 + stepsToGameOver;

	isRankUpToDate = true;
}

ISpecificStat * MazeState::GetGameSpecificStat()
{
	MazeStat * mazeStat = new MazeStat();

	//int length = this->GetDistanceBetween(startX, startY, goalX, goalY, false);
	mazeStat->SetRouteLength(0);

	return mazeStat;
}

bool MazeState::IsGameOver()
{
	if(goalAmount == 0)
	{
		return true;
	}
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

void MazeState::AddCloseDoor(int x, int y)
{
	tileToExplore.push_back(Pos2Dto1D(x, y));
}

int MazeState::GetPlayerChoises(int whoAskID) 
	{ 
		int playerChoises;
		switch(activePlayerID)
		{
			case ENVINRONMENT_AI :
				return nonRedundantTurns.size();
				break;
			case PLAYER_AI :
				return tileToExplore.size();
				break;
		}

		return -1;
	}
