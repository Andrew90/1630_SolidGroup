#pragma once
#include <windows.h>
#include "message.h"
#include "AutomaticThresholdPanel.h"
#include "AutomaticTresholdChart.h"
#include "AutomaticOptionsTresholds.h"
#include "AutomaticChart.h"
#include "SetRemoveTresholds.h"

class AutomaticThresholdsWindow
{
public:
	HWND hWnd;
	HWND hToolBar;
	AutomaticOptionsTresholds automaticOptionsTresholds;
	AutomaticThresholdPanel panel;
	AutomaticTresholdChart chart;
	AutomaticChart  automaticChart;
	SetRemoveTresholds setRemoveTresholds;
	CallMetodTemplate<AutomaticOptionsTresholds> automaticOptionsTresholdsTemplate;
	CallMetodTemplate<AutomaticTresholdChart> automaticTresholdChartTemplate;
	CallMetodTemplate<SetRemoveTresholds> setRemoveTresholdsTemplate;
	AutomaticThresholdsWindow();
	void operator()(TCommand &);
	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
public:
	static AutomaticThresholdsWindow &Instance();
	void Open();
};

