#pragma once

namespace MainWindowMenu
{
struct MainFile{};
struct MainExit{};

struct MainOptionTypeSize{};
struct MainThresholdOption{};
struct MainCreateTypesize{};
struct MainDeleteTypeSize{};

struct MainOptionUnits{};
struct OptionsCoefficients{};
struct AdditionalParametersTypesize{};
struct InputsOutputs{};
struct MainAbout{};
/* test
struct LoadData{};
struct StoredData{};
struct Recalculation{};
struct MedianFiltreDlg{};
struct DeadAreaDlg{};
struct ACFBorderDlg{};
struct Descriptor1730Dlg {};
struct InputsDlg		 {};
struct OutputsDlg		 {};
*/

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<LoadDateFile>
		, MenuItem<SaveDateFile>
		, Separator<0>
		, MenuItem<Compute>
		, Separator<1>
		 , MenuItem<MainExit>
	>::Result list;
};

template<>struct SubMenu<AdditionalParametersTypesize>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<MedianFiltreDlg, DeadAreaDlg, ACFBorderDlg>::Result
		, MenuItem
	>::Result list;
};

template<>struct TopMenu<MainOptionTypeSize>
{
	typedef TL::MkTlst<	
	 MenuItem<TresholdWindow>
		, Separator<0>
		 , MenuItem<MainCreateTypesize>
		, MenuItem<MainDeleteTypeSize>
		, Separator<1>
		, MenuItem<StoredData>
	>::Result list;
};

template<>struct SubMenu<InputsOutputs>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<Descriptor1730Dlg, InputsDlg, OutputsDlg>::Result 
		, MenuItem
	>::Result list;
};
struct CommunicationOptions{};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<	
		MenuItem<CommunicationOptions>
		, MenuItem<SolenoidParametersTableDlg>
		, Separator<0>
		, MenuItem<CoordinatesDlg>
	>::Result list;
};



template<>struct TopMenu<MainAbout>
{
	typedef TL::MkTlst< 
		 MenuItem<MainAbout>
	>::Result list;
};
typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionTypeSize>
	, TopMenu<MainOptionUnits>
	, TopMenu<MainAbout>
>::Result MainMenu;

    MENU_TEXT(L"Файл", TopMenu<MainFile>)
	MENU_TEXT(L"Сохранить данные", MenuItem<SaveDateFile>)
	MENU_TEXT(L"Загрузить данные", MenuItem<LoadDateFile>)
	MENU_TEXT(L"Перерасчёт", MenuItem<Compute>)
	MENU_TEXT(L"Выход", MenuItem<MainExit>)

	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
    MENU_TEXT(L"Дополнительные параметры" , SubMenu<AdditionalParametersTypesize>)
	MENU_TEXT(L"Медианный фильтр" , MenuItem<MedianFiltreDlg>)
	MENU_TEXT(L"Границы АЧХ" , MenuItem<ACFBorderDlg>)
	MENU_TEXT(L"Мёртвые зоны" , MenuItem<DeadAreaDlg>)
	MENU_TEXT(L"Границы достоверности" , MenuItem<MainThresholdOption>)
	MENU_TEXT(L"Создать типоразмер", MenuItem<MainCreateTypesize>)
	MENU_TEXT(L"Удалить типоразмер", MenuItem<MainDeleteTypeSize>)
	MENU_TEXT(L"Обнулить счётчик труб", MenuItem<StoredData>)

	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	MENU_TEXT(L"Настройка аналоговой платы", MenuItem<L502ParamDlg>)	
	MENU_TEXT(L"Настройка сетевого подключения", MenuItem<CommunicationOptions>)	
	MENU_TEXT(L"Настройки генератора", MenuItem<SolenoidParametersTableDlg>)

	MENU_TEXT(L"Коэффициенты перерасчёта", MenuItem<OptionsCoefficients>)	
	MENU_TEXT(L"Пороги опорного сигнала", MenuItem<ReferencePointsDlg>)	
	MENU_TEXT(L"Добавить образец", MenuItem<TresholdWindow>)
//	MENU_TEXT(L"Дополнительные настройки", MenuItem<AdditionalSettingsDlg>)
//	MENU_TEXT(L"Цветовые настройки", MenuItem<ColorDlg>)
	MENU_TEXT(L"Сохранить координаты окна", MenuItem<CoordinatesDlg>)
//	MENU_TEXT(L"Управление дискретными выходами", MenuItem<ManagementIOportsDlg>)

//	MENU_TEXT(L"База", TopMenu<EditOperatorsWindowDlg>)
//	MENU_TEXT(L"Редактирование операторов", MenuItem<EditOperatorsWindowDlg>)	
	
//	MENU_TEXT(L"Редактирование типоразмеров", MenuItem<EditTypeSizeDlg>)
//	MENU_TEXT(L"Выбор порогов", MenuItem<ParametesCycleTest>)

	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
//	MENU_TEXT(L"Статус цикла", MenuItem<StatusCycle>)
	MENU_TEXT(L"О программе", MenuItem<MainAbout>)

//    MENU_TEXT(L"Порты ввода-вывода", SubMenu<InputsOutputs>)
//	MENU_TEXT(L"Входной порт", MenuItem<InputsDlg>)
//	MENU_TEXT(L"Выходной порт", MenuItem<OutputsDlg>)
	//MENU_TEXT(L"Дескриптор платы 1730", MenuItem<Descriptor1730Dlg>)



template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<Compute> >{static void Do(HWND h){compute.Recalculation();(*Automat::dataChanged)();}};//пересчёт
template<>struct Event<MenuItem<MainAbout> >{static void Do(HWND h){AboutWindow::Do(h);}};
template<>struct Event<MenuItem<MainThresholdOption> >{static void Do(HWND h){;}};//настройка порогов
template<>struct Event<MenuItem<L502ParamDlg> >{static void Do(HWND h){L502ParamDlg::Do(h);}};//настройка Lan10m8
template<>struct Event<MenuItem<OptionsCoefficients> >{static void Do(HWND h){;}};//

template<>struct Event<MenuItem<MedianFiltreDlg> >{static void Do(HWND h){;}};//
template<>struct Event<MenuItem<ACFBorderDlg> >{static void Do(HWND h){;}};//
template<>struct Event<MenuItem<DeadAreaDlg> >{static void Do(HWND h){;}};//



template<>struct Event<MenuItem<InputsDlg> >{static void Do(HWND h){;}};//
template<>struct Event<MenuItem<OutputsDlg> >{static void Do(HWND h){;}};//
template<>struct Event<MenuItem<Descriptor1730Dlg> >{static void Do(HWND h){;}};//

template<>struct Event<MenuItem<SaveDateFile> >{static void Do(HWND h){SaveDateFile::Do(h);}};//
template<>struct Event<MenuItem<LoadDateFile> >{static void Do(HWND h){LoadDateFile::Do(h);compute.Recalculation();(*Automat::dataChanged)();}};//

template<>struct Event<MenuItem<MainCreateTypesize> >{static void Do(HWND h){SelectTypeSizeNew::Do(h);}};//
template<>struct Event<MenuItem<MainDeleteTypeSize> >{static void Do(HWND h){SelectTypeSizeDelete::Do(h);}};//

template<>struct Event<MenuItem<StoredData> >
{
	static void Do(HWND h)
    {
		if(TypesizePasswordDlg().Do(h) && IDYES == MessageBox(h, L"Обнулить счётчик труб?", L"Предупреждение", MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2))
		{
			StoredData::Instance().Clear();
		}
	}
};

template<>struct Event<MenuItem<ReferencePointsDlg> >{static void Do(HWND h){ReferencePointsDlg::Do(h);}};//

template<>struct Event<MenuItem<CoordinatesDlg> >{static void Do(HWND h){CoordinatesDlg::Do(h);}};//


template<>struct Event<MenuItem<TresholdWindow> >{static void Do(HWND h){TresholdWindow::Open();}};//
template<>struct Event<MenuItem<CommunicationOptions> >{static void Do(HWND h){TcpCommunicationsDlg::Do(h);}};//

template<>struct Event<MenuItem<SolenoidParametersTableDlg> >{static void Do(HWND h){SolenoidParametersTableDlg::Do(h);}};//
}
