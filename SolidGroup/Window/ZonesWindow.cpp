#include "stdafx.h"
#include "ZonesWindow.h"
#include "AppBase.h"
#include "SensorViewer.h"
#include "typelist.hpp"
#include "WindowsEventTemplate.hpp"
#include "EmptyWindow.h"
#include "dialogs.h"
#include "MenuApi.h"
#include "ZonesWindowToolBar.h"
#include "ZonesWindowMenu.hpp"
#include "DebugMess.h"
//-------------------------------------------------------------------------
ZonesWindow::ZonesWindow() 
	: hWnd(NULL)	
	, frameViewer(zone)
{
}
//-----------------------------------------------------------------------------
void ZonesWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//--------------------------------------------------------------------------------------
void ZonesWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	RECT rt;
	GetClientRect(hToolBar, &rt);
	int dy = rt.bottom;
	RECT r;
	GetClientRect(m.hwnd, &r);	
	MoveWindow(frameViewer.hWnd , 0, dy, r.right, (r.bottom - dy), true);
	dy += 200;
//    MoveWindow(signalViewer.hWnd , 0, dy, r.right / 2, r.bottom - dy, true);
	//MoveWindow(correlationViewer.hWnd, r.right / 2, dy, r.right / 2, r.bottom - dy, true);
}
//-------------------------------------------------------------------------
unsigned ZonesWindow::operator()(TCreate &m)
{
	hToolBar = InitToolbar<detailied_tool_button_list, 16>()(m.hwnd);
	Menu<ZonesWindowMenu::MainMenu>().Init(m.hwnd);
	frameViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<FrameViewer>::Proc, L"frameViewer", &frameViewer);
	return 0;
}
//-------------------------------------------------------------------------
unsigned ZonesWindow::operator()(TMessage &m)
{
	return 0;
}
//-------------------------------------------------------------------------
void ZonesWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 300;
		m.pMinMaxInfo->ptMinTrackSize.y = 200;
	}	
}
//-------------------------------------------------------------------------
void ZonesWindow::operator()(TDestroy &m)
{
	hWnd = 0;
}
//-----------------------------------------------------------------------------
void ZonesWindow::Open_()
{
	    wchar_t buf[128];

		wsprintf(buf, L"Детализированный просмотр. Смещение %d", zone);
		
		if(0 == hWnd)
		{			
			ZonesWindowTable::TItems &o = Singleton<ZonesWindowTable>::Instance().items;
			WindowTemplate(this, L"ZonesWindow", buf, IDI_UPLOAD
				, o.get<OffsetWindowX>().value
				, o.get<OffsetWindowY>().value
				, o.get<WindowWidth>().value
				, o.get<WindowHeight>().value
				);
		}
		frameViewer.currentOffset = zone;
		SetWindowText(hWnd, buf);
		SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hWnd);
	    RepaintWindow(frameViewer.hWnd);
}
//--------------------------------------------------------------------------------------
void ZonesWindow::Open()
{	
	Instance().Open_();
}
//------------------------------------------------------------------------------
ZonesWindow &ZonesWindow::Instance()
{
	static ZonesWindow x; return x;
}
//------------------------------------------------------------------------------------
void ZonesWindow::MouseMoveHandler(unsigned)
{
}
//-----------------------------------------------------------------------------------------------
