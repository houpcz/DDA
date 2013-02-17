#include "EnvironmentAIBasic.h"
#include "IGame.h"
#include <qtime>
#include <stdlib.h>


EnvironmentAIBasic::~EnvironmentAIBasic(void)
{
}

bool EnvironmentAIBasic::Think()
{
	int choises;
	int choisesLegal = 0;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &choises);

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] != NULL)
		{
			choisesLegal++;
		}
	}
	int myTurnLegal = rand() % choisesLegal;

	int tempLegal = 0;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] != NULL)
		{
			if(myTurnLegal == tempLegal)
			{
				myTurn = loop1;
			}
			tempLegal++;
		}
	}
	
	isReady = true;

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] != NULL)
			delete nextState[loop1];
	}
	delete [] nextState;

	return true;
}
