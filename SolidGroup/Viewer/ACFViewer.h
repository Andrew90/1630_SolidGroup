#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "Borders.h"

class ACFViewer
{	
	struct CursorLabel
	{
		ACFViewer &owner;
		double (&data)[1024];
		ColorLabel label;
		Chart &chart;
		CursorLabel(Chart &, ACFViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		//bool GetColorBar(unsigned zone, double &data_, unsigned &color);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double data[1024];
private:
	Cursor cursor;
public:
	class LeftBorder : public VBorder{public:LeftBorder(Chart &c):VBorder(c){}};
	class RightBorder : public VBorder{public:RightBorder(Chart &c):VBorder(c){}};
//	class PeakBorder : public HBorder{public:PeakBorder(Chart &c):HBorder(c){}};
//	class MinEnergyBorder : public HBorder{public:MinEnergyBorder(Chart &c):HBorder(c){color = 0xffff0000;}};
	class VCursor : public VBorder{public:VCursor(Chart &c):VBorder(c){color = 0xff0000ff;}};
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, LineSeries
		, Grid	
		, LeftBorder
		, RightBorder
	//	, PeakBorder
	//	, MinEnergyBorder
	//	, VCursor
	>::Result> chart;
private:
	CursorLabel cursorLabel;
public:
  //  int correlationLength;
public:
	HWND hWnd;
	double coefficientA, coefficientB, peak, minEnergy, thickness;
	ACFViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
};
