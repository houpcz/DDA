#include "EAHillClimber.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

EAHillClimber::EAHillClimber(int _myID) : EnvironmentAI(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

EAHillClimber::~EAHillClimber(void)
{
	delete generator;
}


bool EAHillClimber::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	
	//GameStat stat2 = currentState->GetGameStat();
	//float t2 = MatrixFactory::WeightedMetrics(&stat2, coefMetric);
	//if(t2 < 0.0)
	//	t2 += 0.0f;

	int notNull = 0;
	if(0.2f < rand() / (float)RAND_MAX)
	{
		IGameState ** nextState = currentState->GetNextStates(myID, &choises);
		vector<int> bestID;
		float bestValue = -FLT_MAX;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			if(nextState[loop1] == NULL)
				continue;

			notNull++;
			GameStat stat = nextState[loop1]->GetGameStat();
			
			float tempValue = Utility::WeightedMetrics(&stat, coefMetric);
			//if(tempValue < 0)
			//{
			//	GameStat sta = currentState->GetGameStat();
			//	tempValue += 5 - 5;
			//}

			if(tempValue >= bestValue)
			{
				if(tempValue != bestValue)
				{
					bestValue = tempValue;
					bestID.clear();
				}	

				bestID.push_back(loop1);
			}
		}

		if(bestID.size() == 0)
		{
			char message[256];
			sprintf(message, "EA Hill Climber, no good option. Possible option %d.", notNull);
			currentState->PrintToFile(message);
			for(int loop1 = 0; loop1 < choises; loop1++)
			{
				if(nextState[loop1] != NULL)
					nextState[loop1]->PrintToFile(message);
			}
			 
			//throw message;
		}

		myTurn = bestID[rand() % bestID.size()];

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			if(nextState[loop1] != NULL)
				delete nextState[loop1];
		}
		delete [] nextState;
	} else {
		IGameState * state = currentState->GetRandomNextState(myID, &myTurn);
		delete state;
	}
	//myTurn = 0;
	isReady = true;

	return true;
}