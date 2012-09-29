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

	playerX = 0;
	playerY = 0;
	maze[playerY][playerX] = TILE_EMPTY;
	maze[playerY + 1][playerX] = TILE_EMPTY;
	maze[playerY + 1][playerX + 1] = TILE_WALL;
	maze[playerY][playerX + 1] = TILE_EMPTY;

	tileToExplore.push_back(Pos2Dto1D(playerX + 1, playerY));
	tileToExplore.push_back(Pos2Dto1D(playerX, playerY + 1));
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

void MazeState::Explore(int tileToExploreID)
{
	switch(activePlayerID)
	{
		case PLAYER_AI :
			ExplorePlayer(tileToExploreID);
			break;
		case ENVINRONMENT_AI :
			ExploreEnvironment(tileToExploreID);
			break;
	}

	activePlayerID++;
	if(activePlayerID > 1)
		activePlayerID = 0;
}

void MazeState::ExplorePlayer(int tileToExploreID)
{
	Pos1Dto2D(tileToExplore[tileToExploreID], &playerX, &playerY);
	tileToExplore.erase(tileToExplore.begin() + tileToExploreID, tileToExplore.begin() + tileToExploreID + 1);

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
		hallSize = 7 - (pseudoRnd % 4);
	}
}

void MazeState::ExploreEnvironment(int turn)
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
		return;
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
			hole = turn / (hallSize - 1);
			sign = 1;
		} else if(loop1 == 2)
		{
			hole = turn % (hallSize - 1);
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
				if(loop2 == hallSize -1)
					maze[y][x] = TILE_WALL;
				else
					maze[y][x] = TILE_EMPTY;
			} else {
				if(GetTile(x, y) == TILE_UNDEFINED)
				{
					if(x == hX && y == hY)
					{
						maze[y][x] = TILE_EMPTY;
						if(GetTile(x + holeX * sign, y + holeY * sign) == TILE_UNDEFINED)
						{
							tileToExplore.push_back(Pos2Dto1D(x, y));
						}
					} else {
						maze[y][x] = TILE_WALL;
					}
				}
			}
			x += dx;
			y += dy;
		}
	}

	RemoveNonviableTileToExplore();
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
		   GetTile(x, y - 1) != TILE_UNDEFINED)
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
