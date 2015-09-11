#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"

void LeftArrowHandler(HWND h);
void RightArrowHandler(HWND h);
void UpArrowHandler(HWND h);
void DownArrowHandler(HWND h);

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_LeftArrow, LeftArrowHandler>
  , ButtonToolbar<IDB_RightArrow, RightArrowHandler>
  , SeparatorToolbar<1>
>::Result detailied_tool_button_list;


