#include "PlayerRandomAI.h"


PlayerRandomAI::PlayerRandomAI(void)
{
}


PlayerRandomAI::~PlayerRandomAI(void)
{
}

bool PlayerRandomAI::Think()
{
	int * p_myTurn;
	p_myTurn = game->GetPlayerChoises();

	myTurn = qrand() % *p_myTurn;
	delete p_myTurn;
	isReady = true;

	return true;
}