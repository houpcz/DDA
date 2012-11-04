#include "MatrixFactory.h"
#include <stdlib.h>

MatrixFactory MatrixFactory::inst=MatrixFactory();

MatrixFactory::MatrixFactory(void)
{
	currentWidth = 0;
	currentHeight = 0;
	bufferSize = 0;
}


MatrixFactory::~MatrixFactory(void)
{
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
	if(width != currentWidth || height != currentHeight)
	{
		ClearBuffer();
		currentWidth = width;
		currentHeight = height;
	}

	if(bufferSize > 0)
	{
		return buffer[--bufferSize];
	} else {
		char ** matrix;
		matrix = new char*[currentHeight];
		for(int loop1 = 0; loop1 < currentHeight; loop1++)
		{
			matrix[loop1] = new char[currentWidth];
		}
		return matrix;
	}
}

void MatrixFactory::ReturnMatrix(char ** matrix, int width, int height)
{
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
}