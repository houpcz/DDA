#ifndef _MAZESTAT_H_
#define _MAZESTAT_H_

#include "ispecificstat.h"

class MazeStat :
	public ISpecificStat
{
private :
	int routeLength;

public:
	MazeStat(void);
	virtual ~MazeStat(void);
	ISpecificStat * Plus(ISpecificStat * other);
	ISpecificStat * CopyYourself();
	void Reset();
	void SetRouteLength(int _routeLength) { routeLength = _routeLength; };
	QString Header() { return QString(";Route");};
	QString Record() { return QString::number(routeLength); };
};

#endif
