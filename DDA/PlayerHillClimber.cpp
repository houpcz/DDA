#include "PlayerHillClimber.h"
#include "IGame.h"


PlayerHillClimber::PlayerHillClimber(int _myID) : IPlayer(_myID)
{
}

PlayerHillClimber::~PlayerHillClimber(void)
{
}

bool PlayerHillClimber::Think()
{
	int p_myTurn;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &p_myTurn);
	int maxID = 0;
	int maxValue = nextState[0]->GetPlayerScore(myID, myID);
	for(int loop1 = 1; loop1 < p_myTurn; loop1++)
	{
		int tempScore = nextState[loop1]->GetPlayerScore(myID, myID);
		if(tempScore > maxValue)
		{
			maxValue = tempScore;
			maxID = loop1;
		}
		delete nextState[loop1];
	}
	delete [] nextState;
	myTurn = maxID;
	
	isReady = true;

	return true;
}