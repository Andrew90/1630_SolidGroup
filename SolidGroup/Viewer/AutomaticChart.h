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
#include "MenuApi.h"
#include "AutomaticDlg.h"
#include "AutomaticChartMenu.hpp"
#include "Borders.h"

class AutomaticChart
{	
	int offset;
	const int &offsetAxesX;
	const int &width;
	const double &minY;
	const double &maxY;
	int start;
	int stop;
	int points[8];
public:
	Gdiplus::Bitmap *backScreen;
private:
public:
	ColorLabel label;
	class Signal: public LineSeries{public:Signal(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	class Reference: public LineSeries{public:Reference(Chart &c): LineSeries(c){color = 0xffff0000;}};
	class VThreshold : public VBorder
	{
	    public:
			AutomaticChart *owner;
			VThreshold(Chart &c) : VBorder(c){thinck = 1;}
			void Draw();
	};
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesInt 
		, Signal
		, Reference
		, Grid	
		, VThreshold
	>::Result> chart;
public:
	HWND hWnd;
	AutomaticChart();
	void operator()(TSize &);
	void operator()(TPaint &);
	void Repaint();
	void LeftOffset();
	void RightOffset();
	void FrameBorder();
};
