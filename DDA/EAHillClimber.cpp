#include "EAHillClimber.h"
#include "IGame.h"

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

	if(0.1f < rand() / (float)RAND_MAX)
	{
		IGameState ** nextState = currentState->GetNextStates(myID, &choises);
		vector<int> bestID;
		float bestValue = -FLT_MAX;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			if(nextState[loop1] == NULL)
				continue;

			GameStat stat = nextState[loop1]->GetGameStat();
			float tempValue =  koefMetric[KOEF_LEADER_SWITCHES] * stat.LeaderSwitches() - 
							   koefMetric[KOEF_CONTROL] * stat.Control() - 
							   koefMetric[KOEF_JUSTICE] * stat.Justice() - 
							   koefMetric[KOEF_LEADER_TIME] * stat.LeaderTime() - 
							   koefMetric[KOEF_STATUS_DIFFERENCE] * stat.StatusDifference();
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

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;

		myTurn = bestID[rand() % bestID.size()];
	} else {
		IGameState * state = currentState->GetRandomNextState(myID, &myTurn);
		delete state;
	}

	isReady = true;

	return true;
}