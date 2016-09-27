#include "stdafx.h"
#include "AppBase.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "L502SolidGroup.h"

namespace
{
	PARAM_TITLE(SyncroData, L"Синхронизация")

	template<>struct DlgSubItems<SyncroData, bool >: ComboBoxSubItem<SyncroData>{};

	const wchar_t *syncData[] ={
		L"По току", L"По напряжению"
	};
	const bool syncGainDataBool[] ={
		false, true
	};

	template<>struct FillComboboxList<SyncroData>
	{
		void operator()(HWND h, SyncroData &t)
		{
			for(int i = 0; i < dimention_of(syncData); ++i)
			{
				ComboBox_AddString(h, syncData[i]);
			}
		}
	};

	template<>struct CurrentValue<SyncroData>
	{
		void operator()(HWND h, SyncroData &t)
		{
			bool b = Singleton<SyncroDataTable>::Instance().items.get<SyncroData>().value;
			for(int i = 0; i < dimention_of(syncGainDataBool); ++i)
			{
				if(syncGainDataBool[i] == b)
				{
					ComboBox_SetCurSel(h, i);
					break;
				}
			}
		}
	};
	template<class P>struct __ok_btn__<DlgItem<SyncroData>, P>
	{
		typedef DlgItem<SyncroData> O;
		void operator()(O *o, P *p)
		{
			wchar_t buf[64];
			GetWindowText(o->hWnd, buf, dimention_of(buf));
			for(int i = 0; i < dimention_of(syncData); ++i)
			{
				if(0 == wcscmp(syncData[i], buf))
				{
					o->value.value = syncGainDataBool[i];
					break;
				}
			}
			p->update.set<typename TL::Inner<O>::Result>(o->value.value);
		}
	};
}
void SyncroDlg::Do(HWND h)
{
	if(TemplDialog<
		SyncroDataTable
	>(Singleton<SyncroDataTable>::Instance()).Do(h, L"Синхронизация")
	)
	{}
}