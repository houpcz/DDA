#include "EnvironmentAIBasic.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>

EnvironmentAIBasic::EnvironmentAIBasic(void)
{
}


EnvironmentAIBasic::~EnvironmentAIBasic(void)
{
}

bool EnvironmentAIBasic::Think()
{
	int p_myTurn;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(&p_myTurn);

	int tries = 0;
	do {
		myTurn = rand() % p_myTurn;
		tries++;
	} while(nextState[myTurn]->GetPlayerScore(0) == IGameState::ILLEGAL_GAME && tries < 10);
	isReady = true;

	for(int loop1 = 0; loop1 < p_myTurn; loop1++)
		delete nextState[loop1];
	delete [] nextState;

	return true;
}
