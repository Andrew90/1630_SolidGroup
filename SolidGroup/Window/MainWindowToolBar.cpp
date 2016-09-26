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
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
	Automat::Start();
	mainWindow.CycleBtn();
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
	AutomaticThresholdsWindow::Instance().Open();	
}
//-------------------------------------------------------------------------------------
void ViewerBtn(HWND)
{
	ZonesWindow::Open();	
}
//-----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	Automat::Stop();
	mainWindow.StopBtn();
}
//-------------------------------------------------------------------------------------------


