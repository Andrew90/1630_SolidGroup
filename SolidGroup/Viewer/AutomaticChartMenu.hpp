#pragma once

namespace AutomaticChartMenu
{
	struct MainFile{};
	struct MainExit{};
	struct Graph{};
	
	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};

	template<>struct TopMenu<Graph>
	{
		typedef TL::MkTlst<
			MenuItem<PointsOptionsTableDlg>
			, MenuItem<SignalOffset>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<Graph>
	>::Result MainMenu;


	MENU_TEXT(L"Файл", TopMenu<MainFile>)

	MENU_TEXT(L"Выход", MenuItem<MainExit>)
	template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы

	MENU_TEXT(L"График", TopMenu<Graph>)
	MENU_TEXT(L"Первичный сигнал", MenuItem<SignalOffset>)
	MENU_TEXT(L"Точки смещения", MenuItem<PointsOptionsTableDlg>)
	template<>struct Event<MenuItem<SignalOffset> >{static void Do(HWND h){SignalOffset::Do(h);}};
	template<>struct Event<MenuItem<PointsOptionsTableDlg> >{static void Do(HWND h){PointsOptionsTableDlg::Do(h);}};
};