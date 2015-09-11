// TestDataBase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StoredData.h"
#include "Base.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	Initialize initialize;
	//StoredData storedData;
	StoredData::Instance().Store(L"73", L"K");
	//getchar();
	return 0;
}

