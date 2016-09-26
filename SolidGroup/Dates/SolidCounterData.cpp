#include "stdafx.h"
#include "SolidCounterData.h"
#include <string.h>
#include "typelist.hpp"
/*
void SolidCounterData::SolidGroup::Set(int c, wchar_t *sg)
{
	count = c;
	wcsncpy_s(solidGroup, sg, dimention_of(solidGroup));
}

SolidCounterData::SolidCounterData()
	: count(0)
{
}

void SolidCounterData::Input(int value, wchar_t *sg)
{
	for(int i = 0; i < count && i < dimention_of(buffer); ++i)
	{
		if(0 == wcscmp(buffer[i].solidGroup, sg))
		{
			buffer[i].count = value;
			return;
		}
	}
	if(count < dimention_of(buffer))
	{

	}
}
*/
SolidCounterData &SolidCounterData::Instance()
{
	static SolidCounterData x; return x;
}
