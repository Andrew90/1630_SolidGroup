#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"

//void InitToolBar();

void SycleMeashurement(HWND h);//���������� ��� ������ "����������� ���������"

void StopMeashurement(HWND h);//���������� ��� ������ "��������� ���������"

void TresholdsViewBtn(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"����")
TOOL_LIP_TEXT(ToolLipReset  	  , L"����")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"�����.���.")
  
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



