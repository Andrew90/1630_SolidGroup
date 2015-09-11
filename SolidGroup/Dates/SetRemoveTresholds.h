#pragma once
#include "AutomaticTresholdsCommon.h"
#include <list>
class SetRemoveTresholds
{
	std::list<Data> &tresholds;
	int (&lines)[8];
public:
    SetRemoveTresholds(std::list<Data> &, int (&)[8]);
	void All();
};