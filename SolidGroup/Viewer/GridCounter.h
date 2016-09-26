#pragma once
#include "GridItems.h"
#include <string>
#include <map>
#include "Grid.h"
#include "SolidCounterData.h"
class GridCounterViewer : public GridHandlers
{
public:
	SolidCounterData &row;
public:
	GridNotify grid;
	GridCounterViewer();
public:	
	void Create(HWND);
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);

	void Update();
};
/*
В родительском окне добавить обработчик TNotify
*/