#include "EnvironmentAI.h"


EnvironmentAI::EnvironmentAI(int _myID) : IEnvironmentAI(_myID)
{
	for(int loop1 = 0; loop1 < MAX_METRIC; loop1++)
		koefMetric[loop1] = 1.0f;

	koefMetric[KOEF_CONTROL] = 100.0f;
}


EnvironmentAI::~EnvironmentAI(void)
{
}
