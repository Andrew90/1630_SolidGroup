#pragma once
#include <map>

class NeuroForward
{
	int inputNeurons;
	int hiddenNeurons;
	int outputNeurons;
	double **wih;
	double **who;
	double *inputs;
	double *hidden;
	double *actual;
public:
	std::map<int, std::wstring> typeSizeStr;
public:	
	void Destroy();
	bool Load(wchar_t *);
	void Compute(double *data, double *reference, int count, wchar_t (&type)[128], double &res);
};