#include "MiniMaxPlayer.h"
#include "IGame.h"

MiniMaxPlayer::MiniMaxPlayer(int _myID) : IPlayer(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

MiniMaxPlayer::~MiniMaxPlayer(void)
{
}

float * MiniMaxPlayer::MaxN(IGameState * state, int depth)
{
	int activePlayerID = state->GetActivePlayerID();
	int playerCount = game->GetPlayerCount() - 1;

	if(depth == 0 || state->IsGameOver())
	{
		leafNumber++;
		float * score = new float[playerCount];
		for(int loop1 = 0; loop1 < playerCount; loop1++)
			score[loop1] = state->GetPlayerScore(loop1 + 1, myID);
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

			if(temp[activePlayerID] > result[activePlayerID])
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


bool MiniMaxPlayer::Think()
{
	int choises;
	IGameState ** nextState = game->GetCurrentState()->GetNextStates(myID, &choises);

	leafNumber =  0;
	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		float * values = MaxN(nextState[loop1], 2);
		scores.push_back(valueIndex((int) values[myID - 1], loop1));
		delete [] values;
		delete nextState[loop1];
	}
	delete [] nextState;
	sort(scores.begin(), scores.end(), comparator);
	
	double mean = (level / 100.0) * choises;
	double deviation = 0.4;
	normal_distribution<> normalDistribution(mean, deviation);
	int choise = (int) (normalDistribution(*generator) + 0.5);
	if(choise < 0)
		choise = 0;
	else if(choise >= choises)
		choise = choises - 1;

	myTurn = scores[choise].second;
	isReady = true;

	return true;
}