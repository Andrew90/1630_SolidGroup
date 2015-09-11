#include "stdafx.h"
#include "TresholdsGridViewer.h"
#include <strsafe.h>
#include "MenuApi.h"
#include "Grid.h"
#include "typelist.hpp"
#include "ArchiveEvents.h"
#include "Corel.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

template<class T>struct header_table;
#define Z(T, w, txt)template<>struct header_table<T>\
{\
	LPWSTR name(){return L###txt;}\
	static const int width = w;\
};
/*
#define DEFINE_PARAM_NUM(z, n, type, def_val) template<>struct z<n>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L###z##L###n;}\
	z() : value(def_val), default_value(def_val) {}\
};
*/
template<int >struct El{};
//template<int N>struct header_table<El<N> >;
#define Z_NUM(T, num, w, txt)template<>struct header_table<T<num> >\
{\
	LPWSTR name(){return L###txt;}\
	static const int width = w;\
};

struct Num{};
struct Groups{};
struct Corelation{};

Z(Num       , 60, №)
Z(Groups     , 60, Гр.пр.)
Z(Corelation, 60, Корел.)
Z_NUM(El, 1 , 60, 1)
Z_NUM(El, 2 , 60, 2)
Z_NUM(El, 3 , 60, 3)
Z_NUM(El, 4 , 60, 4)
Z_NUM(El, 5 , 60, 5)
Z_NUM(El, 6 , 60, 6)
Z_NUM(El, 7 , 60, 7)
Z_NUM(El, 8 , 60, 8)
#undef Z

typedef TL::MkTlst<
  Num       
  , Groups     
  , Corelation
  , El<1>        
  , El<2>        
  , El<3>        
  , El<4>        
  , El<5>        
  , El<6>        
  , El<7>        
  , El<8>        
>::Result ParameterNameList;
//-----------------------------------------------------------------------------------
template<class T>struct DataToGrid
{
	typedef typename T::__template_must_be_overridded__ noused;
};
//------------------------------------------------------------------------------------
//extern ClientEvents clientEvents;
//-------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
template<>struct DataToGrid<Num>
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			return  Wchar_from<int>(i)();
		}
		return L"";
	}
};
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<Groups>
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			try
			{
			int t = corel.solidTubeItem[i]->classTube;
			return (wchar_t *)(corel.classTubeItem[t] ? corel.classTubeItem[t]->Name.c_str() : L"");
			}
			catch(...){}
		}
		return L"";
	}
};
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<Corelation>
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->correlation;
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
//------------------------------------------------------
template<>struct DataToGrid<El<1> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[0];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<2> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[1];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<3> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[2];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<4> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[3];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<5> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[4];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<6> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[5];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<7> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[6];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
template<>struct DataToGrid<El<8> >
{
	wchar_t *operator()(unsigned i)
	{
		if(corel.solidTubeItem.size() > i)
		{
			double t = corel.solidTubeItem[i]->elements[7];
		    return Wchar_from<double>(t)();
		}
		return L"";
	}
};
//---------------------------------------------------------------------------------------
/*
template<>struct DataToGrid<El<TL::IntToType<N> > >
{
	wchar_t *operator()(unsigned i)
	{
		return L"Ell";//clientEvents.GetData(i);
	}
};
*/
//--------------------------------------------------------------------------------------
template<class >struct ColorToGrid
{
	void operator()(ClientEvents::DataColor &d)
	{
		d.d->clrText = 0;
		d.d->clrTextBk = -1;
	}
};
/*
template<>struct ColorToGrid<Data>
{
	void operator()(ClientEvents::DataColor &d)
	{
		//clientEvents.GetDataColor(d);
	}
};
template<>struct ColorToGrid<Message>
{
	void operator()(ClientEvents::DataColor &d)
	{
		//clientEvents.GetTextColor(d);
	}
};
*/
//---------------------------------------------------------------------------------------
typedef TL::ListToWapperList<ParameterNameList, DataToGrid>::Result __proc_list__;
TL::Factory<__proc_list__> rows;
TL::ArrayFunc<__proc_list__, unsigned, wchar_t *> dataFunc(rows);

//typedef TL::ListToWapperList<ParameterNameList, ColorToGrid>::Result __proc_color__list__;
//TL::Factory<__proc_color__list__> color_rows;
//TL::ArrayFunc<__proc_color__list__, ClientEvents::DataColor &, void> dataColorFunc(color_rows);
//--------------------------------------------------------------------------------
VOID WINAPI UpdateListView(HWND h, bool)
{
	/*
	if(mainGrid.start != clientEvents.map->head)
	{
		mainGrid.start = clientEvents.map->head;
		ListView_Update(h, 0);
	}
	*/
}
//--------------------------------------------------------------------------------
TresholdeGridWiewer::TresholdeGridWiewer()
{}
//--------------------------------------------------------------------------------
void TresholdeGridWiewer::SetHeaderToGrid(HWND h)
{
	SetGridHeader<ParameterNameList> x(h);
	SetRow(h, 1000);
}
//--------------------------------------------------------------------------------
void TresholdeGridWiewer::SetDataToGrid(LV_DISPINFO *d)
{	
	unsigned i = d->item.iItem;
	wchar_t *c = dataFunc[d->item.iSubItem](i);
	StringCchCopy(d->item.pszText, wcslen(c) + 1, c);		
}
//-----------------------------------------------------------------------------------
void TresholdeGridWiewer::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	/*
	int t = 99 - d->nmcd.dwItemSpec;
	//dprint("__%d", t);
	if(t > 0)
	dataColorFunc[d->iSubItem](ClientEvents::DataColor(d, 99 - d->nmcd.dwItemSpec));
	*/
}
//------------------------------------------------------------------------------------
#if 0
/*
if(d->iItem >= 0 && d->iSubItem >= 0 && d->iSubItem < dimention_of(hEdits))
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, d->iSubItem, buffer, 256);
		SetWindowText(hEdits[d->iSubItem], buffer); 
		if(0 == d->iSubItem)
		{
			for(std::map<int, Corel::ClassTubeItem *>::iterator i = corel.classTubeItem.begin(); i != corel.classTubeItem.end(); ++i)
			{
				if(i->second->Name == buffer)
				{
					corel.inputItem.classTube = i->first;
					HWND h = GetParent(d->hdr.hwndFrom);
					TresholdPanelViewer *o = (TresholdPanelViewer *)GetWindowLongPtr(h, GWLP_USERDATA);
					o->SetResultMessageLabel();
					break;
				}
			}
		}
	}
*/
/*
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
*/
#endif
	namespace TresholdeGridWiewer_Menu
{
	struct MainOptionUnits{};
	struct Del
	{
		static void Do(HWND h)
		{
			dprint("Do");
		}
	};

	template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<TresholdeGridWiewer>
	>::Result list;
};
template<>struct TopMenu<Del>
{
	typedef TL::MkTlst<
		MenuItem<Del>
	>::Result list;
};

	typedef TL::MkTlst<
		TopMenu<MainOptionUnits>
		, TopMenu<Del>
	>::Result MainMenu;

	MENU_TEXT(L"Редактирование образцов", TopMenu<MainOptionUnits>)
	MENU_TEXT(L"Удалить образец", MenuItem<TresholdeGridWiewer>)

	template<>struct Event<MenuItem<TresholdeGridWiewer> >{static void Do(HWND h){TresholdeGridWiewer::RemoveRow(h);}};
	template<>struct Event<MenuItem<Del> >{static void Do(HWND h){Del::Do(h);}};
}
void TresholdeGridWiewer::LClick(LPNMITEMACTIVATE d)
{
	corel.selectedRow = d->iItem;
	//HWND h = GetParent(d->hdr.hwndFrom);
	//PopupMenu<TresholdeGridWiewer_Menu::MainMenu>().Init(h);
}
void TresholdeGridWiewer::RemoveRow(HWND h)
{
	if(corel.selectedRow < corel.solidTubeItem.size())
	{
		corel.RemoveElement(*corel.solidTubeItem[corel.selectedRow]);
	}
}





