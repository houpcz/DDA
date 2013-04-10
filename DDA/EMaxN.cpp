#include "EMaxN.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

EMaxN::EMaxN(int _myID) : EnvironmentAI(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

EMaxN::~EMaxN(void)
{
	delete generator;
}


float * EMaxN::MaxN(IGameState * state, int depth)
{
	int activePlayerID = state->GetActivePlayerID();
	int playerCount = game->GetPlayerCount() - 1;

	if(depth == 0 || state->IsGameOver())
	{
		float * score = new float[playerCount + 1];
		for(int loop1 = 0; loop1 < playerCount; loop1++)
			score[loop1] = state->GetPlayerRank(loop1 + 1, 0);
		GameStat gs = state->GetGameStat();
		score[playerCount] = Utility::Inst()->WeightedMetrics(&gs, coefMetric);
		return score;
	}

	int choises;
	IGameState ** nextState = state->GetNextStates(myID, &choises);
	float * result;
	
	if(activePlayerID == 0)
	{
		result = new float[playerCount + 1];
		result[playerCount] = -FLT_MAX;
		for(int loop1 = 0; loop1 < playerCount; loop1++)
			result[loop1] = 0;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float * temp = MaxN(nextState[loop1], depth - 1);
			
			for(int loop2 = 0; loop2 < playerCount; loop2++)
			{
				if(result[playerCount] < temp[playerCount])
					result[playerCount] = temp[playerCount];

				result[loop2] += temp[loop2];
			}

			delete [] temp;
		}
		for(int loop2 = 0; loop2 < playerCount; loop2++)
			result[loop2] /= (float) choises;
	} else
	{
		result = MaxN(nextState[0], depth - 1);
		for(int loop1 = 1; loop1 < choises; loop1++)
		{
			float * temp = MaxN(nextState[loop1], depth - 1);
			 
			if(temp[activePlayerID - 1] > result[activePlayerID - 1])
			{
				delete [] result;
				result = temp;
			} else {
				delete [] temp;
			}	
		}
	}

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		delete nextState[loop1];
	}
	delete [] nextState;

	return result;
}


bool EMaxN::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	
	int notNull = 0;
	int playerCount = game->GetPlayerCount() - 1;

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
			Utility::WeightedMetrics(&stat, coefMetric);
			float * values = MaxN(nextState[loop1], 6);
			float tempValue = values[playerCount];
			delete [] values;
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
			sprintf(message, "E Max N, no good option. Possible option %d.", notNull);
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
