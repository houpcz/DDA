#include "EMonteCarlo.h"
#include "IGame.h"
#include "Utility.h"
#include "MonteCarloTreeSearch.h"

using namespace std;

EMonteCarlo::EMonteCarlo(int _myID) : EnvironmentAI(_myID)
{
}

EMonteCarlo::~EMonteCarlo(void)
{
}


bool EMonteCarlo::Think()
{
	int iteration;
	switch(game->GetGameID())
	{
		case GAME_MAZE_ID : 
			iteration = 3000;
			break;
		default : 
			iteration = 10000;
			break;
	}

	if(0.15f < rand() / (float)RAND_MAX)
	{
		MonteCarloTreeSearch mcts(game->GetCurrentState(), myID, iteration, coefMetric, false);
		myTurn = mcts.BestTurn();
	} else {
		IGameState * state = game->GetCurrentState()->GetRandomNextState(myID, &myTurn);
		delete state;
	}
	isReady = true;

	return true;
}
