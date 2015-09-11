#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"

//void InitToolBar();

void SycleMeashurement(HWND h);//обработчик для кнопки "Циклическое измерение"

void StopMeashurement(HWND h);//обработчик для кнопки "Остановка измерения"

void TresholdsViewBtn(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Цикл")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Настр.пор.")
  
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  , ButtonToolbar<IDB_OptionsBtn, TresholdsViewBtn, ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
>::Result tool_button_list;

void Disable3Button(HWND );
void Enanle3Button(HWND );

void DisableAllButton();



