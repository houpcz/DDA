#include "EnvironmentAIBasic.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>


EnvironmentAIBasic::~EnvironmentAIBasic(void)
{
}

bool EnvironmentAIBasic::Think()
{
	int turnID;
	IGameState * nextState = game->GetCurrentState()->GetRandomNextState(myID, &turnID);

	myTurn = turnID;
	isReady = true;

	delete nextState;

	return true;
}
