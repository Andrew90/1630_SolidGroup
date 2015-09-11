#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "TopLabelViewer.h"
#include "AutomaticTresholdsCommon.h"
#include <list>

class AutomaticTresholdChart
{		
	const double &minY;
	const double &maxY;
public:
	Gdiplus::Bitmap *backScreen;
private:
public:
	class Lines: public LineSeries
	{
	public:
		std::list<Data> *lines;
		Lines(Chart &chart);
		void Draw();
	};
	class FrameLine: public LineSeries
	{
	public:
		FrameLine(Chart &chart);
		void Draw();
	};
	ColorLabel label;
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesInt 
		, Lines
		, FrameLine
		, Grid		
	>::Result> chart;
public:
	HWND hWnd;
	HWND hToolBar;
	AutomaticTresholdChart();
	void operator()(TSize &);
	void operator()(TPaint &);
	void Repaint();	
};
