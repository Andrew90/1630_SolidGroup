#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "Chart.hpp"
#include "typelist.hpp"
#include "Borders.h"

class MiddleViewer
{	
	struct CursorLabel
	{
		double (&data)[30000];
		ColorLabel label;
		MiddleViewer &owner;
		CursorLabel(MiddleViewer &, double (&)[30000]);
		bool Draw(TMouseMove &l, VGraphics &g);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double (&dataSignal)[30000];
	double (&dateReference)[30000];
	//double coefficientA, coefficientB;
private:
	Cursor cursor;
	
//	class Signal: public LineSeries{public:Signal(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	//class Reference: public LineSeries{public:Reference(Chart &c): LineSeries(c){color = 0xffff0000;}};
public:
	template<int N> class MiddleSignal: public LineSeries
	{
	    public:
			//double data[100];
			MiddleSignal(Chart &c) :LineSeries(c)
		   {
			//   SetData(solidData.referenceNULL[N], solidData.currentOffsetNull);
		   }
	};
	typedef TL::MkTlst<
		LeftAxes
		, BottomAxes 
    //	, Signal
	//	, Reference
		, Grid	
		, MiddleSignal<1>
		, MiddleSignal<2>
		, MiddleSignal<3>
		, MiddleSignal<4>
		, MiddleSignal<5>
		, MiddleSignal<6>
		, MiddleSignal<7>
		, MiddleSignal<8>
	>::Result items_list;
	ChartDraw<Chart, items_list> chart;
	CursorLabel cursorLabel;
public:
	int &currentOffset;
	int frameLength;
public:
	HWND hWnd;
	MiddleViewer(int &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
};
