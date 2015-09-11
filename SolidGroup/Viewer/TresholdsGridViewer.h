#pragma once
#include "GridItems.h"
class TresholdeGridWiewer : public GridHandlers
{
public:	
	TresholdeGridWiewer();

	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	void LClick(LPNMITEMACTIVATE );
	static void RemoveRow(HWND);

};

