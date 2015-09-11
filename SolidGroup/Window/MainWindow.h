#pragma once
#include "SelectTypeSize.h"
#include <windows.h>
#include "message.h"
#include "SignalViewer.h"
#include "TopLabelViewer.h"
class MainWindow
{
public:
	SelectTypeSize select;
	bool sizingWindow;
	HWND hCommunicationCheckBox;
	HWND hStoredCheckBox;
	HWND hPaintCheckBox;
public:
	HWND hWnd;
	HWND hStatusWindow;
	HWND hToolBar;	
	bool options;
	SignalViewer signalViewer;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	//void operator()(TMouseWell &);
	void operator()(TSizing &);
	void DisableMemuItemFile();
	void EnableMemuItemFile();
	void BottomLabel(int, wchar_t *);
	void operator()(TMessage &);
	static void CommunicationRemoveUnitBtn(TCommand &);
	static void PaintCheckBoxBtn(TCommand &m);
};
extern MainWindow mainWindow;
