
#include <queue>
#include <QDebug>
#include "MazeState.h"
#include "MazeStat.h"
#include "MatrixFactory.h"

using namespace std;

MazeState::MazeState(int _activePlayerID, int _stepsToGameOver, int mWidth, int mHeight, int _visibleGoals, bool _abstraction, Game * _game) : IGameState(_game)
{
	playerCount = 2;
	mazeWidth = mWidth;
	mazeHeight = mHeight;
	activePlayerID = _activePlayerID;
	stepsToGameOver = _stepsToGameOver;
	visibleGoals = _visibleGoals;
	abstraction = _abstraction;

	firstRnd = rand() % 100 + 10;

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

	for(int loop1 = 0; loop1 < CRED_PIECE_MAX; loop1++)
		credibility[loop1] = 0;

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
		int MAX_TRIES = 250;
		int tries = 0;
		do {
			tempX = 2 + rand() % (mazeWidth / 2 - 4) + ((tries % 4  == 0 || tries % 4 == 1) ? 0 : (mazeWidth / 2));
			tempY = 2 + rand() % (mazeHeight / 2 - 4) + ((tries % 4  == 1 || tries % 4 == 2) ? 0 : (mazeHeight / 2));

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
		} while(maze[tempY][tempX] != TILE_UNDEFINED || minDist <= 3);

		if(tries > MAX_TRIES)
		{
			goalAmount = goalStart = loop1;
			break;
		}

		goalX[loop1] = tempX;
		goalY[loop1] = tempY;
		maze[tempY][tempX] = TILE_GOAL;

		//if(loop1 == 0)
		{
			maze[tempY + 1][tempX + 1] = TILE_WALL;
			maze[tempY - 1][tempX + 1] = TILE_WALL;
			maze[tempY + 1][tempX - 1] = TILE_WALL;
			maze[tempY - 1][tempX - 1] = TILE_WALL;
			//maze[tempY + 1][tempX] = TILE_WALL;
			//maze[tempY - 1][tempX] = TILE_WALL;
			//maze[tempY][tempX + 1] = TILE_WALL;
			//maze[tempY][tempX - 1] = TILE_WALL;
		}
		switch(rand() % 4)
		{
			case 0 : maze[tempY + 1][tempX] = TILE_UNDEFINED; break;
			case 1 : maze[tempY - 1][tempX] = TILE_UNDEFINED; break;
			case 2 : maze[tempY][tempX + 1] = TILE_UNDEFINED; break;
			case 3 : maze[tempY][tempX - 1] = TILE_UNDEFINED; break;
		}
	}
	
	hallSize = 0;

	isRankUpToDate = false;
	UpdateGameStat();
}

MazeState::MazeState(const MazeState & origin)
{
	IGameState::Init(origin.game, origin.gameStat);
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
	lastPlayerID = origin.lastPlayerID;
	leaderTime = origin.leaderTime;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
		pStatus[loop1] = origin.pStatus[loop1];

	mazeWidth = origin.mazeWidth;
	mazeHeight = origin.mazeHeight;
	visibleGoals = origin.visibleGoals;
	activePlayerID = origin.activePlayerID;
	playerX = origin.playerX;
	playerY = origin.playerY;

	for(int loop1 = 0; loop1 < GOAL_MAX; loop1++)
	{
		goalX[loop1] = origin.goalX[loop1];
		goalY[loop1] = origin.goalY[loop1];
	}
	firstRnd = origin.firstRnd;
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

	for(int loop1 = 0; loop1 < CRED_PIECE_MAX; loop1++)
		credibility[loop1] = origin.credibility[loop1];

	tileToExplore = origin.tileToExplore;
	environmentTurns = origin.environmentTurns;
	environmentTurnsAbstract = origin.environmentTurnsAbstract;
	environmentTurnsReal = origin.environmentTurnsReal;
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
	environmentTurns.clear();
	environmentTurnsAbstract.clear();
	environmentTurnsReal.clear();
}

IGameState ** MazeState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	IGameState ** nextState;
	MazeState * mazeState;

	int choises = GetPlayerChoises(whoAskID);
	int realTurn;
	if(activePlayerID == ENVINRONMENT_AI)
	{
		vector<IGameState *> tempState;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			mazeState = GetNextStateWithID(loop1, &realTurn);
			if(abstraction)
				environmentTurnsReal[loop1] = realTurn;

			if(mazeState == NULL || !mazeState->IsPossibleWayToGoal())
			{
				if(mazeState != NULL)
					delete mazeState;

				tempState.push_back(NULL);
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

IGameState * MazeState::SimulateToTheEnd(int whoAskID)
{
	MazeState * state = new MazeState(*this);

	while(!state->IsGameOver())
	{
		state->Explore(rand() % GetPlayerChoises(whoAskID));
	}
	
	return state;
}


MazeState * MazeState::GetNextStateWithID(int turnID, int * outRealTurn)
{
	*outRealTurn = 0;

	MazeState * mazeState = NULL;
	if(!abstraction || activePlayerID == 1)
	{
		mazeState = new MazeState(*this);
		mazeState->Explore(turnID);
		*outRealTurn = turnID;
	} else {
		int start;
		if(turnID == 0)
			start = 0;
		else
			start = environmentTurnsAbstract[turnID - 1];
		int end = environmentTurnsAbstract[turnID];

		for(int loop1 = start; loop1 < end; loop1++)
		{
			if(mazeState != NULL)
				delete mazeState;

			mazeState = new MazeState(*this);
			mazeState->Explore(loop1);
			*outRealTurn = loop1;

			if(mazeState->IsPossibleWayToGoal())
				break;
		}
	}

	return mazeState;
}

IGameState * MazeState::GetRandomNextState(int whoAskID, int * outStateID)
{
	int numberNextStates = GetPlayerChoises(whoAskID);
	int turn;
	int realTurn;
	MazeState * mazeState = NULL;

	turn = rand() % numberNextStates;	
	mazeState = GetNextStateWithID(turn, &realTurn);
	if(abstraction && activePlayerID == ENVINRONMENT_AI)
		environmentTurnsReal[turn] = realTurn; 

	if(activePlayerID == ENVINRONMENT_AI && (mazeState == NULL || !mazeState->IsPossibleWayToGoal()))
	{
		vector<int> notExploredTurns;

		int end = (abstraction) ? environmentTurnsAbstract.size() : environmentTurns.size();
		for(int loop1 = 0; loop1 < end; loop1++)
		{
			notExploredTurns.push_back(loop1);
		}

		do {
			int rndNumber = rand() % notExploredTurns.size(); // DIVIZION BY ZERO!!!!!
			if(mazeState != NULL)
				delete mazeState;
			
			turn = notExploredTurns[rndNumber];
			notExploredTurns.erase(notExploredTurns.begin() + rndNumber, notExploredTurns.begin() + rndNumber + 1);

			mazeState = GetNextStateWithID(turn, &realTurn);
			if(abstraction)
				environmentTurnsReal[turn] = realTurn;

		} while(mazeState == NULL || !mazeState->IsPossibleWayToGoal());
	}

	*outStateID = turn;
	return mazeState;
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
		if(maze[goalY[loop1]][goalX[loop1]] != TILE_GOAL)
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

			if(maze[y][x] == TILE_GOAL) // you cant go throug goals
				continue;

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
		{
			if(loop1 == 0)
			{
				return false;
			} else {
				SetWallFrom(goalX[loop1], goalY[loop1]);
				goalAmount--;
			}
			
		}
	}

	return true;
}

void MazeState::SetWallFrom(int x, int y)
{
	maze[y][x] = TILE_WALL;
	if(GetTile(x + 1, y) == TILE_UNDEFINED)
		SetWallFrom(x + 1, y);
	if(GetTile(x - 1, y) == TILE_UNDEFINED)
		SetWallFrom(x - 1, y);
	if(GetTile(x, y + 1) == TILE_UNDEFINED)
		SetWallFrom(x, y + 1);
	if(GetTile(x, y - 1) == TILE_UNDEFINED)
		SetWallFrom(x, y - 1);
}
bool MazeState::RealExplore(int tileToExploreID)
{
	if(abstraction && activePlayerID == ENVINRONMENT_AI)
		return Explore(environmentTurnsReal[tileToExploreID]);
	else
		return Explore(tileToExploreID);
}

bool MazeState::Explore(int tileToExploreID)
{
	bool gameOver = false;
	bool twoUndefined;
	int wallX, wallY;
	switch(activePlayerID)
	{
		case PLAYER_AI :
			gameOver = ExplorePlayer(tileToExploreID);
			FindNonRedundantTurns();
			break;
		case ENVINRONMENT_AI :
			gameOver = ExploreEnvironment(environmentTurns[tileToExploreID], &twoUndefined, &wallX, &wallY);
			if(twoUndefined)
			{
				if(!IsStateLegal())
				{
					SetTileEmpty(wallX, wallY);
					AddCloseDoor(wallX, wallY);
					RemoveNonviableTileToExplore();
				}
			}
			possibleWayToGoal = IsStateLegal();
			break;
	}

	if(GetTile(playerX, playerY) == TILE_GOAL ||
	   GetTile(playerX + 1, playerY) == TILE_GOAL ||
	   GetTile(playerX - 1, playerY) == TILE_GOAL ||
	   GetTile(playerX, playerY + 1) == TILE_GOAL ||
	   GetTile(playerX, playerY - 1) == TILE_GOAL)
	{
		maze[playerY][playerX] = TILE_EMPTY;
	} else {
		activePlayerID++;
		if(activePlayerID > 1)
			activePlayerID = 0;
	}

	isRankUpToDate = false;

	UpdateGameStat();

	return gameOver;
}

int MazeState::GetDistanceBetween(int pos1X, int pos1Y, int pos2X, int pos2Y, bool undefined)
{
	vector<int> pos11D;
	pos11D.push_back(Pos2Dto1D(pos1X, pos1Y));
	return GetDistanceBetween(pos11D, pos2X, pos2Y, undefined);
}
int MazeState::GetDistanceBetween(vector<int> pos11D, int pos2X, int pos2Y, bool undefined)
{
	priority_queue<QueueNode, vector<QueueNode>, CompareNode> q;

	QueueNode tempNode(0, 0, 0, 0, 0);
	for(int loop1 = 0; loop1 < pos11D.size(); loop1++)
	{
		int pos1X, pos1Y;
		Pos1Dto2D(pos11D[loop1], &pos1X, &pos1Y);
		tempNode = QueueNode(pos1X, pos1Y, 0, pos2X, pos2Y);
		q.push(tempNode);
	}

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

		if(x > 0 && (maze[y][x-1] == TILE_EMPTY || maze[y][x-1] == TILE_GOAL || maze[y][x-1] == TILE_DOOR || (maze[y][x-1] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y][x-1])
		{
			mazeClosedList[y][x-1] = CLOSE;
			q.push(QueueNode(x - 1, y, depth + 1, pos2X, pos2Y));
		}

		if(x < mazeWidth - 1 && (maze[y][x+1] == TILE_EMPTY || maze[y][x+1] == TILE_GOAL || maze[y][x+1] == TILE_DOOR || (maze[y][x+1] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y][x+1])
		{
			mazeClosedList[y][x+1] = CLOSE;
			q.push(QueueNode(x + 1, y, depth + 1, pos2X, pos2Y));
		}
		if(y > 0 && (maze[y-1][x] == TILE_EMPTY || maze[y-1][x] == TILE_GOAL || maze[y - 1][x] == TILE_DOOR || (maze[y-1][x] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y - 1][x])
		{
			mazeClosedList[y - 1][x] = CLOSE;
			q.push(QueueNode(x, y - 1, depth + 1, pos2X, pos2Y));
		}

		if(y < mazeHeight - 1 && (maze[y+1][x] == TILE_EMPTY || maze[y+1][x] == TILE_GOAL || maze[y + 1][x] == TILE_DOOR || (maze[y+1][x] == TILE_UNDEFINED && undefined)) && OPEN == mazeClosedList[y + 1][x])
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
	int distance = GetDistanceBetween(newPlayerX, newPlayerY, playerX, playerY);
	stepsToGameOver -= distance;
	
	playerX = newPlayerX;
	playerY = newPlayerY;
	tileToExplore.erase(tileToExplore.begin() + tileToExploreID, tileToExplore.begin() + tileToExploreID + 1);

	if(GetTile(playerX, playerY) == TILE_GOAL)
	{
		goalAmount--;
		maze[playerY][playerX] = TILE_EMPTY;
		if(goalAmount > 0 && goalX[0] == playerX && goalY[0] == playerY)
		{
			for(int loop1 = 1; loop1 < goalStart; loop1++)
			{
				if(maze[goalY[loop1]][goalX[loop1]] == TILE_GOAL)
				{
					int temp = goalX[loop1];
					goalX[loop1] = goalX[0];
					goalX[0] = temp;

					temp = goalY[loop1];
					goalY[loop1] = goalY[0];
					goalY[0] = temp;
				}
			}
		}

		if(goalAmount == 0)
			return true;
		else
			return false;
	} else {
		maze[playerY][playerX] = TILE_EMPTY;
		stepsToGameOver += GetDoorKind(playerX, playerY);
	}

	if(stepsToGameOver <= 0)
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

int MazeState::GetDoorKind(int x, int y)
{
	switch((x * 55 / firstRnd + y * firstRnd) % 5)
	{
		case 0:
		case 3:
		case 4:
			return DOOR_NORMAL;
		case 1:
			return DOOR_GOOD;
		case 2:
			return DOOR_BAD;
	}


	return DOOR_NORMAL;
}
void MazeState::AddGoalToTileToExploreIfNeighbours(int x, int y)
{
	bool added = false;
	if(GetTile(x - 1, y) == TILE_GOAL)
	{
		added = true;
		SetGoalNeightboursWall(x - 1, y);
		AddCloseDoor(x - 1, y);
	}
	if(GetTile(x + 1, y) == TILE_GOAL)
	{
		added = true;
		SetGoalNeightboursWall(x + 1, y);
		AddCloseDoor(x + 1, y);
	}
	if(GetTile(x, y - 1) == TILE_GOAL)
	{
		added = true;
		SetGoalNeightboursWall(x, y - 1);
		AddCloseDoor(x, y - 1);
	}
	if(GetTile(x, y + 1) == TILE_GOAL)
	{
		added = true;
		SetGoalNeightboursWall(x, y + 1);
		AddCloseDoor(x, y + 1);
	}
	if(added)
		RemoveNonviableTileToExplore();
}

void MazeState::FindNonRedundantTurns()
{
	int dx = 0, dy = 0;
	int holeX = 0, holeY = 0;
	
	environmentTurns.clear();
	environmentTurnsAbstract.clear();
	environmentTurnsReal.clear();

	vector<int> helpVector;

	for(int loop1 = 0; loop1 < hallSize; loop1++)
	{
		helpVector.push_back(loop1);
	}

	if(hallSize == 1)
	{
		helpVector.push_back(0 + hallSize);
		helpVector.push_back(1 + hallSize);
		helpVector.push_back(2 + hallSize);
		helpVector.push_back(3 + hallSize);
	} 
	random_shuffle(helpVector.begin(), helpVector.end());
	environmentTurns.insert(environmentTurns.end(), helpVector.begin(), helpVector.end());
	environmentTurnsAbstract.push_back(environmentTurns.size());
	helpVector.clear();

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
	int abstractWide = max(2, hallSize / 4);
	int abstractCopyWhen = hallSize % abstractWide;

	int maxTurn = hallSizePlus1 * hallSizePlus1;

	for(int loop3 = 0; loop3 < abstractWide; loop3++)
	{
		for(int loop4 = 0; loop4 < abstractWide; loop4++)
		{		
			for(int loop1 = loop3 * abstractWide; loop1 < (loop3 + 1) * abstractWide && loop1 < hallSizePlus1; loop1++)
			{
				int hole1 = loop1 - 1;
				int h1X = playerX + (1 + hole1) * dx + holeX;
				int h1Y = playerY + (1 + hole1) * dy + holeY;

				if((hole1 >= 0 && GetTile(h1X, h1Y) == TILE_UNDEFINED) || hole1 < 0)
				{
					for(int loop2 = loop4 * abstractWide; loop2 < (loop4 + 1) * abstractWide && loop2 < hallSizePlus1; loop2++)
					{
						int hole2 = loop2 - 1;
						int h2X = playerX + (1 + hole2) * dx - holeX;
						int h2Y = playerY + (1 + hole2) * dy - holeY;
						if(((hole2 >= 0 && GetTile(h2X, h2Y) == TILE_UNDEFINED) || hole2 < 0))
						{
							int turn = loop2 + loop1 * hallSizePlus1;
							helpVector.push_back(turn + hallSize);
							helpVector.push_back(turn + hallSize + MAKE_DOOR_IN_THE_END);
						}
					}
				}
			}

			if(helpVector.size() > 0)
			{
				random_shuffle(helpVector.begin(), helpVector.end());
				environmentTurns.insert(environmentTurns.end(), helpVector.begin(), helpVector.end());
				environmentTurnsAbstract.push_back(environmentTurns.size());
				helpVector.clear();
			}
		}
	}
	environmentTurnsReal.insert(environmentTurnsReal.begin(), environmentTurnsAbstract.begin(), environmentTurnsAbstract.end());
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

bool MazeState::ExploreEnvironment(int turn, bool * twoUndefined, int * wallX, int * wallY)
{
	if(turn >= MAKE_DOOR_IN_THE_END)
	{
		turn -= MAKE_DOOR_IN_THE_END;
		setupOpenHallEnds = OPEN_HALL_ENDS_ALWAYS;
	} else {
		setupOpenHallEnds = OPEN_HALL_ENDS_NEVER;
	}

	int dx = 0, dy = 0;
	int holeX = 0, holeY = 0;
	*twoUndefined = false;
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
			*twoUndefined = true;
			maze[playerY][playerX + 1] = TILE_WALL;
			*wallX = playerX + 1;
			*wallY = playerY;
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
			*twoUndefined = true;
			maze[playerY - 1][playerX] = TILE_WALL;
			*wallX = playerX;
			*wallY = playerY - 1;
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
			*twoUndefined = true;
			maze[playerY + 1][playerX] = TILE_WALL;
			*wallX = playerX;
			*wallY = playerY + 1;
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
	
	int credID = (realHallSize + 1) / CRED_HALL_PIECE_SIZE;
	if(credID >= CRED_PIECE_MAX)
		credID = CRED_PIECE_MAX - 1;
	credibility[credID]++;

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
	AddGoalToTileToExploreIfNeighbours(x, y);
}

void MazeState::RemoveNonviableTileToExplore()
{
	for(int loop1 = 0; loop1 < tileToExplore.size(); loop1++)
	{
		int tile = tileToExplore[loop1];
		int x, y;
		bool delIt = false;
		Pos1Dto2D(tile, &x, &y);
		if(GetTile(x + 1, y) == TILE_GOAL ||
			GetTile(x - 1, y) == TILE_GOAL ||
			GetTile(x, y + 1) == TILE_GOAL ||
			GetTile(x, y - 1) == TILE_GOAL)
		{
			SetGoalNeightboursWall(x, y);
			delIt = true;
		}

		int ways = 0;
		if(GetTile(x + 1, y) == TILE_EMPTY || GetTile(x + 1, y) == TILE_UNDEFINED || GetTile(x + 1, y) == TILE_DOOR)
			ways++;
		if(GetTile(x - 1, y) == TILE_EMPTY || GetTile(x - 1, y) == TILE_UNDEFINED || GetTile(x - 1, y) == TILE_DOOR)
			ways++;
		if(GetTile(x, y + 1) == TILE_EMPTY || GetTile(x, y + 1) == TILE_UNDEFINED || GetTile(x, y + 1) == TILE_DOOR)
			ways++;
		if(GetTile(x, y - 1) == TILE_EMPTY || GetTile(x, y - 1) == TILE_UNDEFINED || GetTile(x, y - 1) == TILE_DOOR)
			ways++;

		if(ways <= 1 && GetDoorKind(x, y) != DOOR_GOOD && GetTile(x, y) != TILE_GOAL)
		{
			delIt = true;
		}

		if(delIt)
		{
			maze[y][x] = TILE_EMPTY;
			tileToExplore.erase(tileToExplore.begin() + loop1, tileToExplore.begin() + loop1 + 1);
			loop1--;
		}
	}
	return;

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

int MazeState::GetPlayerStatus(int playerID)
{
	int undefinedTiles = 0;
	int sumDist = 0;
	int tempDist = 0;

	for(int loop1 = 0; loop1 < goalStart; loop1++)
	{
		if(maze[goalY[loop1]][goalX[loop1]] != TILE_GOAL)
			continue;

		tempDist = GetDistanceBetween(goalX[loop1], goalY[loop1], playerX, playerY, true);

		if(maze[goalY[loop1] + 1][goalX[loop1]] == TILE_EMPTY ||
		   maze[goalY[loop1] - 1][goalX[loop1]] == TILE_EMPTY ||
		   maze[goalY[loop1]][goalX[loop1] + 1] == TILE_EMPTY ||
		   maze[goalY[loop1]][goalX[loop1] - 1] == TILE_EMPTY)
		{
			sumDist += tempDist;
		} else {
			sumDist += tempDist * 2;
		}
	}
	
	for(int loop1 = 0; loop1 < mazeWidth; loop1++)
	{
		for(int loop2 = 0; loop2 < mazeHeight; loop2++)
		{
			if(maze[loop2][loop1] == TILE_UNDEFINED)
				undefinedTiles++;
		}
	}
	
	float undefinedCover = ((float) undefinedTiles) / (mazeWidth * mazeHeight);
	


	int koef = (maze[goalY[0]][goalX[0]] == TILE_GOAL) ? 1 : 0;
	int status = (int) (sumDist * koef - stepsToGameOver + goalAmount * 50);
	return (playerID == 0) ? status : -status;
}

void MazeState::CountRank()
{
	if(!possibleWayToGoal || (tileToExplore.size() == 0 && activePlayerID == PLAYER_AI))
	{
		playerRank = IGameState::ILLEGAL_GAME;
		return;
	} 

	if(visibleGoals)
	{
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
	} else {
		playerRank = stepsToGameOver + (goalStart - goalAmount) * 1000;
		if(GetTile(playerX - 1, playerY) == TILE_UNDEFINED ||
		   GetTile(playerX + 1, playerY) == TILE_UNDEFINED ||
		   GetTile(playerX, playerY + 1) == TILE_UNDEFINED ||
		   GetTile(playerX, playerY - 1) == TILE_UNDEFINED)
		{
			playerRank += 9;
		} else {
			int x, y;
			int minDist = GetDistanceBetween(tileToExplore, playerX, playerY, false);
			playerRank -= minDist * 2;
		}
	}

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
	if(maze[goalY[0]][goalX[0]] == TILE_EMPTY)
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
			if(loop1 == playerY && loop2 == playerX)
				c = '$' ;

			putc(c, fw);
		}
		fprintf(fw, "\n");
	}

	fclose(fw);
}

void MazeState::AddCloseDoor(int x, int y)
{
	if(maze[y][x] != TILE_GOAL)
		maze[y][x] = TILE_DOOR;

	tileToExplore.push_back(Pos2Dto1D(x, y));
}

float MazeState::GetCredibility()
{
	return MatrixFactory::Inst()->Credibility(credibility, CRED_PIECE_MAX - 1) + credibility[CRED_PIECE_MAX -1] * 10000;
}

float MazeState::Freedom()
{
	int freedom = 0;
	for(int loop1 = 0; loop1 < tileToExplore.size(); loop1++)
	{
		int x, y;
		Pos1Dto2D(tileToExplore[loop1], &x, &y);
		if(GetTile(x - 1, y) == TILE_UNDEFINED ||
		   GetTile(x + 1, y) == TILE_UNDEFINED ||
		   GetTile(x, y + 1) == TILE_UNDEFINED ||
		   GetTile(x, y - 1) == TILE_UNDEFINED)
		{
			freedom++;
		}
	}

	return freedom;
}

int MazeState::GetPlayerChoises(int whoAskID) 
	{ 
		int playerChoises;
		switch(activePlayerID)
		{
			case ENVINRONMENT_AI :
				if(abstraction)
				{
					return environmentTurnsAbstract.size();
				} else {
					return environmentTurns.size();
				}
				break;
			case PLAYER_AI :
				return tileToExplore.size();
				break;
		}

		return -1;
	}
