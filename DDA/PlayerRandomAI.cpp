#include "PlayerRandomAI.h"
#include "IGame.h"

PlayerRandomAI::PlayerRandomAI(int _myID) : IPlayer(_myID)
{
}


PlayerRandomAI::~PlayerRandomAI(void)
{
}

bool PlayerRandomAI::Think()
{
	int choises;
	IGameState * state = game->GetCurrentState()->GetRandomNextState(myID, &myTurn);
	delete state;
	isReady = true;

	return true;
}