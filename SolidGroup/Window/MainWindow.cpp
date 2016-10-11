#include "stdafx.h"
#include "Device1730.h"
#include "MainWindow.h"
#include <commctrl.h>
#include "MenuApi.h"
#include "Dialogs.h"
#include "AppBase.h"
#include "AdditionalBase.h"
#include "TresholdWindow.h"
#include "Compute.h"
#include "Automat.h"
#include "SaveLoadDates.h"
#include "SolenoidParametersTableDlg.h"
#include "StoredData.h"
#include "Pass.h"
#include "SolidCounter.h"
#include "MainWindowMenu.hpp"
#include "MainWindowToolBar.h"

#include "EmptyWindow.h"
#include "ClientTreshold.h"

#include "UpdateMainChart.h"
#include "SolidCounter.h"

#include "../Resource.h"


#include "DebugMess.h"
//-------------------------------------------------------------------------
MainWindow::MainWindow(): created(false){}

//------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(!created || m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);

	RECT rt;
	GetWindowRect(hToolBar, &rt);
	RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	static const int width = __tool_bar_width__<tool_button_list>::value;
	select.Size(width, 5, 400);
	MoveWindow(hCommunicationCheckBox, width, 52, 250, 20, TRUE);
	MoveWindow(hStoredCheckBox, width, 68, 250, 20, TRUE);


	MoveWindow(hPaintCheckBox, width + 300, 52, 525, 20, TRUE);
	MoveWindow(gridCounter.grid.hWnd, width + 400 + 30,  2, 145, rt.bottom - rt.top - 2 - 2, TRUE);

	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = (r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight);
	int tt = t;
	if(t < 500)
	{
			t /= 3;
			t *= 2;
	}
	else t = 250;
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	MoveWindow(signalViewer.hWnd , 0, y, r.right, t, true);
	y += t;
	MoveWindow(colorPanel.hWnd , 0, y, r.right, tt - t, true);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		//m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		//m.pMinMaxInfo->ptMaxTrackSize.y = 500;		
	}		
}
//------------------------------------------------------------------------
void MainWindow::CommunicationRemoveUnitBtn(TCommand &m)
{
	bool b = BST_CHECKED == Button_GetCheck(m.hControl);
	dprint(__FUNCTION__"checced %d", b);
	Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value = b;
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		Update<DifferentOptionsTable>(base).set<CommunicationRemoveUnit>((int )b).Where().ID(1).Execute();
	}
}
//-----------------------------------------------------------------------------------------------------------
void MainWindow::PaintCheckBoxBtn(TCommand &m)
{
	bool b = BST_CHECKED == Button_GetCheck(m.hControl);
	dprint(__FUNCTION__"checced %d", b);
	Singleton<AdditionalParametersTable>::Instance().items.get<MarkPaint>().value = b;
	//if(!b)
	//{
	//	device1730.Write(0);
	//}
	CBase base(AdditionalParametersBase().name());
	if(base.IsOpen())
	{
		Update<AdditionalParametersTable>(base).set<MarkPaint>((int )b).Where().ID(1).Execute();
	}
}
//--------------------------------------------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
#if 1
	sizingWindow = true;
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
	hToolBar = InitToolbar<tool_button_list>()(m.hwnd);
	select.Create(hToolBar);
	InitBtn();
	//hCommunicationCheckBox = CreateWindow(L"button", L"Работа со станцией"
	//		,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
	//			, 0, 0, 0, 0, hToolBar, NULL, hInstance, NULL
	//			);
	//SetWindowLongPtr(hCommunicationCheckBox, GWLP_USERDATA, (DWORD)MainWindow::CommunicationRemoveUnitBtn);
	Button_SetCheck(hCommunicationCheckBox
		, 0 != Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value 
		? BST_CHECKED : BST_UNCHECKED);

	hStoredCheckBox  = CreateWindow(L"button", L"Сохранение первичного сигнала"
		,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
		, 0, 0, 0, 0, hToolBar, NULL, hInstance, NULL
		);

	//hPaintCheckBox = CreateWindow(L"button", L"Краскоотметчик"
	//		,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
	//			, 0, 0, 0, 0, hToolBar, NULL, hInstance, NULL
	//			);
	//SetWindowLongPtr(hPaintCheckBox, GWLP_USERDATA, (DWORD)MainWindow::PaintCheckBoxBtn);
	//Button_SetCheck(hPaintCheckBox
	//		, 0 != Singleton<AdditionalParametersTable>::Instance().items.get<MarkPaint>().value 
	//		? BST_CHECKED : BST_UNCHECKED);

	//Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value;

	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	
	signalViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<SignalViewer>::Proc, L"SignalViewer", &signalViewer);
	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	colorPanel.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<ColorPanel>::Proc, L"ColorPanel", &colorPanel);
	UpdateMainChart::hWnd = m.hwnd;	
#endif
	topLabelViewer.SetMessage(L"<ff>Установка остановлена");
	SolidCounter sg;
	wchar_t *typeSize = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	sg.Load(typeSize);
	gridCounter.Create(hToolBar);
	created = true;
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
	return 0;
}
//-------------------------------------------------------------------------------------------
unsigned MainWindow::operator()(TNotify &m)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(!IsBadReadPtr(x, sizeof(x)))return x->Notify(m);
	return 0;
}
//-------------------------------------------------------------------------
void MainWindow::operator()(TSizing &l)
{
	if(!sizingWindow)
	{
		RECT r;
		GetWindowRect(l.hwnd, &r);
		*l.pRect = r;
	}
}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &)
{
#if 0
	dprint("TDestroy");
	DestroyGlobalData();
	Sleep(1000);
#endif
	PostQuitMessage(0);
}
//------------------------------------------------------------------------
struct EnableMenuAll
{
	template<class O, class P>struct loc
	{
		void operator()(O *, P *p)
		{
			EnableMenu<O>(p->h, p->en);
		}
	};
	HWND h;
	bool en;
	explicit EnableMenuAll(HWND h, bool en) : h(h), en(en)
	{
	}
};
void MainWindow::DisableMemuItemFile()
{
	EnableMenuAll(hWnd, false);
	EnableWindow(select.hWnd, false);
}
//-------------------------------------------------------------------------
void MainWindow::EnableMemuItemFile()
{
	EnableMenuAll(hWnd, true);
	EnableWindow(select.hWnd, true);
}
//--------------------------------------------------------------------------
void MainWindow::BottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}
//---------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void MainWindow::operator()(TMessage &m)
{
	if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
}
//-------------------------------------------------------------------
void MainWindow::CycleBtn()
{
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_CycleBtn, MAKELONG(false, 0));
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_OptionsBtn, MAKELONG(false, 0));
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(true, 0));
}
//----------------------------------------------------------------
void MainWindow::StopBtn()
{
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_CycleBtn, MAKELONG(true, 0));
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_OptionsBtn, MAKELONG(true, 0));
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(false, 0));
}
//-----------------------------------------------------------------
void MainWindow::InitBtn()
{
	SendMessage(hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(false, 0));
}
//---------------------------------------------------------------

MainWindow mainWindow;