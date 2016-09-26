#pragma once
#include <windows.h>
#include "typelist.hpp"
//-----------------------------------------------------------------
class CopyFactory
{
template<class O, class P>struct loc
{
	void operator ()(O *o, P *p)
	{
		o->value = p->get<O>().value;
	}
};
public:
	template<class List>static void Do(TL::Factory<List> &to, TL::Factory<List> &from)
	{
		TL::foreach<List, loc>()(&to, &from);
	}
};
//-------------------------------------------------------------------
struct AboutWindow
{
	static void Do(HWND);
};

struct SelectChannelParamDlg
{
	static void Do(HWND h);
};

struct SettingTresholdsParamDlg
{
	static void Do(HWND h);
};

struct OptionConnectionACSParamDlg
{
	static void Do(HWND h);
};

struct ThresholdsParamDlg
{
	static void Do(HWND h);
};

struct L502ParamDlg
{
	static void Do(HWND h);
};

struct CoefficientDlg
{
	static void Do(HWND h);
};

struct SignalDlg
{
	static void Do(HWND h);
};

struct MedianFiltreDlg
{
	static void Do(HWND h);
};

struct DeadAreaDlg
{
	static void Do(HWND h);
};

struct PaintMakerDlg
{
	static void Do(HWND h);
};
struct WorkTimeDlg
{
	static void Do(HWND h);
};
struct AdditionalSettingsDlg
{
	static void Do(HWND h);
};
struct ColorDlg
{
	static void Do(HWND h);
};
struct InputsDlg
{
	static void Do(HWND);
};
struct OutputsDlg
{
	static void Do(HWND);
};
struct CoordinatesDlg
{
	static void Do(HWND h);
};
struct CoordinatesZonesDlg
{
	static void Do(HWND h);
};

struct ManagementIOportsDlg
{
	static void Do(HWND h);
	static void SendTitle();
};
struct Descriptor1730Dlg
{
	static void Do(HWND h);
};
struct ACFBorderDlg
{
	static void Do(HWND h);
};

struct NormativeTechnicalDocumentDlg
{
	static void Do(HWND h);
};

struct SteelGradeDlg
{
	static void Do(HWND h);
};

struct TubeDiametersDlg
{
	static void Do(HWND h);
};

struct NominalThresholdDlg
{
	static void Do(HWND h);
};

struct EditTypeSizeDlg
{
	static void Do(HWND h);
};

struct EditOperatorsWindowDlg
{
	static void Do(HWND h);
};

struct ReferencePointsDlg
{
	static void Do(HWND h);
};

class SelectTypeSizeNew
{
public:
	static void Do(HWND);
};
class SelectTypeSizeDelete
{
public:
	static void Do(HWND);
};

class TcpCommunicationsDlg
{
public:
	static void Do(HWND);
};

class AdcInputPortsDlg
{
public:
	static void Do(HWND);
};

class DInputPortsDlg
{
public:
	static void Do(HWND);
};