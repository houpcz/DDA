#pragma once
class MatrixFactory
{
private :
	static const int BUFFER_SIZE = 4000;
	int currentWidth;
	int currentHeight;
	char ** buffer[BUFFER_SIZE];
	int bufferSize;
	void ClearBuffer();
public:
	char ** GetMatrix(int width, int height);
	void ReturnMatrix(char ** matrix, int width, int height);

	MatrixFactory(void);
	~MatrixFactory(void);
};

