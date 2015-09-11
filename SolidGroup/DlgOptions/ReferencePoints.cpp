#include "stdafx.h"
#include "Dialogs.h"

#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Messages.h"
#include "EmptyWindow.h"
/*
struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			if(__ok_table_btn__<
				Owner::Table
				, typename TL::SubListFromMultyList<ParametersBase::multy_type_list, Owner::Table>::Result
			>()(h, owner))
			{
				EndDialog(h, TRUE);
			}
		}
	}
};
*/
PARAM_TITLE(EParam<1>, L"Порог 1")
PARAM_TITLE(EParam<2>, L"Порог 2")
PARAM_TITLE(EParam<3>, L"Порог 3")
PARAM_TITLE(EParam<4>, L"Порог 4")
PARAM_TITLE(EParam<5>, L"Порог 5")
PARAM_TITLE(EParam<6>, L"Порог 6")
PARAM_TITLE(EParam<7>, L"Порог 7")
PARAM_TITLE(EParam<8>, L"Порог 8")
//PARAM_TITLE(CorrelationTreshold, L"Порог корреляции")

DO_NOT_CHECK(EParam<1>)
DO_NOT_CHECK(EParam<2>)
DO_NOT_CHECK(EParam<3>)
DO_NOT_CHECK(EParam<4>)
DO_NOT_CHECK(EParam<5>)
DO_NOT_CHECK(EParam<6>)
DO_NOT_CHECK(EParam<7>)
DO_NOT_CHECK(EParam<8>)

//MAX_VALUE(CorrelationTreshold, 1.0)
//MIN_VALUE(CorrelationTreshold, 0.9)
namespace ReferencePoints
{
	struct __OkBtn : OkBtn
	{
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
#pragma message("сделать проверку param1 < param2 ... ")
			OkBtn::BtnHandler(owner, h);
		}
	};
}

void ReferencePointsDlg::Do(HWND h)
{
	if(TemplDialog<
		ThresholdsTable
		, TL::MkTlst<ReferencePoints::__OkBtn, CancelBtn>::Result
	    >(Singleton<ThresholdsTable>::Instance()).Do(h, L"Пороги опорного сигнала")
		)
	{
		RepaintWindow(h);
	}
}