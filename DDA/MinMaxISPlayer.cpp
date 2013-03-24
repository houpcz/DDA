#include "MinMaxISPlayer.h"
#include "LostCitiesState.h"
#include "IGame.h"

MinMaxISPlayer::MinMaxISPlayer(int _myID) : IPlayer(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

MinMaxISPlayer::~MinMaxISPlayer(void)
{
	delete generator;
}

float MinMaxISPlayer::MiniMax(IGameState * state, float alfa, float beta, int depth)
{
	int activePlayerID = state->GetActivePlayerID();
	if(depth == 0 || state->IsGameOver())
	{
		leafNumber++;
		return state->GetPlayerRank(myID, 0);
	}

	int choises;
	float result;
	
	if(activePlayerID == 0)
	{
		int trash;
		IGameState * nextState = state->GetRandomNextState(myID, &trash);
		result = MiniMax(nextState, alfa, beta, depth - 1);
		delete nextState;
		
		/*
		IGameState ** nextState = state->GetNextStates(myID, &choises);
		result = 0;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float temp = MiniMax(nextState[loop1], alfa, beta, depth - 1);
			result += temp;
		}
		result /= (float) choises;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;
		*/
	} else
	{
		IGameState ** nextState = state->GetNextStates(myID, &choises);


		vector<valueIndex> scores;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(activePlayerID, activePlayerID), loop1));
		}
		sort(scores.begin(), scores.end(), comparator);

		bool maximaze = activePlayerID == myID;
		result = -FLT_MAX;
		for(int loop1 = 0; loop1 < 3 && loop1 < scores.size(); loop1++)
		{
			int turn = scores[scores.size() - 1 - loop1].second;
			float temp = MiniMax(nextState[turn], alfa, beta, depth - 1);
			 
			if(maximaze)
			{
				alfa = max(temp, alfa);
				if(beta <= alfa)
				{
					result = alfa;
					break;
				}
			} else {
				beta = min(temp, beta);
				if(beta <= alfa)
				{
					result = beta;
					break;
				}
			}
		}

		if(maximaze)
			result = alfa;
		else
			result = beta;

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;
	}

	return result;
}


bool MinMaxISPlayer::Think()
{ 
	IGameState *state = game->GetCurrentState();
	LostCitiesState *currState = dynamic_cast<LostCitiesState*>(state);
	int choises = currState->GetPlayerChoises(myID);

	int cardCount = currState->GetInDeckCount();
	int repeats = cardCount * 6 + 5;
	int treeBreadth = 10; 
	int treeDepth = 3;
	if(cardCount < 8)
		treeDepth = 5;
	if(cardCount < 6)
		treeDepth = 7;
	/*
	if(cardCount < 10)
		treeDepth = 5;*/
	
	float * rank = new float[choises];
	int * rankCount = new int[choises];
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		rankCount[loop1] = 0;
		rank[loop1] = 0.0f;
	}

	int bestTurn = -1;
	float bestVal = -FLT_MAX;
	for(int loop2 = 0; loop2 < repeats; loop2++)
	{
		IGameState *tempState = currState->GetStateFromSameInformSet(myID);
		IGameState ** nextState = currState->GetNextStates(myID, &choises);

		vector<valueIndex> scores;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
		}
		sort(scores.begin(), scores.end(), comparator);
		for(int loop1 = 0; loop1 < treeBreadth && loop1 < scores.size(); loop1++)
		{
			int turn = scores[scores.size() - 1 - loop1].second;
			float value = MiniMax(nextState[turn], -FLT_MAX, FLT_MAX, treeDepth);
			if(bestVal < value)
			{
				bestVal = value;
				bestTurn = turn;
			}
		}

		rank[bestTurn] += bestVal;
		rankCount[bestTurn]++;

		delete tempState;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;

		if(rankCount[bestTurn] > repeats / 2 + 1)
			break;
	}

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(rankCount[loop1] > 0)
			scores.push_back(valueIndex((int) (rank[loop1] / rankCount[loop1]), loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	delete [] rank;
	delete [] rankCount;
	myTurn = scores[scores.size() - 1].second;
	isReady = true;

	return true;
}