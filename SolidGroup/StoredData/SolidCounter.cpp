#include "stdafx.h"
#include "SolidCounter.h"
#include "AppBase.h"

namespace SolidCounterSpace
{
DEFINE_PARAM(CountTube, int, 0)
STR_PARAM(NameParam, 64, L"")
STR_PARAM(SolidGroup, 64, L"")

struct SolidCounterTable
{
	typedef TL::MkTlst<	
		CountTube
		, NameParam
		, SolidGroup
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SolidCounterTable";}
};

struct SolidCounterBase
{
	typedef TL::MkTlst<
		SolidCounterTable
	>::Result type_list;

	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[256];
	const wchar_t *name()
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\SolidCounterBase");
		return path;
	}
};

}

SolidCounter::SolidCounter()
	: base(
		SolidCounterSpace::SolidCounterBase().name()
		, CreateDataBase<SolidCounterSpace::SolidCounterBase::type_list, SetDefault<SolidCounterSpace::SolidCounterBase::type_list> >()
		, SolidCounterSpace::SolidCounterBase().tables
		)
		, data(SolidCounterData::Instance())
{
}

int SolidCounter::Get(wchar_t *nameParam, wchar_t *solidGroup)
{
	if(base.IsOpen())
	{
		typedef TL::MkTlst<SolidCounterSpace::NameParam, SolidCounterSpace::SolidGroup>::Result lstIn;
		TL::Factory<lstIn> facIn;
		facIn.get<SolidCounterSpace::NameParam>().value = nameParam;
		facIn.get<SolidCounterSpace::SolidGroup>().value = solidGroup;

		typedef TL::MkTlst<SolidCounterSpace::CountTube>::Result lstOut;
		SolidCounterSpace::SolidCounterTable facOut;

		if(0 != Select<SolidCounterSpace::SolidCounterTable>(base).eq_all<lstIn>(&facIn).Execute(facOut))
		{
			return facOut.items.get<SolidCounterSpace::CountTube>().value;
		}
	}
	return 0;
}

void SolidCounter::Set(int value, wchar_t *nameParam, wchar_t *solidGroup)
{
	if(base.IsOpen())
	{
		SolidCounterSpace::NameParam n;
		n.value = nameParam;
		SolidCounterSpace::SolidGroup s;
		s.value = solidGroup;

		int id = Select<SolidCounterSpace::SolidCounterTable>(base)
			.eq<SolidCounterSpace::NameParam>(n.value)
			.eq<SolidCounterSpace::SolidGroup>(s.value).Execute();
		if(0 == id)
		{
			SolidCounterSpace::SolidCounterTable t;
			t.items.get<SolidCounterSpace::NameParam>().value = nameParam;
			t.items.get<SolidCounterSpace::SolidGroup>().value = solidGroup;
			t.items.get<SolidCounterSpace::CountTube>().value = value;
			Insert_Into<SolidCounterSpace::SolidCounterTable>(t, base).Execute();
		}
		else
		{
			Update<SolidCounterSpace::SolidCounterTable>(base)
				.set<SolidCounterSpace::CountTube>(value).Where()
				.eq<SolidCounterSpace::NameParam>(n.value)
				.eq<SolidCounterSpace::SolidGroup>(s.value).Execute();
		}
		data.data[solidGroup] = value;
	}
}

void SolidCounter::Clear(wchar_t *nameParam)
{
	if(base.IsOpen())
	{
		SolidCounterSpace::NameParam s;
		s.value = nameParam;
		Delete<SolidCounterSpace::SolidCounterTable>(base).eq<SolidCounterSpace::NameParam>(s.value).Execute();
	}
}

void SolidCounter::Load(wchar_t *nameParam)
{
	if(base.IsOpen())
	{
		SolidCounterSpace::NameParam s;
		s.value = nameParam;
		ADODB::_RecordsetPtr rec;
		Select<SolidCounterSpace::SolidCounterTable>(base).eq<SolidCounterSpace::NameParam>(s.value).Execute(rec);
		while (!rec->EndOfFile) 
		{
			int val = rec->Fields->GetItem(SolidCounterSpace::CountTube().name())->GetValue();
			_bstr_t sg(rec->Fields->GetItem(SolidCounterSpace::SolidGroup().name())->GetValue());			
			data.data[(wchar_t *)sg] = val;
			rec->MoveNext(); 
		}
	}
}

void SolidCounter::Store(wchar_t *nameParam)
{
	if(base.IsOpen())
	{
		SolidCounterSpace::SolidCounterTable t;
		t.items.get<SolidCounterSpace::NameParam>().value = nameParam;

		for(std::map<std::wstring, int>::iterator i = data.data.begin(); data.data.end() != i; ++i)
		{
			t.items.get<SolidCounterSpace::SolidGroup>().value = (wchar_t *)i->first.c_str();
			if(!Update<SolidCounterSpace::SolidCounterTable>(base)
				.set<SolidCounterSpace::CountTube>(i->second)
				.Where()
				.eq<SolidCounterSpace::NameParam>(t.items.get<SolidCounterSpace::NameParam>().value)
				.eq<SolidCounterSpace::SolidGroup>(t.items.get<SolidCounterSpace::SolidGroup>().value).Execute()
				)
			{
				t.items.get<SolidCounterSpace::CountTube>().value = i->second;
				Insert_Into<SolidCounterSpace::SolidCounterTable>(t, base).Execute();
			}
		}
	}
}

int SolidCounter::SolidGroupCount(wchar_t *solidGroup)
{
	return data.data[solidGroup];
}
