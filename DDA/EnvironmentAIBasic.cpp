#include "EnvironmentAIBasic.h"
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
	int * p_myTurn;
	p_myTurn = game->GetPlayerChoises();

	myTurn = rand() % *p_myTurn;
	delete p_myTurn;
	isReady = true;

	return true;
}
