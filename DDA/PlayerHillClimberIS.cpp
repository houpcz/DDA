#include "PlayerHillClimberIS.h"
#include "IGame.h"

using namespace std;

PlayerHillClimberIS::PlayerHillClimberIS(int _myID) : IPlayer(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

PlayerHillClimberIS::~PlayerHillClimberIS(void)
{
	delete generator;
}


bool PlayerHillClimberIS::Think()
{
	IGameState * currentState = game->GetCurrentState();
	int choises = currentState->GetPlayerChoises(myID);
	float * tempRanks = new float[choises];

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		tempRanks[loop1] = 0.0f;
	}

	for(int loop2 = 0; loop2 < 10; loop2++)
	{
		IGameState * tempState = currentState->GetStateFromSameInformSet(myID);
		IGameState ** nextState = tempState->GetNextStates(myID, &choises);

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			tempRanks[loop1] += nextState[loop1]->GetPlayerRank(myID, myID);
		}

		for(int loop1 = 0; loop1 < choises; loop1++)
		{
			delete nextState[loop1];
		}
		delete [] nextState;
		delete tempState;
	}
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex((int) tempRanks[loop1], loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	double mean = (level / 100.0) * choises;
	double deviation = 0.4; 
	normal_distribution<> normalDistribution(mean, deviation);
	int choise = (int) (normalDistribution(*generator) + 0.5);
	if(choise < 0)
		choise = 0;
	else if(choise >= choises)
		choise = choises - 1;

	delete [] tempRanks;
	myTurn = scores[choise].second;
	isReady = true;

	return true;
}