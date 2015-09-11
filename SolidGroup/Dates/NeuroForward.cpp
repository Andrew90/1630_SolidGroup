#include "stdafx.h"
#include "NeuroForward.h"

bool NeuroForward::Load(wchar_t *path)
{
	FILE *f;
	errno_t err = _wfopen_s(&f, path, L"rb");
	if(0 == err)
	{
		fread(&inputNeurons, sizeof(inputNeurons), 1, f);
		fread(&hiddenNeurons, sizeof(hiddenNeurons), 1, f);
		fread(&outputNeurons, sizeof(outputNeurons), 1, f);
		inputs = new double[inputNeurons];
		hidden = new double[hiddenNeurons];
		actual = new double[outputNeurons];
		wih = new double *[1 + inputNeurons];
		for(int i = 0; i < 1 + inputNeurons; ++i)
		{
		    wih[i] = new double[hiddenNeurons];
			fread(wih[i], sizeof(double) * hiddenNeurons, 1, f);
		}
		who = new double *[1 + hiddenNeurons];
		for(int i = 0; i < 1 + hiddenNeurons; ++i)
		{
			who[i] = new double[outputNeurons];
			fread(who[i], sizeof(double) * outputNeurons, 1, f);
		}
		for(int i = 0; i < outputNeurons; ++i)
		{
			wchar_t buf[128];
			fread(buf, sizeof(buf), 1, f);
			typeSizeStr[i] = buf;
		}
		fclose(f);
	}
	return 0 == err;
}

void NeuroForward::Compute(double *data, double *reference, int count, wchar_t (&type)[128], double &res)
{
	int start = int(0.1 * count);
	int stop = count - start;
	int countFrames = 0;
	memset(actual, 0, sizeof(double) * outputNeurons);
	for(int ii = start; ii < stop; ++ii)
	{
		if(data[ii] < 0 && data[ii + 1] >= 0)
		{
			++countFrames;
			double *data_ = &data[ii];
			double *reference_ = &reference[ii];
			for(int j = 0; j < 60; ++j)
			{
				inputs[j] = data_[j];
				inputs[60 + j] = reference_[j];
			}

			for(int i = 0; i < hiddenNeurons; ++i)
			{
				double sum = 0;
				for(int j = 0; j < inputNeurons; ++j)
				{
					sum += inputs[j] * wih[j][i];
				}
				sum += wih[inputNeurons][i];

				hidden[i] = 1.0 / (1.0 + exp(-sum));
			}

			for(int i = 0; i < outputNeurons; ++i)
			{
				double sum = 0;
				for(int j = 0; j < hiddenNeurons; ++j)
				{
					sum += hidden[j] * who[j][i];
				}
				sum += who[hiddenNeurons][i];

				actual[i] += 1.0 / (1.0 + exp(-sum));
			}
		}
	}
	
	double max = actual[0];
	int k = 0;
	for(int i = 1; i < outputNeurons; ++i)
	{
		if(max < actual[i])
		{
			max = actual[i];
			k = i;
		}
	}
	res = max / countFrames;
	wcscpy_s(type, typeSizeStr[k].c_str());
}

void NeuroForward::Destroy()
{
	for(int i = 0; i < 1 + inputNeurons; ++i) delete[] wih[i];
	delete[] wih;
	
	for(int i = 0; i < 1 + hiddenNeurons; ++i) delete[] who[i];
	delete [] who;

	delete [] inputs;
	delete [] hidden;
	delete [] actual;
}