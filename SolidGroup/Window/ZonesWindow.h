#pragma once
#include <windows.h>
#include "message.h"
#include "ZonesViewer.h"
//#include "CorrelationViewer.h"
#include "FrameViewer.h"
//#include "FFT.h"

class ZonesWindow
{
public:
	HWND hWnd;
	HWND hToolBar;
	ZonesWindow();
	void operator()(TCommand &);
	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
public:
	int zone;
private:
	void Open_();
	void MouseMoveHandler(unsigned);
public:
	FrameViewer frameViewer;
	static void Open();
	static ZonesWindow &Instance();
};

