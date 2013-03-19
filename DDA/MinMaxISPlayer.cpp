#include "MinMaxISPlayer.h"
#include "IGame.h"

MinMaxISPlayer::MinMaxISPlayer(int _myID) : IPlayer(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

MinMaxISPlayer::~MinMaxISPlayer(void)
{
}

float MinMaxISPlayer::MiniMax(IGameState * state, int depth)
{
	int activePlayerID = state->GetActivePlayerID();
	if(depth == 0 || state->IsGameOver())
	{
		leafNumber++;
		return state->GetPlayerRank(myID, 0);
	}

	int choises;
	IGameState ** nextState = state->GetNextStates(myID, &choises);
	float result;
	
	if(activePlayerID == 0)
	{
		result = 0;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float temp = MiniMax(nextState[loop1], depth - 1);
			result += temp;
		}
		result /= (float) choises;
	} else
	{
		bool maximaze = activePlayerID == myID;
		result = MiniMax(nextState[0], depth - 1);
		for(int loop1 = 1; loop1 < choises; loop1++)
		{
			float temp = MiniMax(nextState[loop1], depth - 1);
			 
			if((maximaze && temp > result) || (!maximaze && temp < result))
			{
				result = temp;
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


bool MinMaxISPlayer::Think()
{
	IGameState *currState = game->GetCurrentState();
	int choises = currState->GetPlayerChoises(myID);
	float * rank = new float[choises];
	for(int loop1 = 0; loop1 < choises; loop1++)
		rank[loop1] = 0.0f;

	for(int loop2 = 0; loop2 < 100; loop2++)
	{
		IGameState *tempState = currState->GetStateFromSameInformSet(myID);
		IGameState ** nextState = tempState->GetNextStates(myID, &choises);
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float value = MiniMax(nextState[loop1], 1);
			rank[loop1] += value;
			delete nextState[loop1];
		}
		delete tempState;
		delete [] nextState;
	}

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex((int) rank[loop1], loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	/*
	double mean = (level / 100.0) * choises;
	double deviation = 0.4;
	normal_distribution<> normalDistribution(mean, deviation);
	int choise = (int) (normalDistribution(*generator) + 0.5);
	if(choise < 0)
		choise = 0;
	else if(choise >= choises)
		choise = choises - 1;

	myTurn = scores[choise].second;
	*/
	delete [] rank;
	myTurn = scores[scores.size() - 1].second;
	isReady = true;

	return true;
}