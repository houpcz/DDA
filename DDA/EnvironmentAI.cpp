#include "EnvironmentAI.h"


EnvironmentAI::EnvironmentAI(int _myID) : IEnvironmentAI(_myID)
{
	for(int loop1 = 0; loop1 < KOEF_COUNT; loop1++)
		koefMetric[loop1] = 1.0f;

	koefMetric[KOEF_LEADER_SWITCHES] = 10000.0f;
	koefMetric[KOEF_CREDIBILITY] = 0.0f;
	koefMetric[KOEF_JUSTICE] = 0.0f;
	koefMetric[KOEF_STATUS_DIFFERENCE] = 1000.0f;
}


EnvironmentAI::~EnvironmentAI(void)
{
}
