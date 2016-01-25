#include "stdafx.h"
#include "AutomaticThresholdPanel.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "AppBase.h"
#include "templates.hpp"
#include "MenuApi.h"
#include "Pass.h"
#include "AutomaticThresholdsWindow.h"
#include "GlobalHandlers.h"

#include "DebugMess.h"

//------------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
extern HINSTANCE hInstance;
AutomaticThresholdPanel::AutomaticThresholdPanel(int (&data)[8])
	: tresholds(data)
{}

unsigned AutomaticThresholdPanel::operator()(TCreate &l)
{
	int y = 20;
	wchar_t buf[128] = L"Смещение ";
	wchar_t *s = &buf[wcslen(buf)];
	for(int i = 0; i < dimention_of(arrHWND); ++i)
	{
		Wchar_from<double, 9> data(tresholds[i]);
		arrHWND[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", data()
			, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			,10, y, 125, 25, l.hwnd, 0, hInstance, NULL
			);
		_itow(1 + i, s, 10);
		CreateWindow(L"static", buf
			, WS_VISIBLE | WS_CHILD
			, 135, y + 5, 280, 20, l.hwnd, 0, hInstance, NULL
			);
		y += 30;
	}
	
	hUpdeteBtn = CreateWindow(L"button", L"Обновить график"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,10, y, 200, 25, l.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(hUpdeteBtn, GWLP_USERDATA, (DWORD)&AutomaticThresholdPanel::UpdeteBtn);
	hStoreParamBtn = CreateWindow(L"button", L"Сохранить настройки"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,0, 0, 0, 0, l.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(hStoreParamBtn, GWLP_USERDATA, (DWORD)&AutomaticThresholdPanel::StoreParamBtn);
	return 0;
}
//----------------------------------------------------------------------------------------------------
void AutomaticThresholdPanel::operator()(TSize &l)
{
	RECT r;
	GetClientRect(l.hwnd, &r);
	MoveWindow(hStoreParamBtn, 10, r.bottom - 30, 200, 25, TRUE);
}
//
void AutomaticThresholdPanel::operator()(TCommand &l)
{
	GetMenuToolBarEvent(l);
}
//-----------------------------------------------------------------------------------------------------
 void AutomaticThresholdPanel::UpdeteBtn(TCommand &l)
 {
	 AutomaticThresholdPanel *o = (AutomaticThresholdPanel *)GetWindowLongPtr(l.hwnd, GWLP_USERDATA);
	 wchar_t buf[512];
	 for(int i = 0; i < dimention_of(o->arrHWND); ++i)
	 {
		 Edit_GetText(o->arrHWND[i], buf, dimention_of(buf));
		 if(!Test<int>()(buf))
		 {
			 wchar_t mess[512];
			 wsprintf(mess, L"Параметр \"Порог %d\" имеет недопустимое значение", i);
			 MessageBox(l.hwnd, mess, L"Ошибка!!!", MB_ICONERROR);
			 return;
		 }
	 }
	 for(int i = 0; i < dimention_of(o->arrHWND); ++i)
	 {
		 Edit_GetText(o->arrHWND[i], buf, dimention_of(buf));
		 o->tresholds[i] =  _wtoi(buf);
	 }
	 for(int i = 1; i < dimention_of(o->arrHWND); ++i)
	 {
		 if(o->tresholds[i - 1] > o->tresholds[i])
		 {
			 wchar_t mess[512];
			 wsprintf(mess, L"Должно быть выполнено условие \"Порог %d\" < \"Порог %d\"", i - 1, i);
			 MessageBox(l.hwnd, mess, L"Ошибка!!!", MB_ICONERROR);
			 return;
		 }
	 }
	 o->callAutomatic->Do();
	 o->callRepaintChart->Do();
	 AutomaticThresholdsWindow::Instance().automaticChart.Repaint();
 }
 //---------------------------------------------------------------------
 void AutomaticThresholdPanel::StoreParamBtn(TCommand &l)
 {
	 if(TypesizePasswordDlg().Do(l.hwnd))
	 {
		 AutomaticThresholdPanel *o = (AutomaticThresholdPanel *)GetWindowLongPtr(l.hwnd, GWLP_USERDATA);
		 o->callStoreParam->Do();
		 UpdateReferencePointsGrid()();
	 }
 }
