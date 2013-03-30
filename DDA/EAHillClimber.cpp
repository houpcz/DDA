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
	float tempVals[KOEF_COUNT];
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
			tempVals[KOEF_LEADER_SWITCHES] = koefMetric[KOEF_LEADER_SWITCHES] * stat.LeaderSwitches();
			tempVals[KOEF_CREDIBILITY] = koefMetric[KOEF_CREDIBILITY] * stat.Credibility();
			tempVals[KOEF_RANDOMNESS] = koefMetric[KOEF_RANDOMNESS] * stat.Randomness();
			tempVals[KOEF_JUSTICE] = koefMetric[KOEF_JUSTICE] * stat.Justice();
			tempVals[KOEF_LEADER_TIME] = koefMetric[KOEF_LEADER_TIME] * stat.LeaderTime();
			tempVals[KOEF_STATUS_DIFFERENCE] = koefMetric[KOEF_STATUS_DIFFERENCE] * stat.StatusDifference();
			float tempValue = tempVals[KOEF_LEADER_SWITCHES] -
				              tempVals[KOEF_CREDIBILITY] - 
							  tempVals[KOEF_RANDOMNESS] -
							  tempVals[KOEF_JUSTICE] -
							  tempVals[KOEF_LEADER_TIME] -
							  tempVals[KOEF_STATUS_DIFFERENCE];
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