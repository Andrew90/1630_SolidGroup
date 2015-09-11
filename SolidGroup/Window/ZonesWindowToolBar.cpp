#include "stdafx.h"
#include "ZonesWindowToolBar.h"
#include "ZonesWindow.h"
//#include "DetailedWindow.h"
#include "EmptyWindow.h"
//#include "AutomatNew.h"
#include "DebugMess.h"
//#include "PrimaryData.h"
#if 0
void LeftArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone - 1;
	if(zone < 0) zone = primaryData.countZones;
	ZonesWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	++z.zone
	//if(zone > primaryData.countZones) zone = 0;
	ZonesWindow::Open(sensor, zone);
}
#else
void LeftArrowHandler(HWND h)
{
	int &x = ZonesWindow::Instance().zone;
	if(--x < 0) x = 0;
	ZonesWindow::Open();
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	++ZonesWindow::Instance().zone;
	ZonesWindow::Open();
}
#endif
//----------------------------------------------------------------------------
int count_sensors = 50;
void UpArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int zone = z.zone;
}
//----------------------------------------------------------------------------
void DownArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int zone = z.zone;
}
//----------------------------------------------------------------------------