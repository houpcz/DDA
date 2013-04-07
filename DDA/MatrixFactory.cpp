#include "MatrixFactory.h"
#include <stdlib.h>

MatrixFactory MatrixFactory::inst=MatrixFactory();

MatrixFactory::MatrixFactory(void)
{
	currentWidth = 0;
	currentHeight = 0;
	bufferSize = 0;

	random_device randomDevice;
    generator = new mt19937(randomDevice());
}


MatrixFactory::~MatrixFactory(void)
{
	delete generator;
	ClearBuffer();
}

void MatrixFactory::ClearBuffer()
{
	for(int loop1 = 0; loop1 < bufferSize; loop1++)
	{
		for(int loop2 = 0; loop2 < currentHeight; loop2++)
			delete buffer[loop1][loop2];
		delete buffer[loop1];
	}
	bufferSize = 0;
}

char ** MatrixFactory::GetMatrix(int width, int height)
{
	mutex.lock();

	if(width != currentWidth || height != currentHeight)
	{
		ClearBuffer();
		currentWidth = width;
		currentHeight = height;
	}

	if(bufferSize > 0)
	{
		bufferSize--;
		char ** matrix = buffer[bufferSize];
		mutex.unlock();
		return matrix;
	} else {
		char ** matrix;
		matrix = new char*[currentHeight];
		for(int loop1 = 0; loop1 < currentHeight; loop1++)
		{
			matrix[loop1] = new char[currentWidth];
		}
		mutex.unlock();
		return matrix;
	}
}

void MatrixFactory::ReturnMatrix(char ** matrix, int width, int height)
{
	mutex.lock();
	if(width != currentWidth || height != currentHeight || bufferSize == BUFFER_SIZE)
	{
		for(int loop1 = 0; loop1 < height; loop1++)
		{
			delete matrix[loop1];
		}
		delete matrix;
	} else {
		buffer[bufferSize++] = matrix;
	}
	mutex.unlock();
}


int MatrixFactory::GetTurnIDByLevel(int maxTurn, int level)
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

float MatrixFactory::Credibility(int * arr, int arrLength)
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

	if(var < 0.5f)
		var = 0.5f;

	return var - 0.5f;
}

float MatrixFactory::Variance(int * arr, int arrLength)
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