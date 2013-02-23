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
	game->GetCurrentState()->GetRandomNextState(myID, &myTurn);
	isReady = true;

	return true;
}