#include "stdafx.h"
#include "AutomaticDlg.h"
#include "AppBase.h"
#include "ParamDlg.h"

PARAM_TITLE(OffsetAxesX, L"Смещение первичного сигнала")
PARAM_TITLE(FrameWidth, L"Ширина кадра")
PARAM_TITLE(MaxAxesY  , L"Максимальная координата Y")
PARAM_TITLE(MinAxesY  , L"Минимальная координата Y")

MIN_VALUE(OffsetAxesX, 1)
MAX_VALUE(OffsetAxesX, 10000)

MIN_VALUE(FrameWidth, 20)
MAX_VALUE(FrameWidth, 50000)

MIN_EQUAL_VALUE(MaxAxesY, -10)
MAX_EQUAL_VALUE(MaxAxesY, 10)

MIN_EQUAL_VALUE(MinAxesY, -10)
MAX_EQUAL_VALUE(MinAxesY, 10)

void SignalOffset::Do(HWND h)
{
	if(TemplDialog<GraphicSignalOptionsTable>(Singleton<GraphicSignalOptionsTable>::Instance()).Do(h, L"Первичный сигнал"))
	{
	}
}

void PointsOptionsTableDlg::Do(HWND h)
{
	if(TemplDialog<PointsOptionsTable>(Singleton<PointsOptionsTable>::Instance()).Do(h, L"Точки смещения"))
	{
	}
};
