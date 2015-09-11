/*
#if 0
#include "stdafx.h"
#include "StoredData.h"
#include <time.h>
#include <Windows.h>
#include <ShlWapi.h>
#include <stdio.h>
#pragma warning(disable:4996)
#pragma message("Дописать базу данных")
StoredData::StoredData()
	: num(-1)
{}
#if 1
void StoredData::Store(wchar_t *diametre, wchar_t *gr)
{
	 time_t t = time(0);
	 struct tm *now = localtime( & t );
	 wchar_t buf[1024];
	 wsprintf(buf, L"%5d %4d-%02d-%02d_%02d:%02d:%02d %5s %5s\n"
		    , num
			, now->tm_year + 1900
			, now->tm_mon + 1
			, now->tm_mday
			, now->tm_hour
			, now->tm_min
			, now->tm_sec
			, diametre
			, gr
			);
	 ++num;

	 wchar_t path[1024];
	 GetModuleFileName(0, path, 1024);
	 PathRemoveFileSpec(path);
	 wcscat(path, L"\\Data\\");
	 CreateDirectory(path, NULL);
	 wsprintf(&path[wcslen(path)], L"%4d-%02d-%02d.txt"
		 , now->tm_year + 1900
		 , now->tm_mon + 1
		 , now->tm_mday
		 );

	 FILE *f = _wfopen(path, L"at+");
	 if(f)
	 {
		 fputws(buf, f);
		 fclose(f);
	 }
}
#else

void StoredData::Store(wchar_t *diametre, wchar_t *gr)
{
	 wchar_t path[1024];
	 GetModuleFileName(0, path, 1024);
	 PathRemoveFileSpec(path);
	 wcscat(path, L"\\Data\\");
	 CreateDirectory(path, NULL);
	 wsprintf(&path[wcslen(path)], L"%4d-%02d.txt"
		 , now->tm_year + 1900
		 , now->tm_mon + 1
		 );

	 
}
#endif
#else
*/
#include "StdAfx.h"
#include "StoredData.h"
#include "Base.hpp"
#include "AppBase.h"

class StoredCounter
{
public:
	class CounterBase;
public:
  int Curent();
 // int Inc();
  void Clear();
};

StoredData::StoredData()
	: num(0)
{
	num = StoredCounter().Curent();
}

 StoredData &StoredData::Instance()
 {
	 static StoredData x;
	 return x;
 }

int StoredData::Current()
{
	return num;
}

void StoredData::Clear()
{
	num = 0;
	StoredCounter().Clear();
}

DEFINE_PARAM(CurrentTubes, int, 0)
DEFINE_PARAM(Diameters, int, 73)

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
		CurrentTubes
		, CurrentTime
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

struct CurrentTubeTable
{
	typedef TL::MkTlst<
		CurrentTubes
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentTubeTable";}
};
struct CurrentTubeBase
{
	typedef TL::MkTlst<
		CurrentTubeTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[512];
	const wchar_t *name()
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\CurrentTubeBase");
		return path;
	}
};

void StoredData::Store(wchar_t *diametre, wchar_t *gr)
{
	TubesTable table;
	num =  StoredData::Current(); 
		//StoredCounter().Inc();
	table.items.get<CurrentTubes >().value = num;
	table.items.get<Diameters    >().value = _wtoi(diametre);
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
		, CreateDataBase<StoredDataBase::type_list>()//, SetDefault<StoredDataBase::type_list> >()
		, parameters.tables
		);
		if(base.IsOpen())
		{
			Insert_Into<TubesTable>(table, base).Execute();
		}
}

class StoredCounter::CounterBase: public CBase
{
public:
	StoredCounter::CounterBase() : CBase(
		CurrentTubeBase().name()
		, CreateDataBase<CurrentTubeBase::type_list, SetDefault<CurrentTubeBase::type_list> >()
		, CurrentTubeBase().tables
		)
	{}
};

int StoredCounter::Curent()
{
	int ret = 0;
	CounterBase base;
	if(base.IsOpen())
	{
		CurrentTubeTable tt;
		Select<CurrentTubeTable>(base).ID(1).Execute(tt);
		ret =  tt.items.get<CurrentTubes>().value;
	}
	return ret;
}
/*
void StoredCounter::Inc()
{
	int ret = 0;
	CounterBase base;
	if(base.IsOpen())
	{
		///CurrentTubeTable tt;
		//Select<CurrentTubeTable>(base).ID(1).Execute(tt);
		//ret =  tt.items.get<CurrentTubes>().value;
		Update<CurrentTubeTable>(base).set<CurrentTubes>(1 + ret).Where().ID(1).Execute();
	}
	return ret;
}
*/
void StoredData::Inc()
{
	StoredCounter::CounterBase base;
	if(base.IsOpen())
	{
		++num;
		Update<CurrentTubeTable>(base).set<CurrentTubes>(num).Where().ID(1).Execute();
	}
}

void StoredCounter::Clear()
{
	CounterBase base;
	if(base.IsOpen())
	{
		Update<CurrentTubeTable>(base).set<CurrentTubes>(0).Where().ID(1).Execute();
	}
}
//#endif