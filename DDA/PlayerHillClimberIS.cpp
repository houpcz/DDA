#include "PlayerHillClimberIS.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

PlayerHillClimberIS::PlayerHillClimberIS(int _myID) : IPlayer(_myID)
{
}

PlayerHillClimberIS::~PlayerHillClimberIS(void)
{
}


bool PlayerHillClimberIS::Think()
{
	IGameState * currentState = game->GetCurrentState();
	int choises = currentState->GetPlayerChoises(myID);
	float * tempRanks = new float[choises];

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		tempRanks[loop1] = 0.0f;
	}

	for(int loop2 = 0; loop2 < 25; loop2++)
	{
		IGameState * tempState = currentState->GetStateFromSameInformSet(myID);
		IGameState ** nextState = tempState->GetNextStates(myID, &choises);

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			tempRanks[loop1] += nextState[loop1]->GetPlayerRank(myID, myID);
		}

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;
		delete tempState;
	}
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex((int) tempRanks[loop1], loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	myTurn = scores[Utility::Inst()->GetTurnIDByLevel(scores.size(), level)].second;

	delete [] tempRanks;
	isReady = true;

	return true;
}