#include "stdafx.h"
#include "TresholdWindow.h"
#include "AppBase.h"
#include <commctrl.h>
#include "MenuApi.h"
#include "Dialogs.h"
#include "MainWindow.h"

#include "TresholdsWindowMenu.hpp"
//#include "MainWindowToolBar.h"
/*
#include "InitToolBar.hpp"
*/
#include "EmptyWindow.h"
/*
#include "TopLabelViewer.h"
#include "ThicknessViewer.h"
#include "ToolBArWidth.hpp"
#include "GlobalItems.h"
*/
#include "UpdateMainChart.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define xprint debug.print(__FUNCTION__);
#define dprint debug.print
#define d_mess(x)
#define x_mess debug.print
#else
#define xprint
#define dprint
#endif
//------------------------------------------------------------------------
void TresholdWindow::operator()(TSize &m)
{
	static const int offs = 500;
	MoveWindow(tresholdPanelViewer.hWnd , 0, 0, offs, m.Height, true);
	grid.Size(offs, 0, m.Width - offs, m.Height);
}
//------------------------------------------------------------------------
void TresholdWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void TresholdWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 800;
		m.pMinMaxInfo->ptMinTrackSize.y = 310;
		m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		m.pMinMaxInfo->ptMaxTrackSize.y = 310;		
	}		
}
//------------------------------------------------------------------------
unsigned TresholdWindow::operator()(TCreate &m)
{
	Menu<TresholdsWindowSpace::MainMenu>().Init(m.hwnd);
	grid.Create(m, &gridViewer);
	tresholdPanelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TresholdPanelViewer>::Proc, L"TresholdPanelViewer", &tresholdPanelViewer);
	return 0;
}
//-------------------------------------------------------------------------
void TresholdWindow::operator()(TSizing &l)
{
}
//------------------------------------------------------------------------
void TresholdWindow::operator()(TDestroy &)
{
	hWnd = NULL;
}
//------------------------------------------------------------------------

void TresholdWindow::operator()(TMouseWell &l)
{

}
unsigned TresholdWindow::operator()(TNotify &m)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(m);
	return 0;
}

void TresholdWindow::Open_()
{
	wchar_t *buf =  L"Добавить образец";
		if(0 == hWnd)
		{			
			TresholdWindowTable::TItems &o = Singleton<TresholdWindowTable>::Instance().items;
			WindowTemplate(this, L"TresholdWindow"
				, buf
				, IDI_UPLOAD
				, o.get<OffsetWindowX>().value
				, o.get<OffsetWindowY>().value
				, o.get<WindowWidth>().value
				, 310//o.get<WindowHeight>().value
				);
		}
		SetWindowText(hWnd, buf);
		SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hWnd);
}
void TresholdWindow::UpdateTresholdGrid()
{
	HWND hWnd = Instance().hWnd;
	if(hWnd)
	{
		//SetWindowText(hWnd, L"TresholdWindow");
		//SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		//SetForegroundWindow(hWnd);
		ListView_Update(Instance().grid.hWnd, 0);
	}
}

void TresholdWindow::Open()
{
	TresholdWindow::Instance().Open_();
}
TresholdWindow &TresholdWindow::Instance()
{
	static TresholdWindow x; 
	return x;
}
