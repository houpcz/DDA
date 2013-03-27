#ifndef _ENVIRONMENTAI_H_
#define _ENVIRONMENTAI_H_

#include "IEnvironmentAI.h"

enum {
	KOEF_LEADER_SWITCHES,
	KOEF_CONTROL,
	KOEF_JUSTICE,
	KOEF_LEADER_TIME,
	KOEF_STATUS_DIFFERENCE
};

class EnvironmentAI :
	public IEnvironmentAI
{
protected :
	static const int MAX_METRIC = 5;
	float koefMetric[MAX_METRIC];
public:
	EnvironmentAI(int _myID);
	~EnvironmentAI(void);
};

#endif