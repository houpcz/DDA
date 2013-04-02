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
	TILE_DOOR,
	TILE_NO
};

enum {
	ENVINRONMENT_AI,
	PLAYER_AI
};

typedef enum EDoorKind
{
	DOOR_GOOD = 10,
	DOOR_BAD = -10,
	DOOR_NORMAL = 0,
};

typedef enum EOpenHallEnds
{
	OPEN_HALL_ENDS_NEVER,
	OPEN_HALL_ENDS_SOMETIMES,
	OPEN_HALL_ENDS_ALWAYS
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
			//h = dx * dx + dy * dy;
			h = abs(dx) + abs(dy);
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
	static const int GOAL_MAX = 1;
	static const int CRED_HALL_PIECE_SIZE = 2;
	static const int CRED_PIECE_MAX = 6;
	static const int MAKE_DOOR_IN_THE_END = 10000000;
	int credibility[CRED_PIECE_MAX];
	char ** maze;
	char ** mazeClosedList;
	bool isRankUpToDate;
	int firstRnd;
	int mazeWidth, mazeHeight;
	bool visibleGoals;
	bool abstraction;
	int activePlayerID;
	int playerX, playerY;
	int startX, startY;

	int goalX[GOAL_MAX];
	int goalY[GOAL_MAX];
	int goalStart;
	int goalAmount;
	int playerRank;
	vector<int> tileToExplore;
	vector<int> environmentTurns;
	vector<int> environmentTurnsAbstract;
	vector<int> environmentTurnsReal;
	int hallSize;
	int stepsToGameOver;
	bool possibleWayToGoal;
	EOpenHallEnds setupOpenHallEnds;

	void AddCloseDoor(int x, int y);
	bool IsStateLegal();
	void CopyToMe(const MazeState & origin);
	MazeState * Clone() { MazeState * state = new MazeState(*this);  return state;};
	void ClearMe();
	bool ExplorePlayer(int tileToExploreID);
	bool ExploreEnvironment(int turn, bool * twoUndefined, int * wallX, int * wallY);
	void FindNonRedundantTurns();
	void ExploreHallSize1(int dx, int dy, int holeX, int holeY, int turn);
	void RemoveNonviableTileToExplore();
	void SetGoalNeightboursWall(int x, int y);
	void SetTileEmpty(int x, int y);

	int GetDistanceBetween(vector<int> pos11D, int pos2X, int pos2Y, bool undefined = false);
	// undefined - can go through undefined files
	int GetDistanceBetween(int pos1X, int pos1Y, int pos2X, int pos2Y, bool undefined = false);
	void AddGoalToTileToExploreIfNeighbours(int x, int y);
	void CountRank();
	bool Explore(int tileToExploreID);
	MazeState * GetNextStateWithID(int turnID, int * outRealTurn);
	IGameState * SimulateToTheEnd(int whoAskID);
public:
	MazeState(int _activePlayerID, int _stepsToGameOver, int mWidth, int mHeight, int _visibleGoals, bool _abstraction, Game * _game);
	MazeState(const MazeState & origin);
	MazeState& operator=(const MazeState &origin);
	virtual ~MazeState(void);

	float FreedomLimit() { return 10.0f; };
	ISpecificStat * GetGameSpecificStat();
	void SetActivePlayerID(int _activePlayerID) { activePlayerID = _activePlayerID; }
	int GetPlayerX() const { return playerX; }
	int GetPlayerY() const { return playerY; }
	int GetActivePlayerID() const {
		return activePlayerID;
	}
	float Freedom();
	char ** GetMaze() const { return maze; }
	bool VisibleGoals() { return visibleGoals;}
	int GetDoorKind(int x, int y);
	int GetMazeWidth() const { return mazeWidth; }
	int GetMazeHeight() const { return mazeHeight; }
	int GetStepsToGameOver() const { return stepsToGameOver; }
	int GetPlayerChoises(int whoAskID);
	char GetTile(int x, int y);
	bool IsGameOver();
	int GetPlayerRank(int playerID, int whoAskID);
	int GetPlayerStatus(int playerID);
	IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	IGameState * GetRandomNextState(int whoAskID, int * outStateID);
	virtual float GetNextStateProb(int whoAskID, int actionID) { return 1.0f; };
	float GetCredibility();

	const vector<int>* GetTileToExplore() const { return &tileToExplore; }
	int FindTileToExplore(int x, int y);
	bool IsPossibleWayToGoal() { return possibleWayToGoal; };
	bool RealExplore(int tileToExploreID);

	int Pos2Dto1D(int x, int y);
	void Pos1Dto2D(int d1, int * x, int * y);
	void PrintToFile(const char * firstLine);
};

#endif
