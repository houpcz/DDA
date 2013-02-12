#include "PlayerHillClimber.h"
#include "IGame.h"

using namespace std;

PlayerHillClimber::PlayerHillClimber(int _myID) : IPlayer(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

PlayerHillClimber::~PlayerHillClimber(void)
{
	delete generator;
}


bool PlayerHillClimber::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	IGameState ** nextState = currentState->GetNextStates(myID, &choises);

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
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

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		delete nextState[loop1];
	}
	delete [] nextState;

	myTurn = scores[choise].second;
	isReady = true;

	return true;
}