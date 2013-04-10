#include "PlayerHillClimber.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

PlayerHillClimber::PlayerHillClimber(int _myID) : IPlayer(_myID)
{
}

PlayerHillClimber::~PlayerHillClimber(void)
{
}


bool PlayerHillClimber::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	IGameState ** nextState = currentState->GetNextStates(myID, &choises);

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
	}
	sort(scores.begin(), scores.end(), comparator);

	myTurn = scores[Utility::Inst()->GetTurnIDByLevel(scores.size(), level)].second;

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		delete nextState[loop1];
	}
	delete [] nextState;

	isReady = true;

	return true;
}