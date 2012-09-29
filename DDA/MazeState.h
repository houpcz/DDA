#ifndef _MAZESTATE_H_
#define _MAZESTATE_H_

#include <vector>

using namespace std;

typedef enum MazeTile 
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_UNDEFINED,
	TILE_NO
};

enum {
	ENVINRONMENT_AI,
	PLAYER_AI
};

class MazeState
{
private:
	MazeTile ** maze;
	int mazeWidth, mazeHeight;
	int activePlayerID;
	int playerX, playerY;
	vector<int> tileToExplore;
	int hallSize;

	MazeTile GetTile(int x, int y);
	void CopyToMe(const MazeState & origin);
	void ClearMe();
	void ExplorePlayer(int tileToExploreID);
	void ExploreEnvironment(int turn);
	void RemoveNonviableTileToExplore();
public:
	MazeState(int _activePlayerID, int mWidth = 20, int mHeight = 20);
	MazeState(const MazeState & origin);
	MazeState& operator=(const MazeState &origin);
	~MazeState(void);

	void SetActivePlayerID(int _activePlayerID) { activePlayerID = _activePlayerID; }
	int GetPlayerX() const { return playerX; }
	int GetPlayerY() const { return playerY; }
	MazeTile ** GetMaze() const { return maze; }
	int GetMazeWidth() const { return mazeWidth; }
	int GetMazeHeight() const { return mazeHeight; }
	int GetEnvironmentChoises() const 
	{ 
		int environmentChoises = (hallSize - 1) * (hallSize - 1);
		return (environmentChoises == 0) ? 1 : environmentChoises; 
	}
	const vector<int>* GetTileToExplore() const { return &tileToExplore; }
	int FindTileToExplore(int x, int y);

	void Explore(int tileToExploreID);

	int Pos2Dto1D(int x, int y);
	void Pos1Dto2D(int d1, int * x, int * y);
};

#endif
