#ifndef _ENVIRONMENTAI_H_
#define _ENVIRONMENTAI_H_

#include "IEnvironmentAI.h"

enum {
	KOEF_LEADER_SWITCHES,
	KOEF_CREDIBILITY,
	KOEF_JUSTICE,
	KOEF_LEADER_TIME,
	KOEF_STATUS_DIFFERENCE,
	KOEF_RANDOMNESS,
	KOEF_COUNT
};

class EnvironmentAI :
	public IEnvironmentAI
{
protected :
	float koefMetric[KOEF_COUNT];
public:
	EnvironmentAI(int _myID);
	~EnvironmentAI(void);
};

#endif