#include "stdafx.h"
#include "GridCounter.h"
#include <strsafe.h>
#include "typelist.hpp"
#include "Grid.h"
#include <algorithm>

template<class T>struct header_table;

namespace GridCounterViewerSpace
{
	//struct Color{};
	struct Name{};
	struct Count{};

	typedef TL::MkTlst</*Color,*/ Name, Count>::Result list;
		
#define Z(T, w, txt)template<>struct header_table<T>\
	{\
	LPWSTR name(){return txt;}\
	static const int width = w;\
	};

	//Z(Color, 30, L"")
	Z(Name , 50, L"")
	Z(Count, 70, L"количество")

#undef Z

	struct DataParam
	{
       GridCounterViewer &self;
	   int row;
	   DataParam(GridCounterViewer &self, int r) : self(self), row(r){}
	};

	struct ColorParam
	{
       GridCounterViewer &self;
	   int row;
	   unsigned clrText;
	   unsigned clrTextBk;
	   ColorParam(GridCounterViewer &self, int r) : self(self), row(r), clrText(-1), clrTextBk(-1){}
	};

	template<class T>struct DataToGrid
	{
		wchar_t *operator()(DataParam &i)
		{
			return L"";
		}
	};
	template<class >struct ColorToGrid
	{
		void  operator()(ColorParam &i)
		{
			i.clrText = 0;
			i.clrTextBk = -1;
		}
	};

	template<>struct DataToGrid<Name>
	{
		wchar_t *operator()(DataParam &i)
		{
			if(0 == i.row) return L"Всего";
			if(i.row - 1 < (int)i.self.row.data.size())
			{
				std::map<std::wstring, int>::iterator b = i.self.row.data.begin();
				for(int j = 0; j < i.row - 1; ++j) ++b;
			
				return (wchar_t *)b->first.c_str();
			}
			return L"";
		}
	};	

	template<>struct DataToGrid<Count>
	{
		wchar_t buf[32];
		wchar_t *operator()(DataParam &i)
		{
			if(0 == i.row)
			{
				int sum = 0;
				for(std::map<std::wstring, int>::const_iterator j = i.self.row.data.cbegin(); j != i.self.row.data.cend(); ++j)
				{
					sum += j->second;
				}		
				_itow_s(sum, buf, 10);
				return buf;
			}
			if(i.row - 1 < (int)i.self.row.data.size())
			{
				std::map<std::wstring, int>::const_iterator b = i.self.row.data.cbegin();
				for(int j = 0; j < i.row - 1; ++j) ++b;
			    _itow_s(b->second, buf, 10);
				return buf;
			}
			return L"";
		}
	};	
};
//----------------------------------------------------------------
GridCounterViewer::GridCounterViewer()
	: row(SolidCounterData::Instance())
{
}

void GridCounterViewer::Create(HWND hwnd)
{
	grid.Create(hwnd, this);
	HFONT font = CreateFont(12,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(grid.hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
}
//------------------------------------------------------------
typedef TL::ListToWapperList<GridCounterViewerSpace::list, GridCounterViewerSpace::DataToGrid>::Result __proc_list__;
TL::Factory<__proc_list__> rows;

TL::ArrayFunc<__proc_list__, GridCounterViewerSpace::DataParam &, wchar_t *> dataFunc(rows);

typedef TL::ListToWapperList<GridCounterViewerSpace::list, GridCounterViewerSpace::ColorToGrid>::Result __proc_color__list__;
TL::Factory<__proc_color__list__> color_rows;
TL::ArrayFunc<__proc_color__list__, GridCounterViewerSpace::ColorParam &, void> dataColorFunc(color_rows);

void GridCounterViewer::SetHeaderToGrid(HWND h)
{
	SetGridHeader<GridCounterViewerSpace::list> x(h);
	SetRow(h, 50);
}
//--------------------------------------------------------------
void GridCounterViewer::SetDataToGrid(LV_DISPINFO *d)
{
	wchar_t *c = dataFunc[d->item.iSubItem](GridCounterViewerSpace::DataParam(*this, d->item.iItem));
	StringCchCopy(d->item.pszText, wcslen(c) + 1, c);	
}
//--------------------------------------------------------------
void GridCounterViewer::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	/*
	GridCounterViewerSpace::ColorParam x(*this, d->nmcd.dwItemSpec);
	dataColorFunc[d->iSubItem](x);
	d->clrText = x.clrText;
	d->clrTextBk = x.clrTextBk;
	*/
}
//---------------------------------------------------------------------
void GridCounterViewer::Update()
{
	int i = ListView_GetTopIndex(grid.hWnd);
	int count =  ListView_GetCountPerPage(grid.hWnd);
	for(int k = 0; k <= count; ++k, ++i)
	{
		ListView_Update(grid.hWnd, i);
	}
}
