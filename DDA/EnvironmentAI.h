#ifndef _ENVIRONMENTAI_H_
#define _ENVIRONMENTAI_H_

#include "IEnvironmentAI.h"

enum {
	COEF_LEADER_SWITCHES,
	COEF_CREDIBILITY,
	COEF_JUSTICE,
	COEF_LEADER_TIME,
	COEF_STATUS_DIFFERENCE,
	COEF_FREEDOM,
	COEF_RANDOMNESS,
	COEF_COUNT
};

class EnvironmentAI :
	public IEnvironmentAI
{
protected :
	float coefMetric[COEF_COUNT];
public:
	EnvironmentAI(int _myID);
	virtual ~EnvironmentAI(void);
	void SetMetricCoef(int metricID, float value);
	void SetMetricCoefs(float * coefs) 
	{
		for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
		{
			coefMetric[loop1] = coefs[loop1];
		}
	}
	float * CoefMetric() { return coefMetric; };
	virtual bool CoefsHaveMeaning() { return false; };
};

#endif