#ifndef _MAZESTATE_H_
#define _MAZESTATE_H_

#include <vector>
#include "IGameState.h"

using namespace std;

typedef enum MazeTile 
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_UNDEFINED,
	TILE_GOAL,
	TILE_NO
};

enum {
	ENVINRONMENT_AI,
	PLAYER_AI
};

struct QueueNode
{
	int x;
	int y;
	int depth;

	public :
		QueueNode(int _x, int _y, int _depth) {
			x = _x;
			y = _y;
			depth = _depth;
		}
};

class MazeState : public IGameState
{
private:
	MazeTile ** maze;
	bool ** mazeClosedList;
	int mazeWidth, mazeHeight;
	int activePlayerID;
	int playerX, playerY;
	int goalX, goalY;
	vector<int> tileToExplore;
	int hallSize;
	int stepsToGameOver;
	bool possibleWayToGoal;

	MazeTile GetTile(int x, int y);
	void CopyToMe(const MazeState & origin);
	void ClearMe();
	bool ExplorePlayer(int tileToExploreID);
	bool ExploreEnvironment(int turn);
	void RemoveNonviableTileToExplore();
	void SetTileEmpty(int x, int y);
	// undefined - can go through undefined files
	int GetDistanceBetween(int pos1X, int pos1Y, int pos2X, int pos2Y, bool undefined = false);
public:
	MazeState(int _activePlayerID, int _stepsToGameOver = 500, int mWidth = 40, int mHeight = 40);
	MazeState(const MazeState & origin);
	MazeState& operator=(const MazeState &origin);
	~MazeState(void);

	void SetActivePlayerID(int _activePlayerID) { activePlayerID = _activePlayerID; }
	int GetPlayerX() const { return playerX; }
	int GetPlayerY() const { return playerY; }
	int GetActivePlayerID() const {
		return activePlayerID;
	}
	MazeTile ** GetMaze() const { return maze; }
	int GetMazeWidth() const { return mazeWidth; }
	int GetMazeHeight() const { return mazeHeight; }
	int GetStepsToGameOver() const { return stepsToGameOver; }
	int GetPlayerChoises() const 
	{ 
		int playerChoises;
		switch(activePlayerID)
		{
			case ENVINRONMENT_AI :
				playerChoises = hallSize * hallSize;
				return (playerChoises <= 1) ? 2 : playerChoises; 
				break;
			case PLAYER_AI :
				return tileToExplore.size();
				break;
		}

		return -1;
	}
	int GetPlayerScore(int playerID) const;
	IGameState ** GetNextStates(int *outNumberNextStates) const;
	const vector<int>* GetTileToExplore() const { return &tileToExplore; }
	int FindTileToExplore(int x, int y);

	bool Explore(int tileToExploreID);

	int Pos2Dto1D(int x, int y);
	void Pos1Dto2D(int d1, int * x, int * y);
};

#endif
