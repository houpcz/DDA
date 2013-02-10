#include "EnvironmentAIBasic.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>


EnvironmentAIBasic::~EnvironmentAIBasic(void)
{
}

bool EnvironmentAIBasic::Think()
{
	int p_myTurn;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &p_myTurn);
	if(p_myTurn == 0)
		nextState = game->GetCurrentState()->GetNextStates(myID, &p_myTurn);


	int tries = 0;
	myTurn = rand() % p_myTurn;
	if(nextState[myTurn]->GetPlayerScore(0, myID) == IGameState::ILLEGAL_GAME && game->GetCurrentState()->GetPlayerScore(0, myID) != IGameState::ILLEGAL_GAME)
	{
		game->GetCurrentState()->PrintToFile("Last legal state");
		/*
		for(int loop1 = 0; loop1 < p_myTurn; loop1++)
		{
			nextState[loop1]->PrintToFile("Option");
		}
		*/
	}
	isReady = true;

	for(int loop1 = 0; loop1 < p_myTurn; loop1++)
		delete nextState[loop1];
	delete [] nextState;

	return true;
}
