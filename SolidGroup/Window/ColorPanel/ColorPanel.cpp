#include "stdafx.h"
#include "ColorPanel.h"
#include "typelist.hpp"
#include "EmptyWindow.h"

using namespace Gdiplus;

ColorPanel::ColorPanel() 
	: backScreen(NULL)
	, backColor(0xffaaaaaa)
{
	buf[0] = 0;
}

void ColorPanel::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED)return;// || l.Height < 100) return;	
	
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
	g.FillRectangle(&SolidBrush(Color(backColor)), 0, 0, l.Width, l.Height);   
	if(0 != buf[0])
	{
		int len = wcslen(buf);
		PointF origin((REAL)0, (REAL)0);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);

		SolidBrush brushFontColor(-1);
		Font font(L"Arial", (REAL)48, FontStyleBoldItalic);
		RectF rect(0, 0, l.Width, l.Height);
		g.MeasureString(buf, len, &font, origin, &format, &rect);  

		origin.Y += rect.GetTop() + l.Height / 2;
		origin.X += rect.GetLeft() + l.Width / 2;
		g.DrawString(buf, len, &font, origin, &brushFontColor);
	}
}
//--------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ColorPanel::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}
//--------------------------------------------------------------------------------
void ColorPanel::SetText(wchar_t *txt, unsigned color)
{
	wcsncpy(buf, txt, dimention_of(buf));
	backColor = color;
	RepaintWindow(hWnd);
}
//---------------------------------------------
void ColorPanel::Clear()
{
	buf[0] = 0;
	backColor = 0xffaaaaaa;
	RepaintWindow(hWnd);
}