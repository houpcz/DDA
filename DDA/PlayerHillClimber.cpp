#include <vector>
#include "PlayerHillClimber.h"
#include "IGame.h"

using namespace std;

PlayerHillClimber::PlayerHillClimber(int _myID) : IPlayer(_myID)
{
}

PlayerHillClimber::~PlayerHillClimber(void)
{
}


typedef std::pair<int,int> valueIndex;
bool comparator ( const valueIndex& l, const valueIndex& r)
{ 
	return l.first > r.first; 
}

bool PlayerHillClimber::Think()
{
	int choises;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &choises);

	vector<valueIndex> scores;
	for(int loop1 = 1; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex(nextState[loop1]->GetPlayerScore(myID, myID), loop1));
		delete nextState[loop1];
	}
	delete [] nextState;
	sort(scores.begin(), scores.end(), comparator);

	myTurn = scores[0].second;
	
	isReady = true;

	return true;
}