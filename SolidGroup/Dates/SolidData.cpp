#include "stdafx.h"
#include "SolidData.h"
#include <math.h>
#include "ChartData.h"

#include "DebugMess.h"


SolidData::SolidData()
{
	Clear();
//test
#if 0
	currentOffset = 5000;
	double dx = 2 * 3.14 / 300;
	double x = 0;
	for(int i = 0; i < MAX_ZONES_COUNT; ++i)
	{
		x += 0.1;
		if(x > 10) x = 0;
	    referenceBuffer[i] = x;
		dataBuffer[i] = sin(dx * i * 5) * 3 + 4;
	}

	l502Signal.Set(dataBuffer, 300, 300);
	l502Reference.Set(referenceBuffer, 300, 300);
#endif
	//test
}
void SolidData::Clear()
{
	currentOffset = 0;
}
void SolidData::SetData(double *data, int count)
{
	if(count <= 0) return;
	if(currentOffset + count / 2 > MAX_ZONES_COUNT - 10) return;
	for(int i = 0; i < count; i += 2)
	{
		referenceBuffer[currentOffset] = data[i];
		dataBuffer[currentOffset] = data[i + 1];
		++currentOffset;
	}
}
