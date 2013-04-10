#include "EMaxMax.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

EMaxMax::EMaxMax(int _myID) : EnvironmentAI(_myID)
{

}

EMaxMax::~EMaxMax(void)
{
}


void EMaxMax::MaxMax(IGameState * state, int depth)
{
	int choises;
	GameStat stat = state->GetGameStat();
	float tempVal = Utility::WeightedMetrics(&stat, coefMetric);

	if(depth == 0 || state->IsGameOver())
	{
		leaves++;
		
		if(tempVal > bestTurnValue)
		{
			bestTurnValue = tempVal;
		}
		return;
	}

	IGameState ** nextState = state->GetNextStates(myID, &choises);

	int playerID = state->GetActivePlayerID();
	if(playerID == 0)
	{
		if(choises > maxChoises)
		{
			vector<valueDIndex> scores;
			for(int loop1 = 0; loop1 < choises; loop1++)
			{
				if(nextState[loop1] != NULL) {
					GameStat st = nextState[loop1]->GetGameStat();
					scores.push_back(valueDIndex(Utility::WeightedMetrics(&st, coefMetric), loop1));
				}
			}
			sort(scores.begin(), scores.end(), comparatorD);

			for(int loop1 = 0; loop1 < maxChoises && loop1 < scores.size(); loop1++)
			{
				MaxMax(nextState[scores[scores.size() - 1 - loop1].second], depth - 1);
			}
		} else {
			for(int loop1 = 0; loop1 < choises; loop1++)
			{
				if(nextState[loop1] != NULL)
					MaxMax(nextState[loop1], depth - 1);
			}
		}
	} else {
		float bestStatus = nextState[0]->GetPlayerStatus(playerID);
		int bestID = 0;
		for(int loop1 = 1; loop1 < choises; loop1++)
		{
			float tempStatus = nextState[1]->GetPlayerStatus(playerID);
			if(tempStatus > bestStatus)
			{
				bestStatus = tempStatus;
				bestID = loop1;
			}
		}
		MaxMax(nextState[bestID], depth - 1);
	}

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] != NULL)
			delete nextState[loop1];
	}
	delete [] nextState;
}

bool EMaxMax::Think()
{
	switch(game->GetGameID())
	{
		case GAME_MAZE_ID : 
			treeDepth = 2; 
			maxChoises = 4;
			break;
		default : treeDepth = 5; 
			maxChoises = 6;
			break;
	}

	int choises;
	IGameState * currentState = game->GetCurrentState();
	
	int notNull = 0;
	leaves = 0;
	if(0.15f < rand() / (float)RAND_MAX)
	{
		IGameState ** nextState = currentState->GetNextStates(myID, &choises);
		vector<int> bestID;
		float bestValue = -FLT_MAX;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			if(nextState[loop1] == NULL)
				continue;

			bestTurnValue = -FLT_MAX;
			notNull++;

			MaxMax(nextState[loop1], treeDepth);
			float tempValue = bestTurnValue;

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
			sprintf(message, "E Max Max, no good option. Possible option %d.", notNull);
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