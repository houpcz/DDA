#include "MaxNPlayer.h"
#include "IGame.h"
#include "MatrixFactory.h"

MaxNPlayer::MaxNPlayer(int _myID) : IPlayer(_myID)
{
}

MaxNPlayer::~MaxNPlayer(void)
{
}

float * MaxNPlayer::MaxN(IGameState * state, int depth)
{
	int activePlayerID = state->GetActivePlayerID();
	int playerCount = game->GetPlayerCount() - 1;

	if(depth == 0 || state->IsGameOver())
	{
		leafNumber++;
		float * score = new float[playerCount];
		for(int loop1 = 0; loop1 < playerCount; loop1++)
			score[loop1] = state->GetPlayerRank(loop1 + 1, 0);
		return score;
	}

	int choises;
	IGameState ** nextState = state->GetNextStates(myID, &choises);
	float * result;
	
	if(activePlayerID == 0)
	{
		result = new float[playerCount];
		for(int loop1 = 0; loop1 < playerCount; loop1++)
			result[loop1] = 0;
		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			float * temp = MaxN(nextState[loop1], depth - 1);
			for(int loop2 = 0; loop2 < playerCount; loop2++)
				result[loop2] += temp[loop2];
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


bool MaxNPlayer::Think()
{
	int choises;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &choises);

	leafNumber =  0;
	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		float * values = MaxN(nextState[loop1], 6);
		scores.push_back(valueIndex((int) values[myID - 1], loop1));
		delete [] values;
		delete nextState[loop1];
	}
	delete [] nextState;
	sort(scores.begin(), scores.end(), comparator);
	myTurn = scores[MatrixFactory::Inst()->GetTurnIDByLevel(scores.size(), level)].second;
	
	isReady = true;

	return true;
}