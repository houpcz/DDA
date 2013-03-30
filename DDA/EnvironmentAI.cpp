#include "EnvironmentAI.h"


EnvironmentAI::EnvironmentAI(int _myID) : IEnvironmentAI(_myID)
{
	for(int loop1 = 0; loop1 < KOEF_COUNT; loop1++)
		koefMetric[loop1] = 100.0f;

	koefMetric[KOEF_RANDOMNESS] = 0.0f;
}


EnvironmentAI::~EnvironmentAI(void)
{
}

void EnvironmentAI::SetMetricCoef(int metricID, float value) 
{ 
	koefMetric[metricID] = value; 
}