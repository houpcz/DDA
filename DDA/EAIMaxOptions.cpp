#include "EAIMaxOptions.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>


EAIMaxOptions::~EAIMaxOptions(void)
{
}

bool EAIMaxOptions::Think()
{
	int turnNumber;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &turnNumber);

	int bestTurn = 0;
	int bestValue = 0;

	for(int loop1 = 0; loop1 < turnNumber; loop1++)
	{
		int temp = nextState[loop1]->GetPlayerChoises(nextState[loop1]->GetActivePlayerID());
		if(temp > bestValue)
		{
			bestValue = temp;
			bestTurn = loop1;
		} else if(temp == bestValue) {
			if(rand() % (loop1 + 1) == 0)
			{
				bestTurn = loop1;
			}
		}
	}

	for(int loop1 = 0; loop1 < turnNumber; loop1++)
		delete nextState[loop1];
	delete [] nextState;

	myTurn = bestTurn;
	isReady = true;
	return true;
}

