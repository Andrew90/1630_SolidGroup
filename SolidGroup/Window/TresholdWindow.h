#pragma once

#include <windows.h>
#include "message.h"
#include "TresholdViewer.h"
//#include "SelectTypeSize.h"
//#include "TopLabelViewer.h"
#include "TresholdsGridViewer.h"
#include "TresholdPanelViewer.h"
#include "Grid.h"
class TresholdWindow
{
	void Open_();
public:
	HWND hWnd;
	//TresholdViewer tresholdViewer;
	GridNotify grid;
	TresholdeGridWiewer gridViewer;
	TresholdPanelViewer tresholdPanelViewer;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMouseWell &);
	void operator()(TSizing &);
	unsigned operator()(TNotify &);
	static void Open();
    void UpdateTresholdGrid();
    static TresholdWindow &Instance();
};