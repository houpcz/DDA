#include "PlayerRandomAI.h"
#include "IGame.h"

PlayerRandomAI::PlayerRandomAI(int _myID) : IPlayer(_myID)
{
}


PlayerRandomAI::~PlayerRandomAI(void)
{
}

bool PlayerRandomAI::Think()
{
	int p_myTurn;
	p_myTurn = game->GetCurrentState()->GetPlayerChoises(myID);

	myTurn = qrand() % p_myTurn;
	
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