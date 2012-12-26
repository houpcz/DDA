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
	choises = game->GetCurrentState()->GetPlayerChoises(myID);
	myTurn = qrand() % choises;
	isReady = true;

	return true;
}