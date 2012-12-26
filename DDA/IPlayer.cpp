#include "IPlayer.h"

IPlayer::IPlayer(int _myID) 
{ 
	level = 50;
	myID = _myID;
}
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

