#pragma once

class Compute
{
	int &frequency502;
	int &frequenctGenerator;
public:
	struct Data
	{
		virtual double dataBuffer(int ) = 0;
		virtual double referenceBuffer(int ) = 0;
	};
	 Compute();
	 void Do();
	 void Recalculation();
	 bool SubCompute(int(&)[8], int start, int stop, Data &, double(&)[1024], int &length);
};
extern Compute compute;