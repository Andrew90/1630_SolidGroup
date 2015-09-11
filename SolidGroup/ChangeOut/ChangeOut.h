#pragma once
#include <map>

class ChangeOut
{
public:
	std::map<int, int> toOut;
	bool operator()();
};
extern ChangeOut changeOut;