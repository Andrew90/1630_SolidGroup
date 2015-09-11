#pragma once
#include <windows.h>
#include "message.h"
//#include "ZonesViewer.h"
//#include "CorrelationViewer.h"
#include "MiddleViewer.h"
//#include "FFT.h"

class MiddleWindow
{
public:
	HWND hWnd;
	HWND hToolBar;
	MiddleWindow();
	void operator()(TCommand &);
	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
private:
	//CFFT fft;
public:
	int zone;//, offset;
private:
//	int &signalLength;
	void Open_();
	void MouseMoveHandler(unsigned);
public:
	MiddleViewer middleViewer;
//	SignalViewer signalViewer;
//	CorrelationViewer correlationViewer;
	static void Open();
	static MiddleWindow &Instance();
};

