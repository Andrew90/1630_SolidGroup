#include "stdafx.h"
#include "AutomaticTresholdChart.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "LabelMessage.h" 
#include "Corel.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

AutomaticTresholdChart::AutomaticTresholdChart()
	: backScreen(NULL)
	, chart(backScreen)
	, minY(Singleton<PointsOptionsTable>::Instance().items.get<MinAxesY>().value)
	, maxY(Singleton<PointsOptionsTable>::Instance().items.get<MaxAxesY>().value)
{
	chart.rect.left = 10;
	chart.rect.top = 20;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 1;
	chart.maxAxesX = 8;
	
	label.fontHeight = 15;
	
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void AutomaticTresholdChart::operator()(TSize &l)
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
	chart.minAxesY = minY;
	chart.maxAxesY = maxY;
	chart.Draw(g);
	wchar_t buf[512];
	buf[0] = 0;
	wchar_t *s = buf;
	for(Corel::TClassTubeItem::const_iterator i = corel.classTubeItem.cbegin(); i != corel.classTubeItem.cend(); ++i)
	{
		wsprintf(s, L"<%8x>%s ", i->second->color, i->second->Name.c_str());
		s = &s[wcslen(s)]; 
	}
	
	if(0 != corel.inputItem.classTube)
	{
	wsprintf(s, L" <ff0000>Результат %s <ff>%s"
		, (wchar_t *)corel.classTubeItem[corel.inputItem.classTube]->Name.c_str()
		,  Wchar_from<double>(corel.inputItem.correlation)()
		);
	//corel.inputItem.classTube = 0;
	}
	else
	{
		s = L"";
	}
	label = buf;
	label.Draw(g);	

}
//----------------------------------------------------------------------------------------------------
void AutomaticTresholdChart::operator()(TPaint &l)
{
	if(NULL != backScreen)
	{
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(l.hwnd, &p);
		{		
			Graphics g(hdc);		
			g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		}
		EndPaint(l.hwnd, &p);
	}
}
//-----------------------------------------------------------------------------------------
void AutomaticTresholdChart::Repaint()
{
	RECT r;
	GetClientRect(hWnd, &r);
	TSize l = {hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
	(*this)(l);
    InvalidateRect(hWnd, NULL, true);
}
//---------------------------------------------------------------------------------------------------------
AutomaticTresholdChart::Lines::Lines(Chart &chart) : LineSeries(chart), lines(NULL){}
//----------------------------------------------------------------------------------------------------------------
void AutomaticTresholdChart::Lines::Draw()
{
	if(NULL != lines)
	{
		for(std::list<Data>::const_iterator i = lines->cbegin(); i != lines->cend(); ++i)
		{
			color = i->color;
			SetData((double *)i->tresholds, dimention_of(i->tresholds));
			LineSeries::Draw();
		}
	}
}
//-------------------------------------------------------------------------------------------
AutomaticTresholdChart::FrameLine::FrameLine(Chart &chart) : LineSeries(chart)
{
	color = 0xff000000;
}
//----------------------------------------------------------------------------------------------------------------
void AutomaticTresholdChart::FrameLine::Draw()
{

		SetData((double *)corel.inputItem.elements, dimention_of(corel.inputItem.elements));
		LineSeries::Draw();
}

