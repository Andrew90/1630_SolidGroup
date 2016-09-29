#include "stdafx.h"
#include "SolidData.h"
#include <math.h>
#include "ChartData.h"

#include "DebugMess.h"


SolidData::SolidData()
{
	Clear();
}
void SolidData::Clear()
{
	currentOffset = 0;
}
void SolidData::SetData(double *data, int count, int start)
{
	if(count <= 0) return;
	if(currentOffset + count / 2 > MAX_ZONES_COUNT - 10) return;
	double *a = &referenceBuffer[currentOffset];
	double *b = &dataBuffer[currentOffset];
	if(start)
	{
		b = &referenceBuffer[currentOffset];
	    a = &dataBuffer[currentOffset];
	}

	count &= ~1;

	for(double *i = data, *stop = &data[count]; i < stop;)
	{
		*a = *i;
		++a;
		++i;
		*b = *i;
		++b;
		++i;
		++currentOffset;
	}
}
