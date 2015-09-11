#include "stdafx.h"
#include "UpdateMainChart.h"
#include "MainWindow.h"
#include "SolidData.h"
#include "ChartData.h"
#include "EmptyWindow.h"

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

HWND UpdateMainChart::hWnd;
void UpdateMainChart::Update()
{
	if(hWnd)
	{
		MainWindow *o = (MainWindow *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(o)
		{
			int length = o->signalViewer.chart.rect.right 
				- o->signalViewer.chart.rect.left 
				- o->signalViewer.chart.offsetAxesLeft 
				- o->signalViewer.chart.offsetAxesRight;
			if(solidData.currentOffset < length)
			{
				l502Signal.Set(solidData.dataBuffer, solidData.currentOffset, solidData.currentOffset);
				l502Reference.Set(solidData.referenceBuffer, solidData.currentOffset, solidData.currentOffset);
				o->signalViewer.chart.maxAxesX = solidData.currentOffset;
			}
			else
			{
				l502Signal.Set(solidData.dataBuffer, solidData.currentOffset, length);
				l502Reference.Set(solidData.referenceBuffer, solidData.currentOffset, length);
				o->signalViewer.chart.maxAxesX = solidData.currentOffset;
			}
			RepaintWindow(o->signalViewer.hWnd);
		}
	}
}