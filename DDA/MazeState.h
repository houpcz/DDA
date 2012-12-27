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
	int g;
	int h;
	int f;

	public :
		QueueNode(int _x, int _y, int _g, int goalX, int goalY) {
			x = _x;
			y = _y;
			g = _g;
			int dx = x - goalX;
			int dy = y - goalY;
			h = dx * dx + dy * dy;
			//h = abs(dx) + abs(dy);
			f = h + g;
		}
};

class CompareNode
{  
public:
  bool operator()(const QueueNode& l, const QueueNode& r)  
  {  
      return l.f > r.f;  
  }  
}; 


class MazeState : public IGameState
{
private:
	static const char OPEN = ' ';
	static const char CLOSE = '#';

	char ** maze;
	char ** mazeClosedList;
	int mazeWidth, mazeHeight;
	int activePlayerID;
	int playerX, playerY;
	int goalX, goalY;
	int playerScore;
	vector<int> tileToExplore;
	int hallSize;
	int stepsToGameOver;
	bool possibleWayToGoal;

	char GetTile(int x, int y);
	void CopyToMe(const MazeState & origin);
	void ClearMe();
	bool ExplorePlayer(int tileToExploreID);
	bool ExploreEnvironment(int turn);
	void RemoveNonviableTileToExplore();
	void SetTileEmpty(int x, int y);
	// undefined - can go through undefined files
	int GetDistanceBetween(int pos1X, int pos1Y, int pos2X, int pos2Y, bool undefined = false);
	void CountScore();
public:
	MazeState(int _activePlayerID, int _stepsToGameOver = 5000, int mWidth = 41, int mHeight = 41);
	MazeState(const MazeState & origin);
	MazeState& operator=(const MazeState &origin);
	virtual ~MazeState(void);

	void SetActivePlayerID(int _activePlayerID) { activePlayerID = _activePlayerID; }
	int GetPlayerX() const { return playerX; }
	int GetPlayerY() const { return playerY; }
	int GetActivePlayerID() const {
		return activePlayerID;
	}
	char ** GetMaze() const { return maze; }
	int GetMazeWidth() const { return mazeWidth; }
	int GetMazeHeight() const { return mazeHeight; }
	int GetStepsToGameOver() const { return stepsToGameOver; }
	int GetPlayerChoises(int whoAskID) 
	{ 
		int playerChoises;
		switch(activePlayerID)
		{
			case ENVINRONMENT_AI :
				playerChoises = (hallSize + 1) * (hallSize + 1);
				return (playerChoises <= 1) ? 2 : playerChoises; 
				break;
			case PLAYER_AI :
				return tileToExplore.size();
				break;
		}

		return -1;
	}
	int GetPlayerScore(int playerID, int whoAskID);
	IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	const vector<int>* GetTileToExplore() const { return &tileToExplore; }
	int FindTileToExplore(int x, int y);

	bool Explore(int tileToExploreID);

	int Pos2Dto1D(int x, int y);
	void Pos1Dto2D(int d1, int * x, int * y);
	void PrintToFile(const char * firstLine);
};

#endif
