#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
//#include "SignalData.h"
#include "Chart.hpp"
#include "typelist.hpp"
#include "Borders.h"

class SignalViewer
{	
	struct CursorLabel
	{
		double (&data)[1024];
		ColorLabel label;
		SignalViewer &owner;
		CursorLabel(SignalViewer &, double (&)[1024]);
		bool Draw(TMouseMove &l, VGraphics &g);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double (&dataMin)[1024];
	double (&dataMax)[1024];
	double (&referenceMin)[1024];
	double (&referenceMax)[1024];
private:
	Cursor cursor;
	///*
	class VCursor1 : public VBorder
	{
	public:
		VCursor1(Chart &c):VBorder(c){color = 0xffffff00;}
	};
	class VCursor2 : public VBorder
	{
	public:
		VCursor2(Chart &c):VBorder(c){color = 0xffffff00;}
	};
	//*/
	class SignalMin: public LineSeries{public:SignalMin(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	class SignalMax: public LineSeries{public:SignalMax(Chart &c): LineSeries(c){color = 0xff0000aa;}};
	class ReferenceMin: public LineSeries{public:ReferenceMin(Chart &c): LineSeries(c){color = 0xffff0000;}};
	class ReferenceMax: public LineSeries{public:ReferenceMax(Chart &c): LineSeries(c){color = 0xffaa0000;}};
	/*
	template<int N> class Threshold: public HBorder
	{
	    public:
			Threshold(Chart &c)
			   :HBorder(c, Singleton<ThresholdsTable>::Instance().items.get<EParam<N>>().value)
		   {}
	};
	*/
public:
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, SignalMin
		, SignalMax
		, ReferenceMin
		, ReferenceMax
		, Grid	
		, VCursor1
		, VCursor2
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
	>::Result> chart;
	CursorLabel cursorLabel;
	const int &signalLength;
public:
	HWND hWnd;
	SignalViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
};
