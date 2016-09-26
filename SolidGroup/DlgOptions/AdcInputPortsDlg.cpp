#include "stdafx.h"
#include "AppBase.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "L502SolidGroup.h"

namespace
{
    PARAM_TITLE(InputSignal, L"����������")
	PARAM_TITLE(ReferenceSignal, L"���")

	DO_NOT_CHECK(InputSignal)
	DO_NOT_CHECK(ReferenceSignal)

	template<>struct DlgSubItems<InputSignal, int >: UpDownSubItem<InputSignal>{};
    template<>struct DlgSubItems<ReferenceSignal, int >: UpDownSubItem<ReferenceSignal>{};
}
void AdcInputPortsDlg::Do(HWND h)
{
	if(TemplDialog<
		ADCInputsParametersTable
	>(Singleton<ADCInputsParametersTable>::Instance()).Do(h, L"��������� ������� ������ ���������� �����")
	)
	{
		if(Singleton<L502SolidGroup>::Instance().SetupParams())
		{			
			MessageBox(0, L"�� ���� ������������ ���������� �����", L"������!!!", MB_ICONERROR);
		}
	}
}