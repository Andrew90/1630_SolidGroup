#include "stdafx.h"
#include "SolidGridViewer.h"
#pragma warning(disable : 4995)
#include <strsafe.h>
#include "Grid.h"
#include "typelist.hpp"
#include "corel.h"
#include "TresholdPanelViewer.h"

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
#define Z_NUM(T, num, w, txt)template<>struct header_table<T<num> >\
{\
	LPWSTR name(){return L###txt;}\
	static const int width = w;\
};

struct Group{};
struct CommunicationID{};

Z(Group     , 130, Группа прочности)
Z(CommunicationID, 65, Номер TCP)
#undef Z

typedef TL::MkTlst<
   Group     
  , CommunicationID
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
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<Group>
{
	wchar_t *operator()(unsigned i)
	{
		unsigned size = corel.classTubeItem.size();
		if(size > 0 && size > i)
		{
			std::map<int, Corel::ClassTubeItem *>::const_iterator t = corel.classTubeItem.cbegin();
			while(i > 0)
			{
				--i;
				++t;
			}
			if(t->second)
			{
				return (wchar_t *)t->second->Name.c_str();
			}
		}
		return L"";
	}
};
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<CommunicationID>
{
	wchar_t *operator()(unsigned i)
	{
		unsigned size = corel.classTubeItem.size();
		if(size > 0 && size > i)
		{
			std::map<int, Corel::ClassTubeItem *>::const_iterator t = corel.classTubeItem.cbegin();
			while(i > 0)
			{
				--i;
				++t;
			}
			if(t->second)
			{
				return Wchar_from<int>(t->second->communicationID)();
			}
		}
		return L"";
	}
};
//--------------------------------------------------------------------------------------
/*
template<class >struct ColorToGrid
{
	void operator()(ClientEvents::DataColor &d)
	{
		d.d->clrText = 0;
		d.d->clrTextBk = -1;
	}
};
*/
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
void SolidGridGridViewer::SetHeaderToGrid(HWND h)
{
	SetGridHeader<ParameterNameList> x(h);
	SetRow(h, 1000);
}
//--------------------------------------------------------------------------------
void SolidGridGridViewer::SetDataToGrid(LV_DISPINFO *d)
{	
	unsigned i = d->item.iItem;
	wchar_t *c = dataFunc[d->item.iSubItem](i);
	StringCchCopy(d->item.pszText, wcslen(c) + 1, c);		
}
//-----------------------------------------------------------------------------------
void SolidGridGridViewer::LClick(LPNMITEMACTIVATE d)
{
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
}
//-----------------------------------------------------------------------------------
void SolidGridGridViewer::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	/*
	int t =  d->nmcd.dwItemSpec;
	//dprint("__%d", t);
	if(t > 0)
	dataColorFunc[d->iSubItem](ClientEvents::DataColor(d, 99 - d->nmcd.dwItemSpec));
	*/
}
//------------------------------------------------------------------------------------



