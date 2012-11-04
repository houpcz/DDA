#include "PlayerRandomAI.h"
#include "IGame.h"

PlayerRandomAI::PlayerRandomAI(void)
{
}


PlayerRandomAI::~PlayerRandomAI(void)
{
}

bool PlayerRandomAI::Think()
{
	int p_myTurn;
	p_myTurn = game->GetCurrentState()->GetPlayerChoises();

	myTurn = qrand() % p_myTurn;
	/*
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(&p_myTurn);
	int maxID = 0;
	int myID = game->GetCurrentState()->GetActivePlayerID() - 1;
	int maxValue = nextState[0]->GetPlayerScore(myID);
	for(int loop1 = 1; loop1 < p_myTurn; loop1++)
	{
		if(nextState[loop1]->GetPlayerScore(myID) > maxValue)
		{
			maxValue = nextState[loop1]->GetPlayerScore(myID);
			maxID = loop1;
		}
		delete nextState[loop1];
	}
	delete [] nextState;
	myTurn = maxID;
	*/
	isReady = true;

	return true;
}