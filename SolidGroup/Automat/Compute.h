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
	 bool SubCompute(int(&)[8], int start, int stop, Data &, double(&)[1024]);
};
extern Compute compute;