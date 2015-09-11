#pragma once

class ChartData
{
public:
	int count;
	double dataMin[1024];
	double dataMax[1024];
	ChartData();
	void Clear();
	void Set(double *inputData, int inputCount, int outputCout);
};

extern ChartData l502Signal;
extern ChartData l502Reference;