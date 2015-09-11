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
	wchar_t *Title(){return L"���������";}
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
PARAM_TITLE(EParam<1>, L"����� 1")
PARAM_TITLE(EParam<2>, L"����� 2")
PARAM_TITLE(EParam<3>, L"����� 3")
PARAM_TITLE(EParam<4>, L"����� 4")
PARAM_TITLE(EParam<5>, L"����� 5")
PARAM_TITLE(EParam<6>, L"����� 6")
PARAM_TITLE(EParam<7>, L"����� 7")
PARAM_TITLE(EParam<8>, L"����� 8")
//PARAM_TITLE(CorrelationTreshold, L"����� ����������")

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
#pragma message("������� �������� param1 < param2 ... ")
			OkBtn::BtnHandler(owner, h);
		}
	};
}

void ReferencePointsDlg::Do(HWND h)
{
	if(TemplDialog<
		ThresholdsTable
		, TL::MkTlst<ReferencePoints::__OkBtn, CancelBtn>::Result
	    >(Singleton<ThresholdsTable>::Instance()).Do(h, L"������ �������� �������")
		)
	{
		RepaintWindow(h);
	}
}