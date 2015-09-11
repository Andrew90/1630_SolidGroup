#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
//#include "SignalData.h"
#include "Chart.hpp"
#include "typelist.hpp"
#include "Borders.h"

class TresholdViewer
{	
	struct CursorLabel
	{
		double (&data)[240000];
		ColorLabel label;
		TresholdViewer &owner;
		CursorLabel(TresholdViewer &, double (&)[240000]);
		bool Draw(TMouseMove &l, VGraphics &g);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double (&dataSignal)[240000];
	double (&dateReference)[240000];
	double coefficientA, coefficientB;
private:
	Cursor cursor;
	
	class Signal: public LineSeries{public:Signal(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	//class Reference: public LineSeries{public:Reference(Chart &c): LineSeries(c){color = 0xffff0000;}};
	/*
	template<int N> class Threshold: public HBorder
	{
	    public:
			Threshold(Chart &c)
			   :HBorder(c, Singleton<ThresholdsTable>::Instance().items.get<EParam<N>>().value)
		   {}
	};
	*/
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
    	, Signal
		//, Reference
		, Grid	
		/*
		, Threshold<1>
		, Threshold<2>
		, Threshold<3>
		, Threshold<4>
		, Threshold<5>
		, Threshold<6>
		, Threshold<7>
		, Threshold<8>
		*/
		//, VCursor1
		//, VCursor2
	>::Result> chart;
	CursorLabel cursorLabel;
	//const int &signalLength;
	//const int signalLength;
public:
	//int &currentOffset;
	int frameLength;
public:
	HWND hWnd;
	TresholdViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
};
