#include "MazeState.h"


MazeState::MazeState(int _activePlayerID, int mWidth, int mHeight)
{
	mazeWidth = mWidth;
	mazeHeight = mHeight;
	activePlayerID = _activePlayerID;

	maze = new MazeTile*[mazeHeight];
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		maze[loop1] = new MazeTile[mazeWidth];

		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			maze[loop1][loop2] = TILE_UNDEFINED;
		}
	}

	int goalX, goalY;
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

	maze[mazeHeight - playerY - 1][mazeWidth - playerX - 1] = TILE_GOAL;
	//maze[mazeHeight - playerY - dy - 1][mazeWidth - playerX - 1] = TILE_EMPTY;
	//maze[mazeHeight - playerY - dy - 1][mazeWidth - playerX - dx - 1] = TILE_WALL;
	//maze[mazeHeight - playerY - 1][mazeWidth - playerX - dx - 1] = TILE_EMPTY;

	tileToExplore.push_back(Pos2Dto1D(playerX + dx, playerY));
	tileToExplore.push_back(Pos2Dto1D(playerX, playerY + dy));
	hallSize = 0;
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

	maze = new MazeTile*[mazeHeight];
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		maze[loop1] = new MazeTile[mazeWidth];

		for(int loop2 = 0; loop2 < mazeWidth; loop2++)
		{
			maze[loop1][loop2] = origin.maze[loop1][loop2];
		}
	}

	tileToExplore = origin.tileToExplore;
}


MazeState::~MazeState(void)
{
	ClearMe();
}

void MazeState::ClearMe()
{
	for(int loop1 = 0; loop1 < mazeHeight; loop1++)
	{
		delete[] maze[loop1];
	}
	delete[] maze;

	tileToExplore.clear();
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

bool MazeState::Explore(int tileToExploreID)
{
	bool gameOver = false;

	switch(activePlayerID)
	{
		case PLAYER_AI :
			gameOver = ExplorePlayer(tileToExploreID);
			break;
		case ENVINRONMENT_AI :
			gameOver = ExploreEnvironment(tileToExploreID);
			break;
	}

	activePlayerID++;
	if(activePlayerID > 1)
		activePlayerID = 0;

	return gameOver;
}

bool MazeState::ExplorePlayer(int tileToExploreID)
{
	Pos1Dto2D(tileToExplore[tileToExploreID], &playerX, &playerY);
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
	if(hallSize > 7)
	{
		hallSize = 7 - (pseudoRnd % 5);
	}

	return false;
}

bool MazeState::ExploreEnvironment(int turn)
{
	int dx = 0, dy = 0;
	int holeX = 0, holeY = 0;
	if(GetTile(playerX - 1, playerY) == TILE_UNDEFINED)
	{
		dx = -1;
		holeY = 1;
	} else if(GetTile(playerX + 1, playerY) == TILE_UNDEFINED)
	{
		dx = 1;
		holeY = 1;
	} else if(GetTile(playerX, playerY - 1) == TILE_UNDEFINED)
	{
		dy = -1;
		holeX = 1;
	} else if(GetTile(playerX, playerY + 1) == TILE_UNDEFINED)
	{
		dy = 1;
		holeX = 1;
	}

	if(hallSize == 1)
	{
		if(GetTile(playerX + dx, playerY + dy) == TILE_UNDEFINED)
		{
			maze[playerY + dy][playerX + dx] = TILE_WALL;
			RemoveNonviableTileToExplore();
		}
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
			hole = turn / hallSize;
			sign = 1;
		} else if(loop1 == 2)
		{
			hole = turn % hallSize;
			sign = -1;
		} else {
			hole = 0;
			sign = 0;
		}
		hX = playerX + (1 + hole) * dx + holeX * sign;
		hY = playerY + (1 + hole) * dy + holeY * sign;
		x += holeX * sign;
		y += holeY * sign;
		for(int loop2 = 0; loop2 < hallSize; loop2++)
		{
			if(loop1 == 1)
			{
				if(loop2 == hallSize -1 && GetTile(x + dx, y + dy) == TILE_UNDEFINED)
					maze[y][x] = TILE_WALL;
				else {
					SetTileEmpty(x, y);
				}
			} else {
				if(GetTile(x, y) == TILE_UNDEFINED)
				{
					if(x == hX && y == hY && 
						(loop2 != hallSize - 1 || GetTile(x + dx, y + dy) == TILE_NO))
					{
						SetTileEmpty(x, y);
						if(GetTile(x + holeX * sign, y + holeY * sign) == TILE_UNDEFINED)
						{
							tileToExplore.push_back(Pos2Dto1D(x, y));
						}
					} else {
						maze[y][x] = TILE_WALL;
					}
				} else if(GetTile(x, y) == TILE_GOAL)
				{
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

void MazeState::SetTileEmpty(int x, int y)
{
	maze[y][x] = TILE_EMPTY;
	if(GetTile(x - 1, y) == TILE_GOAL)
		tileToExplore.push_back(Pos2Dto1D(x - 1, y));
	if(GetTile(x + 1, y) == TILE_GOAL)
		tileToExplore.push_back(Pos2Dto1D(x + 1, y));
	if(GetTile(x, y - 1) == TILE_GOAL)
		tileToExplore.push_back(Pos2Dto1D(x, y - 1));
	if(GetTile(x, y + 1) == TILE_GOAL)
		tileToExplore.push_back(Pos2Dto1D(x, y + 1));

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
		}
	}
}

MazeTile MazeState::GetTile(int x, int y)
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
