#include "stdafx.h"
#include "AppBase.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "L502SolidGroup.h"

namespace
{
    PARAM_TITLE(InputSignal, L"напряжение")
	PARAM_TITLE(ReferenceSignal, L"ток")

	DO_NOT_CHECK(InputSignal)
	DO_NOT_CHECK(ReferenceSignal)

	template<>struct DlgSubItems<InputSignal, int >: UpDownSubItem<InputSignal>{};
    template<>struct DlgSubItems<ReferenceSignal, int >: UpDownSubItem<ReferenceSignal>{};
}
void AdcInputPortsDlg::Do(HWND h)
{
	if(TemplDialog<
		ADCInputsParametersTable
	>(Singleton<ADCInputsParametersTable>::Instance()).Do(h, L"Настройка номеров входов аналоговой платы")
	)
	{
		if(Singleton<L502SolidGroup>::Instance().SetupParams())
		{			
			MessageBox(0, L"Не могу инициировать аналоговую плату", L"Ошибка!!!", MB_ICONERROR);
		}
	}
}