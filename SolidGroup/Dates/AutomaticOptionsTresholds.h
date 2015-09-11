#pragma once
#include <windows.h>
#include <list>
#include "AutomaticTresholdsCommon.h"
class AutomaticOptionsTresholds
{
	wchar_t path[1024];
	wchar_t *subPath;
public:	
	std::list<Data> thresholdsData;
public:
	int tresholds[8];
	void Init();
	void Update();
private:
	void SubUpdate(wchar_t *, unsigned);
};
