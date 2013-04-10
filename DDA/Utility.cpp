#include "Utility.h"
#include <stdlib.h>
#include "EnvironmentAI.h"

Utility Utility::inst=Utility();

Utility::Utility(void)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}


Utility::~Utility(void)
{
	delete generator;
}

float Utility::WeightedMetrics(GameStat * stat, float * coefMetric)
{
	float tempVals[COEF_COUNT];
	tempVals[COEF_LEADER_SWITCHES] = coefMetric[COEF_LEADER_SWITCHES] * stat->LeaderSwitches();
	tempVals[COEF_CREDIBILITY] = coefMetric[COEF_CREDIBILITY] * stat->Credibility();
	tempVals[COEF_RANDOMNESS] = coefMetric[COEF_RANDOMNESS] * stat->Randomness();
	tempVals[COEF_JUSTICE] = coefMetric[COEF_JUSTICE] * stat->Justice();
	tempVals[COEF_LEADER_TIME] = coefMetric[COEF_LEADER_TIME] * stat->LeaderTime();
	tempVals[COEF_AVG_STATUS_DIFFERENCE] = coefMetric[COEF_AVG_STATUS_DIFFERENCE] * stat->StatusDifference();
	tempVals[COEF_END_STATUS_DIFFERENCE] = coefMetric[COEF_END_STATUS_DIFFERENCE] * stat->EndRankDifference();
	tempVals[COEF_FREEDOM] = coefMetric[COEF_FREEDOM] * stat->Freedom();
	float tempValue = tempVals[COEF_LEADER_SWITCHES] +
						tempVals[COEF_FREEDOM] -
				        tempVals[COEF_CREDIBILITY] - 
						tempVals[COEF_RANDOMNESS] -
						tempVals[COEF_JUSTICE] -
						tempVals[COEF_LEADER_TIME] -
						tempVals[COEF_AVG_STATUS_DIFFERENCE] -
						tempVals[COEF_END_STATUS_DIFFERENCE];

	return tempValue;
}

int Utility::GetTurnIDByLevel(int maxTurn, int level)
{
	if(level == 100)
		return maxTurn - 1;

	double mean = (level / 100.0) * maxTurn;
	double deviation = 0.4; 
	normal_distribution<> normalDistribution(mean, deviation);
	int choise = (int) (normalDistribution(*generator) + 0.5);
	if(choise < 0)
		choise = 0;
	else if(choise >= maxTurn)
		choise = maxTurn - 1;

	return choise;
}

float Utility::Credibility(int * arr, int arrLength)
{
	int sumAll = 0;
	int * tempArr = new int[arrLength];
	for(int loop1 = 0; loop1 < arrLength; loop1++)
	{
		sumAll += arr[loop1];
	}

	int ones = sumAll % arrLength;
	for(int loop1 = 0; loop1 < arrLength; loop1++)
	{
		if(loop1 < ones)
			tempArr[loop1] = 1;
		else
			tempArr[loop1] = 0;
	}
	float minVar = Variance(tempArr, arrLength);
	delete [] tempArr;

	float var = Variance(arr, arrLength) - minVar;
	//if(var > 0.5)
	//{
	//	Variance(arr, arrLength);
	//}
	if(var < 0.5f)
		return 0.0f;

	return var - 0.5f;
}

float Utility::Variance(int * arr, int arrLength)
{
	float mu = 0.0;
	for(int loop1 = 0; loop1 < arrLength; loop1++)
	{
		mu += arr[loop1];
	}
	mu /= arrLength;

	float var = 0.0f;
	for(int loop1 = 0; loop1 < arrLength; loop1++)
	{
		float delta = mu - arr[loop1];
		var += delta * delta;
	}
	var /= arrLength;

	return var;
}