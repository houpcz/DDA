#pragma once
class MatrixFactory
{
private:
	static const int BUFFER_SIZE = 4000;
	static MatrixFactory inst;
	int currentWidth;
	int currentHeight;
	char ** buffer[BUFFER_SIZE];
	int bufferSize;
public:
	~MatrixFactory(void);
	static MatrixFactory * Inst() { return &inst; }
	char ** GetMatrix(int width, int height);
	void ReturnMatrix(char ** matrix, int width, int height);
private:
	MatrixFactory(void);
	void ClearBuffer();
};

