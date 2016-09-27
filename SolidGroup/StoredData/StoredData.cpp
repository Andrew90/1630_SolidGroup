#include "StdAfx.h"
#include "StoredData.h"
#include "Base.hpp"
#include "AppBase.h"

//class StoredCounter
//{
//public:
//	class CounterBase;
//public:
//  int Curent();
//  void Clear();
//};

//StoredData::StoredData()
//	//: num(0)
//{
//	//num = StoredCounter().Curent();
//}

 //StoredData &StoredData::Instance()
 //{
//	 static StoredData x;
//	 return x;
 //}

//int StoredData::Current()
//{
////	return num;
//}

//void StoredData::Clear()
//{
//	//num = 0;
//	StoredCounter().Clear();
//}

//DEFINE_PARAM(CurrentTubes, int, 0)
STR_PARAM(Diameters, 64, L"")

STR_PARAM(GroupStrength, 32, L"Z");

struct CurrentTime
{
	typedef COleDateTime type_value;
	type_value value;
	const wchar_t *name(){return L"CurrentTime";}
	CurrentTime()  
	{
		value = COleDateTime::GetCurrentTime();
    }
};

struct TubesTable
{
	typedef TL::MkTlst<
	//	CurrentTubes
		 CurrentTime
		, Diameters
		, GroupStrength
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubesTable";}
};

struct StoredDataBase
{
	typedef TL::MkTlst<
		TubesTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[512];
	const wchar_t *name(wchar_t *baseName)
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
	    wcscat(path, L"\\Data\\");
	    CreateDirectory(path, NULL);
		wsprintf(&path[wcslen(path)], L"\\%s", baseName);
		return path;
	}
};

//struct CurrentTubeTable
//{
//	typedef TL::MkTlst<
//		CurrentTubes
//	>::Result items_list;
//	typedef NullType unique_list;
//	typedef TL::Factory<items_list> TItems;
//	TItems items;
//	const wchar_t *name(){return L"CurrentTubeTable";}
//};
//struct CurrentTubeBase
//{
//	typedef TL::MkTlst<
//		CurrentTubeTable
//	>::Result type_list;
//	typedef TL::Factory<type_list> TTables;
//	TTables tables;
//	wchar_t path[512];
//	const wchar_t *name()
//	{
//		GetModuleFileName(0, path, dimention_of(path));
//		PathRemoveFileSpec(path);
//		wcscat(path, L"\\CurrentTubeBase");
//		return path;
//	}
//};

void StoredData::Store(wchar_t *diametre, wchar_t *gr)
{
	TubesTable table;
	//num =  StoredData::Current(); 
//	table.items.get<CurrentTubes >().value = num;
	//table.items.get<Diameters    >().value = _wtoi(diametre);
	table.items.get<Diameters    >().value = diametre;
	table.items.get<GroupStrength>().value = gr;

	time_t t = time(0);
	struct tm *now = localtime( & t );

	wchar_t baseName[32];
	 wsprintf(baseName, L"%02d%02d"
		 , now->tm_year - 100
		 , now->tm_mon + 1
		 );

	StoredDataBase parameters;
	CBase base(
		parameters.name(baseName)
		, CreateDataBase<StoredDataBase::type_list>()
		, parameters.tables
		);
		if(base.IsOpen())
		{
			Insert_Into<TubesTable>(table, base).Execute();
		}
}

//class StoredCounter::CounterBase: public CBase
//{
//public:
//	StoredCounter::CounterBase() : CBase(
//		CurrentTubeBase().name()
//		, CreateDataBase<CurrentTubeBase::type_list, SetDefault<CurrentTubeBase::type_list> >()
//		, CurrentTubeBase().tables
//		)
//	{}
//};
//
//int StoredCounter::Curent()
//{
//	int ret = 0;
//	CounterBase base;
//	if(base.IsOpen())
//	{
//		CurrentTubeTable tt;
//		Select<CurrentTubeTable>(base).ID(1).Execute(tt);
//		ret =  tt.items.get<CurrentTubes>().value;
//	}
//	return ret;
//}

//void StoredData::Inc()
//{
//	StoredCounter::CounterBase base;
//	if(base.IsOpen())
//	{
//		++num;
//		Update<CurrentTubeTable>(base).set<CurrentTubes>(num).Where().ID(1).Execute();
//	}
//}
//
//void StoredCounter::Clear()
//{
//	CounterBase base;
//	if(base.IsOpen())
//	{
//		Update<CurrentTubeTable>(base).set<CurrentTubes>(0).Where().ID(1).Execute();
//	}
//}
