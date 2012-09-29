#include "EnvironmentAIBasic.h"
#include <qtime>

EnvironmentAIBasic::EnvironmentAIBasic(void)
{
}


EnvironmentAIBasic::~EnvironmentAIBasic(void)
{
}

void EnvironmentAIBasic::Think()
{
	int * p_myTurn;
	p_myTurn = game->GetEnvironmentChoises();

	myTurn = qrand() % *p_myTurn;
	delete p_myTurn;
	isReady = true;
	emit ImReady();
}
