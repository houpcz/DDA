#include "EnvironmentAI.h"


EnvironmentAI::EnvironmentAI(int _myID) : IEnvironmentAI(_myID)
{
	for(int loop1 = 0; loop1 < KOEF_COUNT; loop1++)
		koefMetric[loop1] = 1.0f;

	koefMetric[KOEF_CREDIBILITY] = 100.0f;
	koefMetric[KOEF_JUSTICE] = 100.0f;
}


EnvironmentAI::~EnvironmentAI(void)
{
}
