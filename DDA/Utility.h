#pragma once

#include <qmutex.h>
#include <vector>
#include <random>
#include <cmath>
#include "GameStat.h"

using namespace std;

class Utility
{
private:
	static Utility inst;
	mt19937 * generator;
public:
	~Utility(void);
	static Utility * Inst() { return &inst; };

	int GetTurnIDByLevel(int maxTurn, int level);
	float Credibility(int * arr, int arrLength);
	float Variance(int * arr, int arrLength);
	static float WeightedMetrics(GameStat * gameStat, float * coefs);
private:
	Utility(void);
};

