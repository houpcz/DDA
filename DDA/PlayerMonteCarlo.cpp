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
	
	MonteCarloTreeSearch mcts(game->GetCurrentState(), myID, 100 + level * 5);
	myTurn = mcts.BestTurn();
	isReady = true;

	return true;
}