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
	IGameState ** nextState = state->GetNextStates(myID, &choises);
	float result;
	
	if(activePlayerID == 0)
	{
		result = 0;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float temp = MiniMax(nextState[loop1], alfa, beta, depth - 1);
			result += temp;
		}
		result /= (float) choises;
	} else
	{
		bool maximaze = activePlayerID == myID;
		result = MiniMax(nextState[0], alfa, beta, depth - 1);
		int loop1;
		for(loop1 = 1; loop1 < choises; loop1++)
		{
			float temp = MiniMax(nextState[loop1], alfa, beta, depth - 1);
			 
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

		if(loop1 == choises)
		{
			if(maximaze)
				result = alfa;
			else
				result = beta;
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
	IGameState *state = game->GetCurrentState();
	LostCitiesState *currState = dynamic_cast<LostCitiesState*>(state);

	int cardCount = currState->GetInDeckCount();
	int repeats = cardCount + 1;
	int treeBreadth = 3;

	int choises = currState->GetPlayerChoises(myID);
	int * rank = new int[choises];
	for(int loop1 = 0; loop1 < choises; loop1++)
		rank[loop1] = 0;

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
		for(int loop1 = 0; loop1 < treeBreadth; loop1++)
		{
			int turn = scores[scores.size() - 1 - loop1].second;
			float value = MiniMax(nextState[turn], -FLT_MAX, FLT_MAX, 2);
			if(bestVal < value)
			{
				bestVal = value;
				bestTurn = turn;
			}
		}

		rank[bestTurn]++;

		delete tempState;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;

		if(rank[bestTurn] > repeats / 2 + 1)
			break;
	}

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex(rank[loop1], loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	delete [] rank;
	myTurn = scores[scores.size() - 1].second;
	isReady = true;

	return true;
}