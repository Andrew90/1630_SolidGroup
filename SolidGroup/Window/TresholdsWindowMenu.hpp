#pragma once 

namespace TresholdsWindowSpace
{
struct MainFile{};
struct MainExit{};
struct CoordinatesZonesDlg
{
	static void Do(HWND);
};

struct MainOptionUnits{};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<CoordinatesZonesDlg>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<CoordinatesZonesDlg>)
	
template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};
template<>struct Event<MenuItem<CoordinatesZonesDlg> >{static void Do(HWND h){CoordinatesZonesDlg::Do(h);}};

void CoordinatesZonesDlg::Do(HWND h)
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		RECT r;
		GetWindowRect(h, &r);
		TresholdWindowTable o;
		o.items.get<OffsetWindowX>().value = r.left;
		o.items.get<OffsetWindowY>().value = r.top;
		o.items.get<WindowWidth>().value = r.right - r.left;
		o.items.get<WindowHeight>().value = r.bottom - r.top;
		UpdateWhere<TresholdWindowTable>(o, base).ID(1).Execute();
	}
}
}