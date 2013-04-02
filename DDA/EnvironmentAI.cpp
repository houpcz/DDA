#include "EnvironmentAI.h"


EnvironmentAI::EnvironmentAI(int _myID) : IEnvironmentAI(_myID)
{
	for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
		coefMetric[loop1] = 100.0f;

	coefMetric[COEF_RANDOMNESS] = 0.0f;
}


EnvironmentAI::~EnvironmentAI(void)
{
}

void EnvironmentAI::SetMetricCoef(int metricID, float value) 
{ 
	coefMetric[metricID] = value; 
}