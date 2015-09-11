#include "stdafx.h"
#include "AutomaticChart.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "LabelMessage.h" 
#include "AppBase.h"
#include "SolidData.h"
#include "Corel.h"
#include "AutomaticThresholdsWindow.h"
#include "Compute.h"

//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

AutomaticChart::AutomaticChart()
	: backScreen(NULL)
	, chart(backScreen)
	, offset(0)
	, width(Singleton<GraphicSignalOptionsTable>::Instance().items.get<FrameWidth>().value)
	, offsetAxesX(Singleton<GraphicSignalOptionsTable>::Instance().items.get<OffsetAxesX>().value)
	, minY(Singleton<GraphicSignalOptionsTable>::Instance().items.get<MinAxesY>().value)
	, maxY(Singleton<GraphicSignalOptionsTable>::Instance().items.get<MaxAxesY>().value)
{
	chart.rect.left = 10;
	chart.rect.top = 20;
	chart.offsetAxesBottom = 10;
	
	label.fontHeight = 15;
	label = L"";
	chart.items.get<VThreshold>().owner = this;
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void AutomaticChart::operator()(TSize &l)
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

	FrameBorder();

	chart.items.get<Signal>().SetData(&solidData.dataBuffer[offset], width);
	chart.items.get<Reference>().SetData(&solidData.referenceBuffer[offset], width);

	chart.minAxesX = offset;
	chart.maxAxesX = offset + width - 1;
	chart.minAxesY = minY;
	chart.maxAxesY = maxY;

	chart.Draw(g);
	label.Draw(g);
}
//----------------------------------------------------------------------------------------------------
void AutomaticChart::operator()(TPaint &l)
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
void AutomaticChart::Repaint()
{
	RECT r;
	GetClientRect(hWnd, &r);
	TSize l = {hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
	(*this)(l);
    InvalidateRect(hWnd, NULL, true);
}
//------------------------------------------------------------------------------------------------------------------
void AutomaticChart::LeftOffset()
{
	if(solidData.currentOffset > offset + width)
	offset += offsetAxesX;
	Repaint();
}
//----------------------------------------------------------------------------------------------------
void AutomaticChart::RightOffset()
{
	offset -= offsetAxesX;
	if(offset < 0) offset = 0;
	Repaint();
}
//---------------------------------------------------------------------------------------------------
#if 0
void AutomaticChart::FrameBorder()
{
	int z = 0;
	int i = offset;
	int (&offsets)[8] = AutomaticThresholdsWindow::Instance().automaticOptionsTresholds.tresholds;
	bool b = false;
	for(; z < width && solidData.referenceBuffer[i] > 0; ++z, ++i) b = true;
	if(b)
	{
	//for(; z < width; ++z)
//	{
	//	if(solidData.referenceBuffer[i] < 0)
	//	{
			start = i;
		//	break;
	//	}
	//	++i;
	//}
	for(; z < width; ++z)
	{
		if(solidData.referenceBuffer[i] > 0)
		{
			stop = i;
			int length = stop - start;
			wchar_t *s = label.buffer;
			wsprintf(s, L"<ff>");
			for(int i = 0; i < dimention_of(corel.inputItem.elements); ++i)
			{
				int x = start + int((double)length * offsets[i] / 100.0);
				corel.inputItem.elements[i] = solidData.dataBuffer[x];
				points[i] = x;
				s = &s[wcslen(s)];
				wsprintf(s, L"%s  ", Wchar_from<double>(corel.inputItem.elements[i])());
			}

			corel.Compute();
			AutomaticThresholdsWindow::Instance().chart.Repaint();
			break;
		}
		++i;
	}
	}
}
#else
struct DataBufferXX: Compute::Data
{
public:
	double dataBuffer(int i)
	{
		return solidData.dataBuffer[i];
	}
	double referenceBuffer(int i)
	{
		return solidData.referenceBuffer[i];
	}
};
void AutomaticChart::FrameBorder()
{
	bool b = false;
	int z = 0;
	int i = offset;
	for(; z < width && solidData.referenceBuffer[i] > 0; ++z, ++i) b = true;
	int start = i;
	if(!b) return;
	b = false;
	for(; z < width && solidData.referenceBuffer[i] < 0; ++z, ++i) b = true;
	int stop = i;
	if(b)
	{
	int (&offsets)[8] = AutomaticThresholdsWindow::Instance().automaticOptionsTresholds.tresholds;
	double inputs[1024] = {};
	int length;
	DataBufferXX dataBuff;

	if(compute.SubCompute(
		offsets
		, start - 10
		, stop + 10
		, dataBuff
		, inputs
		, length
		))
	{
	corel.Compute();
	wchar_t *s = label.buffer;
	wsprintf(s, L"<ff>");
	for(int i = 0; i < dimention_of(corel.inputItem.elements); ++i)
	{
		int x = start + int((double)length * offsets[i] / 100.0);
		corel.inputItem.elements[i] = solidData.dataBuffer[x];
		points[i] = x;
		s = &s[wcslen(s)];
		wsprintf(s, L"%s  ", Wchar_from<double>(corel.inputItem.elements[i])());
	}
	}
	else
	{
		wsprintf(label.buffer, L"<ff0000>������ ��������� �� ����������");
		corel.inputItem.classTube = 0;
	}
	AutomaticThresholdsWindow::Instance().chart.Repaint();
	}
}
#endif

void AutomaticChart::VThreshold::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );
	for(int i = 0; i < dimention_of(owner->points); ++i)
	{
		value = owner->points[i];
		VBorder::Draw();
	}
	chart.g->SetClip(&Region());
}
	
	