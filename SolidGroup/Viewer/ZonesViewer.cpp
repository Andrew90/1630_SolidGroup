#include "stdafx.h"
#include "ZonesViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
//#include "PrimaryData.h"
#include "LabelMessage.h"
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
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

ZonesViewer::CursorLabel::CursorLabel(Chart &chart, ZonesViewer &d)
	: owner(d)
   // , data(d.data)
	, chart(chart)
	, offset(0)
{
	label.fontHeight = 15;
	label.top = 10;	
}
#pragma warning(disable : 4996)
bool ZonesViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	
	return false;
}
double x_data = 0;
unsigned color_ = 0xff00ff00;
bool ZonesViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color)
{
	color = color_;
	x_data += 0.1;
	if(x_data > 10) x_data = 0;
	return true;
}

ZonesViewer::ZonesViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(chart, *this)
	, offset(cursorLabel.offset)
	, obj(NULL)
	, ptr(NULL)
{
	chart.rect.left = 10;
	chart.rect.top = 30;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.minAxesY = 0;
	
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
	chart.items.get<BarSeries>().SetColorBarHandler(&cursorLabel, &CursorLabel::GetColorBar);
	mouseMove = true; 
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ZonesViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
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
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, l.Width, l.Height); 
	
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;

	chart.minAxesY = 0;//Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value;
	chart.maxAxesY = 10;//Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value;
	chart.minAxesX = zonesOffset;
	chart.maxAxesX = zonesOffset + 100;//data.countZones - 1;

	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TMouseMove &l)
{
	
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
		else
		{
			cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
		}
		if(obj&&ptr)(obj->*ptr)(offset);
	}
}
//-----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
	if(obj&&ptr)(obj->*ptr)(offset);
}
//----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
	if(obj&&ptr)(obj->*ptr)(offset);
}
//---------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;

		chart.items.get<BottomAxesInt>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
		if(obj&&ptr)(obj->*ptr)(offset);
	}
}
//---------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TDestroy &l)
{
}
//------------------------------------------------------------------------------------

