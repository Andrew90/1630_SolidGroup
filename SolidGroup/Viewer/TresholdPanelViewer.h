#pragma once
//#include <string>
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "Chart.hpp"
#include "typelist.hpp"
#include "Borders.h"
#include "Grid.h"
#include "SolidGridViewer.h"

class TresholdPanelViewer
{	
	Gdiplus::Bitmap *backScreen;
public:
	HWND hWnd;
	HWND comboBoxWnd;
	HWND removeUnitBtnWnd;
	HWND addUnitBtnWnd;
	HWND removeNameBtnWnd;
	HWND addNameBtnWnd;
	HWND renameGroup;
	HWND editRenameGroup;
	HWND hEditGroup;
	HWND hEditCommunicationID;
	HWND hResultMessageLabel;
	SolidGridGridViewer solidGridGridViewer;
	GridNotify grid;
	TresholdPanelViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TDestroy &);
	void operator()(TCommand &m);

	static void RemoveUnitBtn	 (TCommand &);
	static void AddUnitBtn	 (TCommand &);
	static void RemoveNameBtn(TCommand &);
	static void AddNameBtn   (TCommand &);
	//static void RenameNameBtn(TCommand &);
	unsigned operator()(TNotify &);
	void operator()(TRButtonDown &);
	void SetResultMessageLabel();
};
