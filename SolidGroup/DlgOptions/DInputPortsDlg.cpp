#include "stdafx.h"
#include "AppBase.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
//#include "SolidGroupUnit.h"
namespace
{
	PARAM_TITLE(ModuleOn, L"Модуль включен")
		PARAM_TITLE(TubeIn, L"Труба в модуле")

		DO_NOT_CHECK(ModuleOn)
		DO_NOT_CHECK(TubeIn)


		template<>struct DlgSubItems<ModuleOn, int >: UpDownSubItem<ModuleOn>{};
	template<>struct DlgSubItems<TubeIn, int >: UpDownSubItem<TubeIn>{};

	template<class O, class P>struct __unshift__
	{
		void operator()(O *o, P *p)
		{
			int value = 0;
			int shift = o->value;
			while(shift >>= 1)++value;
			p->get<O>().value = value;
		}
	};

	template<class O, class P>struct __cpy__
	{
		void operator()(O *o, P *p)
		{
			o->value = p->get<O>().value;
		}
	};

	template<class O, class P>struct __shift_ok_btn__
	{
		void operator()(O *o, P *p)
		{
			int shift = __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
			int value = 1;
			while(--shift >= 0) value <<= 1;
			o->value.value = value;
			p->update.set<typename TL::Inner<O>::Result>(o->value.value);
		}
	};

	template<class P>struct __ok_btn__<DlgItem<ModuleOn>, P>: __shift_ok_btn__<DlgItem<ModuleOn>, P>{};
	template<class P>struct __ok_btn__<DlgItem<TubeIn>, P>: __shift_ok_btn__<DlgItem<TubeIn>, P>{};

}

void DInputPortsDlg::Do(HWND h)
{
	DInputsParametersTable table;
	TL::foreach<DInputsParametersTable::items_list, __unshift__>()(
		&Singleton<DInputsParametersTable>::Instance().items, &table.items);
	if(TemplDialog<
		DInputsParametersTable
	>(table).Do(h, L"Настройка номеров входов дискретной платы")
	)
	{
		TL::foreach<DInputsParametersTable::items_list, __cpy__>()(&Singleton<DInputsParametersTable>::Instance().items
			, &table.items);
	}
}