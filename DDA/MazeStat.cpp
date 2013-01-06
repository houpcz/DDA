#include "MazeStat.h"


MazeStat::MazeStat(void)
{
}


MazeStat::~MazeStat(void)
{
}

ISpecificStat * MazeStat::Plus(ISpecificStat * other)
{
	MazeStat * otherMaze = dynamic_cast<MazeStat *>(other);
	MazeStat * newGameStat = new MazeStat();
	newGameStat->routeLength = routeLength + otherMaze->routeLength;
	return newGameStat;
}

void MazeStat::Reset()
{
	routeLength = 0;
}

ISpecificStat * MazeStat::CopyYourself()
{
	MazeStat * newStat = new MazeStat(*this);
	return newStat;
}