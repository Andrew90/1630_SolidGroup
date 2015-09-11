#include "stdafx.h"
#include "GlobalItems.h"
#include "ArchiveEvents.h"
#include "CounterTubes.h"
//#include "MainLoop.h"
#include "AppBase.h"
//#include "Automat.h"
#include "Device1730.h"
#include "Compute.h"
#include "ChangeOut.h"
//#include "Lan.h"
//#include "OutBitDown.h"
//#include "SQEvents.h"
//#include "Controller.h"
//#include "Config.h"
#include "StoredData.h"

#include "ChartData.h"
#include "SolidData.h"
#include "L502SolidGroup.h"
#include "Automat.h"
#include "UpdateMainChart.h"
#include "Corel.h"
#include "ClientTreshold.h"

bool InitGlobalData()
{
	AppBase().Init();	
	
	if(!device1730.Init(L"PCIE-1730,BID#0"))
	{
		MessageBox(0, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		return false;
	}
	device1730.Write(0);
	if(l502SolidGroup.Init() && l502SolidGroup.SetupParams())
	{
		MessageBox(0, L"Не могу инициировать плату L502", L"Ошибка !!!", MB_ICONERROR);
		return false;
	}
	automat.dataChanged = UpdateMainChart::Update;
	corel.Init();
	automat.Init();
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
		Select<CurrentParametersTable>(base).ID(1).Execute(current);
		corel.LoadFromBase(current.items.get<CurrentID>().value);
	}
	return true;

}

void DestroyGlobalData()
{
//	counterTubes.Destroy();
	device1730.Write(0);
	Sleep(100);
	device1730.Destroy();
	l502SolidGroup.Destroy();
	automat.Destroy();
}
ArchiveEvents archiveEvents;

ChartData l502Signal;
ChartData l502Reference;

SolidData solidData;
Compute compute;

L502SolidGroup l502SolidGroup;

Automat automat;

Corel corel;
ClientTreshold clientTreshold;

//CounterTubes counterTubes;
ChangeOut changeOut;
//StoredData storedData;

#ifndef DEBUG_ITEMS
 Device1730 device1730;
#else
 Debug1730 device1730;
#endif
 /*
bool initGlobalUnitOk;
*/