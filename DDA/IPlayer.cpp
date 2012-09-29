#include "IPlayer.h"

bool IPlayer::IsReady()
{
	bool tempReady = isReady;
	isReady = false;
	return tempReady;
}

int IPlayer::MakeTurn()
{
	return myTurn;
}

