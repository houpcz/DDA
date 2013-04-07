#include "IPlayer.h"

bool comparator ( const valueIndex& l, const valueIndex& r)
{ 
	return l.first < r.first || (l.first == r.first && l.second > r.second); 
}

bool comparatorD ( const valueDIndex& l, const valueDIndex& r)
{ 
	return l.first < r.first || (l.first == r.first && l.second > r.second); 
}


IPlayer::IPlayer(int _myID) 
{ 
	level = 100;
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

