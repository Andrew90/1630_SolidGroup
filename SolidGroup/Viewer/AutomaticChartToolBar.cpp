#include "stdafx.h"
#include "AutomaticChartToolBar.h"
#include "AutomaticChart.h"
#include "AutomaticThresholdsWindow.h"
#include "TresholdWindow.h"
void AutomaticChartToolBar::LeftArrowHandler(HWND h)
{
	AutomaticThresholdsWindow::Instance().automaticChart.RightOffset();
	TresholdWindow::Instance().UpdateTresholdGrid();
}

void AutomaticChartToolBar::RightArrowHandler(HWND h)
{
	AutomaticThresholdsWindow::Instance().automaticChart.LeftOffset();
	TresholdWindow::Instance().UpdateTresholdGrid();
}