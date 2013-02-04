#include "EAIMinOptions.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>


EAIMinOptions::~EAIMinOptions(void)
{
}

bool EAIMinOptions::Think()
{
	vector<int> bestTurns;
	int turnNumber;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &turnNumber);

	int bestValue = nextState[0]->GetPlayerChoises(nextState[0]->GetActivePlayerID());
	bestTurns.push_back(0);

	for(int loop1 = 1; loop1 < turnNumber; loop1++)
	{
		int temp = nextState[loop1]->GetPlayerChoises(nextState[loop1]->GetActivePlayerID());
		if(temp < bestValue)
		{
			bestTurns.clear();
			bestTurns.push_back(loop1);
			bestValue = temp;
		} else if(temp == bestValue) {
			bestTurns.push_back(loop1);
		}
	}

	for(int loop1 = 0; loop1 < turnNumber; loop1++)
		delete nextState[loop1];
	delete [] nextState;

	myTurn = bestTurns[rand() % bestTurns.size()];
	isReady = true;
	return true;
}

