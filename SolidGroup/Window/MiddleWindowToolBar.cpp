#include "stdafx.h"
#include "MiddleWindowToolBar.h"
#include "MiddleWindow.h"
//#include "DetailedWindow.h"
#include "EmptyWindow.h"
//#include "AutomatNew.h"
#include "DebugMess.h"
//#include "PrimaryData.h"
#if 0
void LeftArrowHandler(HWND h)
{
	MiddleWindow &z = MiddleWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone - 1;
	if(zone < 0) zone = primaryData.countZones;
	MiddleWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	MiddleWindow &z = MiddleWindow::Instance();
	int sensor = z.sensor;
	++z.zone
	//if(zone > primaryData.countZones) zone = 0;
	MiddleWindow::Open(sensor, zone);
}
#else
namespace MiddleWindowToolBar
{
void LeftArrowHandler(HWND h)
{
	int &x = MiddleWindow::Instance().zone;
	if(--x < 0) x = 0;
	MiddleWindow::Open();
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	++MiddleWindow::Instance().zone;
	MiddleWindow::Open();
}
#endif
//----------------------------------------------------------------------------
#pragma message("����� ���� ??????????????????")
int count_sensors = 50;
void UpArrowHandler(HWND h)
{
	MiddleWindow &z = MiddleWindow::Instance();
	int zone = z.zone;
//	ZonesWindow::Open(zone);
}
//----------------------------------------------------------------------------
void DownArrowHandler(HWND h)
{
	MiddleWindow &z = MiddleWindow::Instance();
	int zone = z.zone;
	//ZonesWindow::Open(zone);
}
//----------------------------------------------------------------------------
}