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
	float tempVals[COEF_COUNT];
	for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
	{
		tempVals[loop1] = 0.0f;
	}
	int notNull = 0;
	if(0.0f < rand() / (float)RAND_MAX)
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
			tempVals[COEF_LEADER_SWITCHES] = coefMetric[COEF_LEADER_SWITCHES] * stat.LeaderSwitches();
			tempVals[COEF_CREDIBILITY] = coefMetric[COEF_CREDIBILITY] * stat.Credibility();
			tempVals[COEF_RANDOMNESS] = coefMetric[COEF_RANDOMNESS] * stat.Randomness();
			tempVals[COEF_JUSTICE] = coefMetric[COEF_JUSTICE] * stat.Justice();
			tempVals[COEF_LEADER_TIME] = coefMetric[COEF_LEADER_TIME] * stat.LeaderTime();
			tempVals[COEF_STATUS_DIFFERENCE] = coefMetric[COEF_STATUS_DIFFERENCE] * stat.StatusDifference();
			tempVals[COEF_FREEDOM] = coefMetric[COEF_FREEDOM] * stat.Freedom();
			float tempValue = tempVals[COEF_LEADER_SWITCHES] +
							  tempVals[COEF_FREEDOM] -
				              tempVals[COEF_CREDIBILITY] - 
							  tempVals[COEF_RANDOMNESS] -
							  tempVals[COEF_JUSTICE] -
							  tempVals[COEF_LEADER_TIME] -
							  tempVals[COEF_STATUS_DIFFERENCE];
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

	isReady = true;

	return true;
}