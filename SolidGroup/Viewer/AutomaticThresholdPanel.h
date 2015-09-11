#pragma once
#include "message.h"
#include "AutomaticTresholdsCommon.h"

class AutomaticThresholdPanel
{	
	HWND arrHWND[8];
	HWND hUpdeteBtn;
	HWND hStoreParamBtn;
public:
	CallMetod *callAutomatic;
	CallMetod *callRepaintChart;
	CallMetod *callStoreParam;
	HWND hWnd;
	int (&tresholds)[8];
	AutomaticThresholdPanel(int (&)[8]);
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TCommand &);
	static void UpdeteBtn(TCommand &);
	static void StoreParamBtn(TCommand &);
};