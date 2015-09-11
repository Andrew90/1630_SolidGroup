#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Emptywindow.h"
#include <typeinfo>

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
/*
PARAM_TITLE(PacketSize, L"Размер пакета данных")
PARAM_TITLE(Gain0, L"Усиление канала данных")
PARAM_TITLE(SyncLevel, L"Уровень синхронизации")
PARAM_TITLE(SyncGain, L"Усиление канала синхронизации")
PARAM_TITLE(StartDelay, L"Задержка при старте")
//---------------------------------------------------------------
PARAM_TITLE(Frequency, L"Частота сбора данных(МГц)")
PARAM_TITLE(NumberPackets, L"Количество пакетов данных")

PARAM_TITLE(SyncInput, L"Вход синхронизации открыт") 
PARAM_TITLE(MeasurementInput, L"Измерительный вход открыт")    
PARAM_TITLE(SynchronizationEdge, L"Синхронизация по фронту") 
//.............................................................
MIN_VALUE(SyncLevel, 1)
MAX_VALUE(SyncLevel, 255)

MIN_VALUE(StartDelay, -1)
MAX_VALUE(StartDelay, 999)

DO_NOT_CHECK(PacketSize)
DO_NOT_CHECK(Gain0)
DO_NOT_CHECK(SyncGain)
//---------------------------------------------------------------------
DO_NOT_CHECK(Frequency)

MIN_VALUE(NumberPackets, 0)
MAX_VALUE(NumberPackets, 1000000 / 986 / count_sensors)
//..................................................................
template<>struct DlgSubItems<PacketSize, int>: ComboBoxSubItem<PacketSize>{};
const int PacketSizeData[] = {474, 986};
template<>struct FillComboboxList<PacketSize>
{
	void operator()(HWND h, PacketSize &t)
	{
		wchar_t buf[32];
		for(int i = 0; i < dimention_of(PacketSizeData); ++i)
		{
			ComboBox_AddString(h, _itow(PacketSizeData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<PacketSize>
{
	void operator()(HWND h, PacketSize &t)
	{
		for(int i = 0; i < dimention_of(PacketSizeData); ++i)
		{
			if(PacketSizeData[i] == Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};
//..................................................................
template<>struct DlgSubItems<Gain0, int>: ComboBoxSubItem<Gain0>{};
const int Gain0Data[] = {1, 2, 5, 10, 20, 40, 100, 200};
template<>struct FillComboboxList<Gain0>
{
	void operator()(HWND h, Gain0 &t)
	{
		wchar_t buf[32];
		for(int i = 0; i < dimention_of(Gain0Data); ++i)
		{
			ComboBox_AddString(h, _itow(Gain0Data[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<Gain0>
{
	void operator()(HWND h, Gain0 &t)
	{
		for(int i = 0; i < dimention_of(Gain0Data); ++i)
		{
			if(Gain0Data[i] == Singleton<LanParametersTable>::Instance().items.get<Gain0>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};
//-------------------------------------------------------------------
template<>struct DlgSubItems<Frequency, int>: ComboBoxSubItem<Frequency>{};
const int FrequencyData[] = {25, 50};
template<>struct FillComboboxList<Frequency>
{
	void operator()(HWND h, Frequency &t)
	{
		wchar_t buf[32];
		for(int i = 0; i < dimention_of(FrequencyData); ++i)
		{
			ComboBox_AddString(h, _itow(FrequencyData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<Frequency>
{
	void operator()(HWND h, Frequency &t)
	{
		for(int i = 0; i < dimention_of(FrequencyData); ++i)
		{
			if(FrequencyData[i] == Singleton<LanParametersTable>::Instance().items.get<Frequency>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};
//..................................................................
template<>struct DlgSubItems<SyncGain, int>: ComboBoxSubItem<SyncGain>{};
const int SyncGainData[] = {1, 10};
template<>struct FillComboboxList<SyncGain>
{
	void operator()(HWND h, SyncGain &t)
	{
		wchar_t buf[32];
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, _itow(SyncGainData[i], buf, 10));
		}
	}
};
template<>struct CurrentValue<SyncGain>
{
	void operator()(HWND h, SyncGain &t)
	{
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			if(SyncGainData[i] == Singleton<LanParametersTable>::Instance().items.get<SyncGain>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};
//------------------------------------------------------------------
void LanParamDlg::Do(HWND h)
{
	if(TemplDialog<LanParametersTable>(Singleton<LanParametersTable>::Instance()).Do(h, L"Настройка аналоговой платы"))
	{
		Lan::SetParams();
	}
}
*/
//DEFINE_PARAM(locL502Signal, int, 0)
//DEFINE_PARAM(locL502ReferencePoint, int, 1)
/*
struct locL502Signal
{
	typedef Holder<16> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"locL502Signal";}
	locL502Signal()
	{
		default_value = L"10";
		value = default_value;		
	}
};

struct locL502ReferencePoint
{
	typedef Holder<16> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"locL502ReferencePoint";}
	locL502ReferencePoint()
	{
		default_value = L"10";
		value = default_value;		
	}
};
*/
PARAM_TITLE(L502Signal, L"Размер пакета данных")
PARAM_TITLE(L502ReferencePoint, L"Усиление канала данных")

DO_NOT_CHECK(L502Signal)
DO_NOT_CHECK(L502ReferencePoint)

template<>struct DlgSubItems<L502Signal, double >: ComboBoxSubItem<L502Signal>{};
template<>struct DlgSubItems<L502ReferencePoint, double >: ComboBoxSubItem<L502ReferencePoint>{};


const wchar_t *SyncGainData[] ={
	L"0.2", L"0.5", L"1", L"2", L"5", L"10"
};
const double SyncGainDataDouble[] ={
	0.2, 0.5, 1, 2, 5, 10
};
template<>struct FillComboboxList<L502Signal>
{
	void operator()(HWND h, L502Signal &t)
	{
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, SyncGainData[i]);
		}
	}
};

template<>struct CurrentValue<L502Signal>
{
	void operator()(HWND h, L502Signal &t)
	{
		for(int i = 0; i < dimention_of(SyncGainDataDouble); ++i)
		{
			if(SyncGainDataDouble[i] == Singleton<L502ParametersTable>::Instance().items.get<L502Signal>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};

template<>struct FillComboboxList<L502ReferencePoint>
{
	void operator()(HWND h, L502ReferencePoint &t)
	{
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, SyncGainData[i]);
		}
	}
};

template<>struct CurrentValue<L502ReferencePoint>
{
	void operator()(HWND h, L502ReferencePoint &t)
	{
		for(int i = 0; i < dimention_of(SyncGainDataDouble); ++i)
		{
			if(SyncGainDataDouble[i] == Singleton<L502ParametersTable>::Instance().items.get<L502ReferencePoint>().value)
		  {
			  ComboBox_SetCurSel(h, i);
			  break;
		  }
		}
	}
};

template<int N> struct Wchar_to<Holder<N> >
{
	wchar_t *operator()(wchar_t *data)
	{
		return data;
	}
};


/*
struct CancelBtn502
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
	    EndDialog(h, FALSE);
	}
};
*/
//Singleton<L502ParametersTable>::Instance()
/*
struct locL502ParametersTable
{
	typedef TL::MkTlst<	
		locL502Signal
		, locL502ReferencePoint
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"locL502ParametersTable";}
};
*/
/*
void L502ParamDlg::Do(HWND h)
{
	locL502ParametersTable l502;
	if(TemplDialog<locL502ParametersTable, TL::MkTlst<OkBtn502, CancelBtn>::Result>(l502).Do(h, L"Настройка аналоговой платы"))
	{
		//Lan::SetParams();
	}
}
*/
void L502ParamDlg::Do(HWND h)
{
	/*
	if(TemplDialog<L502ParametersTable>(Singleton<L502ParametersTable>::Instance()).Do(h, L"Настройка аналоговой платы"))
	{
		//Lan::SetParams();//
#pragma message("дописать настройку аналоговой платы")
	}
	*/
}