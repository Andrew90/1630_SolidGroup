#include "stdafx.h"
#include "MiddleWindow.h"
#include "AppBase.h"
#include "MiddleViewer.h"
#include "typelist.hpp"
#include "WindowsEventTemplate.hpp"
#include "EmptyWindow.h"
//#include "PrimaryData.h"
//#include "compute.h"
#include "dialogs.h"
#include "MenuApi.h"
#include "MiddleWindowToolBar.h"
#include "MiddleWindowMenu.hpp"
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
//#pragma message("Смотри отоототот")
//int packet = 777;
//-------------------------------------------------------------------------
MiddleWindow::MiddleWindow() 
	: hWnd(NULL)	
	//, signalLength(packet)//(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, middleViewer(zone)
{
//	zonesViewer.SetMouseMoveHandler(this, &ZonesWindow::MouseMoveHandler);
}
//-----------------------------------------------------------------------------
void MiddleWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//--------------------------------------------------------------------------------------
void MiddleWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	//MoveWindow(hToolBar, 0, 0, 0, 0, false);
	//RECT rt;
	//GetClientRect(hToolBar, &rt);
	//int dy = rt.bottom;
	RECT r;
	GetClientRect(m.hwnd, &r);	
	MoveWindow(middleViewer.hWnd , 0, 0, r.right, (r.bottom ), true);
	//dy += 200;
}
//-------------------------------------------------------------------------
unsigned MiddleWindow::operator()(TCreate &m)
{
	///*
	//hToolBar = InitToolbar<MiddleWindowToolBar::detailied_tool_button_list, 16>()(m.hwnd);
	Menu<MiddleWindowMenu::MainMenu>().Init(m.hwnd);
	middleViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<MiddleViewer>::Proc, L"middleViewer", &middleViewer);
	//*/
	return 0;
}
//-------------------------------------------------------------------------
unsigned MiddleWindow::operator()(TMessage &m)
{
	return 0;
}
//-------------------------------------------------------------------------
void MiddleWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 300;
		m.pMinMaxInfo->ptMinTrackSize.y = 200;
	//	dprint("TGetMinMaxInfo %s", __FUNCTION__);
	//	RepaintWindow(m.hwnd);
	}	
}
//-------------------------------------------------------------------------
void MiddleWindow::operator()(TDestroy &m)
{
	hWnd = 0;
}
//-----------------------------------------------------------------------------
void MiddleWindow::Open_()
{
	    wchar_t buf[128];

		wsprintf(buf, L"Детализированный просмотр. Смещение %d", zone);
		
		if(0 == hWnd)
		{			
			//ZonesWindowTable::TItems &o = Singleton<ZonesWindowTable>::Instance().items;
			WindowTemplate(this, L"MiddleWindow", buf);
			/*
				, IDI_UPLOAD
				, o.get<OffsetWindowX>().value
				, o.get<OffsetWindowY>().value
				, o.get<WindowWidth>().value
				, o.get<WindowHeight>().value
				);
				*/
		}
		middleViewer.currentOffset = zone;
		SetWindowText(hWnd, buf);
		SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hWnd);
	    RepaintWindow(middleViewer.hWnd);
}
//--------------------------------------------------------------------------------------
void MiddleWindow::Open()
{	
	Instance().Open_();
}
//------------------------------------------------------------------------------
MiddleWindow &MiddleWindow::Instance()
{
	static MiddleWindow x; return x;
}
//------------------------------------------------------------------------------------
void MiddleWindow::MouseMoveHandler(unsigned)
{
	/*
	unsigned offs = offset + offsetInZone;
	char *s = primaryData.SensorData(sensor, offs);
	for(int i = 0; i < signalLength; ++i)
	{
		signalViewer.data[i] = s[i] + 128;
	}
	RepaintWindow(signalViewer.hWnd);
	double (&data)[1024] = correlationViewer.data;
	int i = 0;
	double t = 0;
	for(; i < signalLength; ++i)
	{
		data[i] = s[i];
		t += s[i];
	}
	t /= signalLength;
	for(; i < fft.bufferSize; ++i)
	{
		data[i] = t;
	}
	fft.Direct(data);
	fft.Spectrum(data);
	fft.Direct(data);
	fft.Spectrum(data);
	correlationViewer.thickness = primaryData.result[sensor][offs];
	correlationViewer.chart.maxAxesY = data[0];
	if(correlationViewer.chart.maxAxesY < 1) correlationViewer.chart.maxAxesY = 1;
	correlationViewer.chart.maxAxesX = correlationViewer.correlationLength/2 - 1;
	RepaintWindow(correlationViewer.hWnd);
	*/
}
//-----------------------------------------------------------------------------------------------
