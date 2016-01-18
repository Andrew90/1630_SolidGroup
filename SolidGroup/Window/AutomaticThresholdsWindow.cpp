#include "stdafx.h"
#include "AutomaticThresholdsWindow.h"
#include "MenuApi.h"
#include "EmptyWindow.h"
#include "AutomaticChartToolBar.h"

#include "DebugMess.h"

//-------------------------------------------------------------------------
AutomaticThresholdsWindow::AutomaticThresholdsWindow() 
	: hWnd(NULL)
	, panel(automaticOptionsTresholds.tresholds)
	, setRemoveTresholds(automaticOptionsTresholds.thresholdsData, automaticOptionsTresholds.tresholds)
{}
//-----------------------------------------------------------------------------
void AutomaticThresholdsWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//--------------------------------------------------------------------------------------
void AutomaticThresholdsWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	RECT rt;
	GetClientRect(hToolBar, &rt);
	int dy = rt.bottom;
	RECT r;
    GetClientRect(m.hwnd, &r);	
	r.bottom -= dy;
	static const int widthPanel = 225;
#if 1
	MoveWindow(chart.hWnd,          0,                    dy, r.right - widthPanel               , r.bottom / 2, true);
	MoveWindow(automaticChart.hWnd, 0,                    dy + r.bottom / 2, r.right - widthPanel, r.bottom / 2, true);
#else
	MoveWindow(chart.hWnd,          0,                    0, r.right - widthPanel, r.bottom, true);
#endif
	MoveWindow(panel.hWnd, r.right - widthPanel, 0, widthPanel          , dy + r.bottom, true);
	//dy += 200;
}
//-------------------------------------------------------------------------
unsigned AutomaticThresholdsWindow::operator()(TCreate &m)
{
	Menu<AutomaticChartMenu::MainMenu>().Init(m.hwnd);
	hToolBar = InitToolbar<AutomaticChartToolBar::tool_button_list, 16>()(m.hwnd);
	automaticOptionsTresholds.Init();
	panel.hWnd = CreateChildWindowBackground(m.hwnd, (WNDPROC)&Viewer<AutomaticThresholdPanel>::Proc, L"AutomaticThresholdPanel", &panel);
	chart.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<AutomaticTresholdChart>::Proc, L"AutomaticTresholdChart", &chart);	
	chart.chart.items.get<AutomaticTresholdChart::Lines>().lines = &automaticOptionsTresholds.thresholdsData;
	automaticOptionsTresholdsTemplate.Set(&automaticOptionsTresholds, &AutomaticOptionsTresholds::Update);
	panel.callAutomatic = &automaticOptionsTresholdsTemplate;
	automaticTresholdChartTemplate.Set(&chart, &AutomaticTresholdChart::Repaint);
	panel.callRepaintChart = &automaticTresholdChartTemplate;
	setRemoveTresholdsTemplate.Set(&setRemoveTresholds, &SetRemoveTresholds::All);
	panel.callStoreParam = &setRemoveTresholdsTemplate;
	automaticChart.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<AutomaticChart>::Proc, L"AutomaticChart", &automaticChart);
	return 0;
}
//-------------------------------------------------------------------------
void AutomaticThresholdsWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	//	dprint("TGetMinMaxInfo %s", __FUNCTION__);
	//	RepaintWindow(m.hwnd);
	}	
}
//-------------------------------------------------------------------------
void AutomaticThresholdsWindow::operator()(TDestroy &)
{
	hWnd = 0;
}
//-----------------------------------------------------------------------------
AutomaticThresholdsWindow &AutomaticThresholdsWindow::Instance()
{
	static AutomaticThresholdsWindow x; return x;
}
//-----------------------------------------------------------------------------------------------
void AutomaticThresholdsWindow::Open()
{
	wchar_t *buf = L"Просмотр и настройка порогов";

	if(0 == hWnd)
	{			
		WindowTemplate(this, L"AutomaticThresholdsWindow", buf);
	}
	SetWindowText(hWnd, buf);
	SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	SetForegroundWindow(hWnd);
}
