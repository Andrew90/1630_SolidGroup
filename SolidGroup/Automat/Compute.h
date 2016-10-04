#pragma once

class Compute
{
public:
	int &frequency502;
	int &frequenctGenerator;
public:
	struct Data
	{
		virtual bool dataBuffer(int, double &) = 0;
		virtual bool referenceBuffer(int, double &) = 0;
	};
	Compute();
	void Do();
	void Recalculation();
	bool SubCompute(int(&)[8], int start, int stop, Data &);
};

struct DataBufferXX: Compute::Data
{
	bool(DataBufferXX::*ptr)(int , double &);
	DataBufferXX();
	bool dataBuffer(int i, double &d);
	bool ref(int i, double &d);
	bool referenceBuffer(int i, double &d);
};

extern Compute compute;