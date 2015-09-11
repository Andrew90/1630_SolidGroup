#include "stdafx.h"
#include "CounterTubes.h"
#include "AppBase.h"
/*
CounterTubes::CounterTubes()
	: counter(Singleton<DifferentOptionsTable>::Instance().items.get<CounterTubesStored>().value)
{
}

void CounterTubes::Inc()
{
	++counter;
}

void CounterTubes::Clear()
{
	counter = 0;
}

int CounterTubes::operator()()
{
	return counter;
}

void CounterTubes::Destroy()
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		Update<DifferentOptionsTable>(base).set<CounterTubesStored>(counter).Where().ID(1).Execute();
	}
}
*/

