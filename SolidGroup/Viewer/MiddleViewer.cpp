#include "stdafx.h"
#include "MiddleViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "AppBase.h"

#include "DebugMess.h"
#include "SolidData.h"
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

MiddleViewer::CursorLabel::CursorLabel(MiddleViewer &o, double (&d)[30000])
	: data(d)
	, owner(o)
{
	label.fontHeight = 15;
	label.top = 10;	
	label.left = 30;
}
#pragma warning(disable : 4996)
bool MiddleViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
#if 0
	char buf[128];
	double dX = (double)(owner.chart.rect.right - owner.chart.rect.left - owner.chart.offsetAxesLeft - owner.chart.offsetAxesRight) / (owner.chart.maxAxesX - owner.chart.minAxesX);
	double x = owner.chart.rect.left + owner.chart.offsetAxesLeft - dX / 2;
	//double thickness = owner.chart.items.get<VCursor2>().value - owner.chart.items.get<VCursor1>().value;
	//thickness *= owner.coefficientA;
	//thickness += owner.coefficientB;
	//if(thickness < 0) thickness = -thickness;
	char bufTh[128];
	bufTh[0] = 0;
	if(thickness > 3.0)
	{
		sprintf(bufTh, "<ff0000> ÒÎËÙÈÍÀ %.2f ", thickness);
	}
	
	unsigned leftOffs = unsigned(double(l.x - x) / dX);
	if(leftOffs > 0 && dimention_of(data) >= leftOffs)
	{
		double y = data[leftOffs - 1];
		sprintf(buf, "%s<0xff>X <0xff0000>%d   <0xff>Y <0xff0000>%0.1f", bufTh, leftOffs, y - 127);
		RECT r;
		GetClientRect(l.hwnd, &r);
		label = buf;
		label.Draw(g());
		return true;
	}
#endif
	return false;
}

MiddleViewer::MiddleViewer(int &currentOffset)
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, dataSignal(solidData.dataBuffer)
	, dateReference(solidData.referenceBuffer)
	, cursorLabel(*this, dataSignal)
	, mouseMove(true)
    , frameLength(100)
	, currentOffset(currentOffset)

{
	chart.rect.left = 10;
	chart.rect.top = 30;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.minAxesY = -10;
	chart.maxAxesY = 10;
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
}
//----------------------------------------------------------------------------------------------------
template<class O, class P>struct __set_chart_data__
{
	void operator()(O *, P *){}
};
template<int N, class P>struct __set_chart_data__<MiddleViewer::MiddleSignal<N>, P>
{
	typedef MiddleViewer::MiddleSignal<N> O;
	void operator()(O *o, P *)
	{
		o->SetData(solidData.referenceNULL[N], solidData.currentOffsetNull);
	}
};
#pragma warning(disable : 4996)
void MiddleViewer::operator()(TSize &l)
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
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, l.Width, l.Height);   
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;	
	if(currentOffset < 0) currentOffset = 0;
	chart.minAxesX = 0;
	chart.maxAxesX = solidData.currentOffsetNull;
	//if(solidData.currentOffset > 0 && solidData.currentOffset < chart.maxAxesX) 
	{
		//	--currentOffset;
	}
	//chart.items.get<Signal>().SetData(&dataSignal[currentOffset * frameLength], frameLength);//, 0, signalLength - 1);
	//chart.items.get<Reference>().SetData(&dateReference[currentOffset * frameLength], frameLength);
	//SetData(solidData.referenceNULL[N], solidData.currentOffsetNull);
	//chart.items.get<MiddleSignal<1> >().SetData
	TL::foreach<items_list, __set_chart_data__>()(&chart.items, (int *)0);
	chart.Draw(g);
	
	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TPaint &l)
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
void MiddleViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TLButtonDown &l)
{
	//chart.items.get<VCursor1>().SetCoordinate(*(POINTS *)&storedMouseMove.x);
	RepaintWindow(l.hwnd);
}
//----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TRButtonDown &l)
{
	//chart.items.get<VCursor2>().SetCoordinate(*(POINTS *)&storedMouseMove.x);
	RepaintWindow(l.hwnd);
}
//----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;
		storedMouseMove.x -= l.delta / 120;
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//-----------------------------------------------------------------------------------------------------
void MiddleViewer::operator()(TDestroy &l)
{
	//if(NULL != backScreen)delete backScreen;
}
//-----------------------------------------------------------------------------------------------------

