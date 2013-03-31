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
	virtual ~EnvironmentAI(void);
	void SetMetricCoef(int metricID, float value);
	void SetMetricCoefs(float * coefs) 
	{
		for(int loop1 = 0; loop1 < KOEF_COUNT; loop1++)
		{
			koefMetric[loop1] = coefs[loop1];
		}
	}
	float * CoefMetric() { return koefMetric; };
	virtual bool CoefsHaveMeaning() { return false; };
};

#endif