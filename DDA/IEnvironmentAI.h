#ifndef _IENVIRONMENTAI_H_
#define _IENVIRONMENTAI_H_

#include "IPlayer.h"

class IEnvironmentAI : public IPlayer
{
Q_OBJECT

public :
	IEnvironmentAI(int _myID) : IPlayer(_myID) {};
	virtual ~IEnvironmentAI() {};
	virtual bool Think() = 0;
	virtual bool IsScalable() { return false; };
	virtual IPlayer * Factory(int playerID) = 0;
	virtual void SetMetricCoef(int metricID, float value) = 0;
};

#endif