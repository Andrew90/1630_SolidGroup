#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "MainWindow.h"
#include "ZonesWindow.h"
#include "Automat.h"
#include "MiddleWindow.h"
#include "SaveLoadDates.h"
#include "AutomaticThresholdsWindow.h"
#include "Config.h"
#include "Automat.h"
//#include "StoredData.h"
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
#ifndef DEBUG_ITEMS
	automat.Start();
#else
	ComputeStep(0);
	StoreDataFile();
//	StoredData::Instance().Inc();
#endif
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
#pragma message("Автоматическая настройка порогов тулбар кнопка")
	AutomaticThresholdsWindow::Instance().Open();
}
//-------------------------------------------------------------------------------------
/*
void SingleMeashurement(HWND h)
{
#ifndef DEBUG_ITEMS
	automat.Start();
#else
	StoreDataFile();
#endif
}
*/

void ViewerBtn(HWND)
{
	ZonesWindow::Open();
}
//-----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	automat.Stop();
}
//-------------------------------------------------------------------------------------------


