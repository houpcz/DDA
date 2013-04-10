#include "EMonteCarloDelta.h"
#include "IGame.h"
#include "Utility.h"
#include "MonteCarloTreeSearch.h"

using namespace std;

EMonteCarloDelta::EMonteCarloDelta(int _myID) : EnvironmentAI(_myID)
{
}

EMonteCarloDelta::~EMonteCarloDelta(void)
{
}


bool EMonteCarloDelta::Think()
{
	int iteration;
	switch(game->GetGameID())
	{
		case GAME_MAZE_ID : 
			iteration = 5000;
			break;
		default : 
			iteration = 10000;
			break;
	}

	if(0.15f < rand() / (float)RAND_MAX)
	{
		MonteCarloTreeSearch mcts(game->GetCurrentState(), myID, iteration, coefMetric, true);
		myTurn = mcts.BestTurn();
	} else {
		IGameState * state = game->GetCurrentState()->GetRandomNextState(myID, &myTurn);
		delete state;
	}
	isReady = true;

	return true;
}
