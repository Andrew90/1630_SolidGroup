#pragma once

class SolidData
{
public:
	static const int MAX_ZONES_COUNT = 4000 * 60; 
	int currentOffset;
	int start;
	int stop; 
	double referenceBuffer[MAX_ZONES_COUNT];
	double dataBuffer[MAX_ZONES_COUNT];
public:
	SolidData();
	void Clear();
	void SetData(double *data, int count, int start);
};

extern SolidData solidData;