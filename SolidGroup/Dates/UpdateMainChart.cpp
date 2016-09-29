#include "stdafx.h"
#include "UpdateMainChart.h"
#include "MainWindow.h"
#include "SolidData.h"
#include "ChartData.h"
#include "EmptyWindow.h"

#include "DebugMess.h"

DWORD WINAPI Updata(LPVOID)
{
	MainWindow *o = (MainWindow *)GetWindowLongPtr(UpdateMainChart::hWnd, GWLP_USERDATA);
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
	return 0;
}

HWND UpdateMainChart::hWnd;

void UpdateMainChart::Update(int delay)
{
	static unsigned lastTime = 0;
	unsigned t = GetTickCount();
	if(lastTime + delay < t)
	{
		lastTime = t;
		QueueUserWorkItem(Updata, NULL, WT_EXECUTEDEFAULT);
	}
}