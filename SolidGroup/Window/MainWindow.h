#pragma once
#include "SelectTypeSize.h"
#include <windows.h>
#include "message.h"
#include "SignalViewer.h"
#include "TopLabelViewer.h"
#include "GridCounter.h"
#include "ColorPanel/ColorPanel.h"

class MainWindow
{
public:
	SelectTypeSize select;
	bool sizingWindow;
	HWND hCommunicationCheckBox;
	HWND hStoredCheckBox;
	HWND hPaintCheckBox;
	bool created;
public:
	HWND hWnd;
	HWND hStatusWindow;
	HWND hToolBar;	
	bool options;
	SignalViewer signalViewer;
	GridCounterViewer gridCounter;
	ColorPanel colorPanel;
	MainWindow();
	void operator()(TSize &);
	unsigned operator()(TNotify &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSizing &);
	void DisableMemuItemFile();
	void EnableMemuItemFile();
	void BottomLabel(int, wchar_t *);
	void operator()(TMessage &);
	static void CommunicationRemoveUnitBtn(TCommand &);
	static void PaintCheckBoxBtn(TCommand &m);
	void CycleBtn();
	void StopBtn();
	void InitBtn();
};
extern MainWindow mainWindow;
