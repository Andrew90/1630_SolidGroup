#pragma once
#include "GridItems.h"
class SolidGridGridViewer : public GridHandlers
{
public:	
	HWND hEdits[2];
	void Create(HWND);
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	void LClick(LPNMITEMACTIVATE );
};

