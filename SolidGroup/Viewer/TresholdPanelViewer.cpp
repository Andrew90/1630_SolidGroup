#include "stdafx.h"
#include "TresholdPanelViewer.h"
#include "TresholdWindow.h"
#include "MenuApi.h"
#include <algorithm>
#include <string>
#include <stdio.h>
#include "EmptyWindow.h"
#include "AppBase.h"
#include "GlobalHandlers.h"

#include "DebugMess.h"
#include "SolidData.h"
#include "ParamDlg.hpp"
#include "Corel.h"
#include "templates.hpp"
#include "Config.h"

//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;



TresholdPanelViewer::TresholdPanelViewer()
	: backScreen(NULL)
{
}
//-------------------------------------------------------------------------------
extern HINSTANCE hInstance;
unsigned TresholdPanelViewer::operator()(TCreate &m)
{
	CreateWindow(L"static", L"Результат измерения", WS_VISIBLE | WS_CHILD, 10, 3, 280, 20, m.hwnd, 0, hInstance, NULL);
	hResultMessageLabel = CreateWindow(L"static", L"XXXXXXXXXXXXXXXXXXXXXXXXXXXX", WS_VISIBLE | WS_CHILD, 10, 3 + 20, 500, 20, m.hwnd, 0, hInstance, NULL);
	/*
	comboBoxWnd = CreateWindow(L"COMBOBOX", NULL
			, WS_CHILD | WS_VISIBLE | CBS_SIMPLE |CBS_AUTOHSCROLL |WS_VSCROLL 
			, 10, 20, 200, 125, m.hwnd, 0, hInstance, 0
			);	
			*/
	static const int dy = 50;
	addUnitBtnWnd     = CreateWindow(L"button",L"Добавить образец"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 240, 20 + dy, 150, 30, m.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(addUnitBtnWnd, GWLP_USERDATA, (DWORD)&TresholdPanelViewer::AddUnitBtn);
	removeUnitBtnWnd  = CreateWindow(L"button", L"Удалить образец"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				, 240, 60 + dy, 150, 30, m.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(removeUnitBtnWnd, GWLP_USERDATA, (DWORD)&TresholdPanelViewer::RemoveUnitBtn);
	addNameBtnWnd  = CreateWindow(L"button",L"Добавить гр.проч."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 5, 160 + dy, 125, 30, m.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(addNameBtnWnd, GWLP_USERDATA, (DWORD)&TresholdPanelViewer::AddNameBtn);
	removeNameBtnWnd  = CreateWindow(L"button",L"Удалить гр.проч."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 135, 160 + dy, 125, 30, m.hwnd, NULL, hInstance, NULL
				);
	SetWindowLongPtr(removeNameBtnWnd, GWLP_USERDATA, (DWORD)&TresholdPanelViewer::RemoveNameBtn);

	//renameGroup = CreateWindow(L"button",L"Изменить имя гр."
		//		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			//	, 265, 160 + dy, 125, 30, m.hwnd, NULL, hInstance, NULL
			//	);
//	SetWindowLongPtr(renameGroup, GWLP_USERDATA, (DWORD)&TresholdPanelViewer::RenameNameBtn);

	 hEditGroup = CreateWindow(L"edit", L""
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				, 5, 135 + dy, 125, 20, m.hwnd, 0, hInstance, NULL
				);
	 hEditCommunicationID = CreateWindow(L"edit", L""
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				, 135, 135 + dy, 125, 20, m.hwnd, 0, hInstance, NULL
				);

	// editRenameGroup = CreateWindow(L"edit", L""
		//		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			//	, 265, 135 + dy, 125, 20, m.hwnd, 0, hInstance, NULL
			//	);

	grid.Create(m, &solidGridGridViewer);
	solidGridGridViewer.hEdits[0] = hEditGroup;
	solidGridGridViewer.hEdits[1] = hEditCommunicationID;
	return 0;
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void TresholdPanelViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || l.Height < 100) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
    Graphics g(backScreen);
	g.FillRectangle(&SolidBrush(Color(0xfff0f0f0)), 0, 0, l.Width, l.Height);   
	grid.Size(10, 3 + 50, 220, 125);
}
//----------------------------------------------------------------------------------------------------
void TresholdPanelViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void TresholdPanelViewer::operator()(TDestroy &l)
{
	//if(NULL != backScreen)delete backScreen;
}
//-----------------------------------------------------------------------------------------------------
void TresholdPanelViewer::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
void TresholdPanelViewer::RemoveUnitBtn	 (TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		if(corel.selectedRow != -1 && corel.selectedRow < corel.solidTubeItem.size())
		{
			Corel::SolidTubeItem x = *corel.solidTubeItem[corel.selectedRow];
			corel.RemoveElement(x);
			corel.selectedRow = -1;
			HWND h = GetParent(m.hwnd);
			TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			ListView_Update(o->grid.hWnd, 0);
		}
	}
}

void GlobalHandlers<UpdateReferencePointsGridHandler>::operator()()
{
	TresholdWindow &o = TresholdWindow::Instance();
	if(0 != o.hWnd)
	{
		ListView_Update(o.grid.hWnd, 0);
	}
}

void TresholdPanelViewer::AddUnitBtn	 (TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		HWND h = GetParent(m.hwnd);
		TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		if(0 != corel.inputItem.classTube)
		{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
				Select<CurrentParametersTable>(base).ID(1).Execute(current);

				corel.inputItem.typeSize = current.items.get<CurrentID>().value;
				corel.InsertElement(corel.inputItem);
				ListView_Update(o->grid.hWnd, 0);
			}
		}
		else
		{
			MessageBox(m.hwnd, L"Необходимо выбрать группу прочности", L"Ошибка!!!", MB_ICONEXCLAMATION);
		}
	}
}
struct rem
{
	std::wstring s;
	rem(std::wstring s) : s(s) {}
};
void TresholdPanelViewer::RemoveNameBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
	TresholdPanelViewer *o = (TresholdPanelViewer *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
	wchar_t editBuf[128];
	GetWindowText(o->hEditGroup, editBuf, 128);
	if(corel.RemoveGroupeName(editBuf))
	{
		for(std::map<int, Corel::ClassTubeItem *>::iterator i = corel.classTubeItem.begin(); i != corel.classTubeItem.end(); ++i)
		{
			if(i->second->Name == editBuf)
			{
				delete i->second;
				corel.classTubeItem.erase(i);
				break;
			}
		}
		ListView_Update(o->grid.hWnd, 0);
	}
	}
}

/*
void TresholdPanelViewer::RenameNameBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		TresholdPanelViewer *o = (TresholdPanelViewer *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		wchar_t editBuf[128];
		GetWindowText(o->hEditGroup, editBuf, 128);
		wchar_t editRenameGroupBuf[128];
	    GetWindowText(o->editRenameGroup, editRenameGroupBuf, 128);
		int id = corel.ReNameGroupeName(editBuf, editRenameGroupBuf);
		if(id)
		{
			Corel::ClassTubeItem *t = corel.classTubeItem[id];
			if(NULL != t)
			{
				t->Name = editRenameGroupBuf;
				corel.classTubeItem[id] = t;
				ListView_Update(o->grid.hWnd, 0);
			}

		}
	}
}
*/
void TresholdPanelViewer::AddNameBtn   (TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		TresholdPanelViewer *o = (TresholdPanelViewer *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		wchar_t editBuf[128];
		GetWindowText(o->hEditGroup, editBuf, 128);
		if(editBuf[0] == 0)
		{
			MessageBox(o->hWnd, L"Необходимо ввести группу прочности", L"Ошибка", MB_ICONERROR);
			return;
		}
		wchar_t comm[128];
		GetWindowText(o->hEditCommunicationID, comm, 128);
		if(Test<int>()(comm))
		{
			int comID = _wtoi(comm);
			int id = corel.InsertGroupeName(editBuf, comID);
			if(id)
			{
				Corel::ClassTubeItem *t = corel.classTubeItem[id];
				if(NULL == t)
				{
					t = new Corel::ClassTubeItem;
					t->Name = editBuf;
					corel.classTubeItem[id] = t;
				}
				corel.classTubeItem[id]->communicationID = comID;
				ListView_Update(o->grid.hWnd, 0);
			}
		}
		else
		{
			MessageBox(o->hWnd, L"Должно быть введено число", L"Ошибка", MB_ICONERROR);
		}
	}
}
unsigned TresholdPanelViewer::operator()(TNotify &m)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(m);
	return 0;
}
#include "MenuApi.h"
namespace test
{
struct MainFile{};
struct MainExit{};
struct CoordinatesZonesDlg{};

struct MainOptionUnits{};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<CoordinatesZonesDlg>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"tesy окна", MenuItem<CoordinatesZonesDlg>)
	
template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};
template<>struct Event<MenuItem<CoordinatesZonesDlg> >{static void Do(HWND h){/*CoordinatesZonesDlg::Do(h)*/;}};
}
void TresholdPanelViewer::operator()(TRButtonDown &m)
{	
	PopupMenu<test::MainMenu>().Init(m.hwnd);
}

void TresholdPanelViewer::SetResultMessageLabel()
{
	std::wstring s;
	if(0 != corel.inputItem.classTube && corel.classTubeItem[corel.inputItem.classTube])
	{
		s = corel.classTubeItem[corel.inputItem.classTube]->Name;
		s += L"  ";
	}
	else
	{
		s = L"нет группы  ";
	}

	for(int i = 0; i < Corel::SolidTubeItem::size_array; ++i)
	{
		s += Wchar_from<double, 5>(corel.inputItem.elements[i])();
		s += L" ";
	}
	SetWindowText(hResultMessageLabel, s.c_str());
#ifdef L502OFF
	wchar_t path[1024];
	 GetModuleFileName(0, path, 1024);
	 PathRemoveFileSpec(path);
	 wcscat(path, L"\\Points\\");
	 CreateDirectory(path, NULL);
	 time_t t = time(0);
	 struct tm *now = localtime( & t );
	 wsprintf(&path[wcslen(path)], L"%04d-%02d-%02d.txt"
		 , now->tm_year + 1900
		 , now->tm_mon + 1
		 , now->tm_mday
		 );
	 s += L"\n";
	 FILE *f = _wfopen(path, L"at+");
	 if(f)
	 {
		 fwrite(s.c_str(), 2 * (s.length() + 1), 1, f);
		 fclose(f);
	 }
#endif	
}