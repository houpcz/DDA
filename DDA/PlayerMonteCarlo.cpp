#include "PlayerMonteCarlo.h"
#include "IGame.h"
#include "MonteCarloTreeSearch.h"

PlayerMonteCarlo::PlayerMonteCarlo(int _myID) : IPlayer(_myID)
{
}


PlayerMonteCarlo::~PlayerMonteCarlo(void)
{
}

bool PlayerMonteCarlo::Think()
{
	int choises;
	
	MonteCarloTreeSearch mcts(game->GetCurrentState(), myID, 5000);
	myTurn = mcts.BestTurn(level);
	isReady = true;

	return true;
}