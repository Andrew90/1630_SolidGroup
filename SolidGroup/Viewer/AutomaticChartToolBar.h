#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"
//#include "menuapi.h"

class AutomaticChartToolBar
{
public:
	static void LeftArrowHandler(HWND h);
	static void RightArrowHandler(HWND h);

	typedef TL::MkTlst<
		SeparatorToolbar<0>
		, ButtonToolbar<IDB_LeftArrow, LeftArrowHandler>
		, ButtonToolbar<IDB_RightArrow, RightArrowHandler>
		, SeparatorToolbar<1>
	>::Result tool_button_list;
};



