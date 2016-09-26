#pragma once
#include <string>
#include <map>

class SolidCounterData
{
public:	
	std::map<std::wstring, int> data;
	static SolidCounterData &Instance();
};