#pragma once

#include <qmutex.h>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

class MatrixFactory
{
private:
	static const int BUFFER_SIZE = 4000;
	static MatrixFactory inst;
	int currentWidth;
	int currentHeight;
	char ** buffer[BUFFER_SIZE];
	int bufferSize;
	QMutex mutex;

	mt19937 * generator;
public:
	~MatrixFactory(void);
	static MatrixFactory * Inst() { return &inst; }
	char ** GetMatrix(int width, int height);
	void ReturnMatrix(char ** matrix, int width, int height);

	int GetTurnIDByLevel(int maxTurn, int level);
	float Credibility(int * arr, int arrLength);
	float Variance(int * arr, int arrLength);
private:
	MatrixFactory(void);
	void ClearBuffer();
};

