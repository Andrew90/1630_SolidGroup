#include "stdafx.h"
#include "Corel.h"
#include <math.h>
#include <algorithm>

#include "AppBase.h"
#include "Base.hpp"
#include "Corel.h"
#include "SolidData.h"

#include "DebugMess.h"


DEFINE_PARAM(ClassTube, int, 1)
	DEFINE_PARAM(TypeSize, int, 1)
	template<int >struct XParam;
DEFINE_PARAM_NUM(XParam, 1, double, 1)
	DEFINE_PARAM_NUM(XParam, 2, double, 1)
	DEFINE_PARAM_NUM(XParam, 3, double, 1)
	DEFINE_PARAM_NUM(XParam, 4, double, 1)
	DEFINE_PARAM_NUM(XParam, 5, double, 1)
	DEFINE_PARAM_NUM(XParam, 6, double, 1)
	DEFINE_PARAM_NUM(XParam, 7, double, 1)
	DEFINE_PARAM_NUM(XParam, 8, double, 1)

struct NameFile
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NameFile";}
	NameFile()
	{
		default_value = L"NONAME";
		value = default_value;
	}
};

struct SolidTubeTable
{
	typedef TL::MkTlst<
		XParam<1>
		, XParam<2>
		, XParam<3>
		, XParam<4>
		, XParam<5>
		, XParam<6>
		, XParam<7>
		, XParam<8>
		, ClassTube
		, TypeSize
		, NameFile
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SolidTubeTable";}
};

struct Name
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"Name";}
	Name()
	{
		default_value = L"NONAME";
		value = default_value;
	}
};

DEFINE_PARAM(ClassTubeID, int, 1)
	DEFINE_PARAM(TypeSizeID, int, 1)
	DEFINE_PARAM(CommunicationID, int, 1)

struct TypeSizeTable
{
	typedef TL::MkTlst<
		TypeSizeID
		, Name
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TypeSizeTable";}
};
struct ClassTubeTable
{
	typedef TL::MkTlst<
		CommunicationID
		, Name
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ClassTubeTable";}
};

struct SolidTubeBase
{
	typedef TL::MkTlst<
		SolidTubeTable
		, ClassTubeTable
	>::Result type_list; 
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[256];
	const wchar_t *name()
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\SolidTubeBase");
		return path;
	}
};

Corel::Corel()
{
}

Corel::~Corel()
{
	Clear();
}
struct DelSolid
{
	void operator()(Corel::SolidTubeItem *o){delete o;}
};
//struct DelClass
//{
//	void operator()(Corel::TClassTubeItem::value_type &o)
//	{
//		delete o.second;
//	}
//};
void Corel::Clear()
{
	std::for_each(solidTubeItem.begin(), solidTubeItem.end(), DelSolid());
	solidTubeItem.clear();

	//std::for_each(classTubeItem.begin(), classTubeItem.end(), DelClass());
	//classTubeItem.clear();
}

namespace __corel__
{	
	template<class O, class P>struct __get__
	{
		typedef typename O::__template_must_be_overridded__ noused;
	};
	template<class P>struct __get__<ClassTube, P>
	{
		typedef ClassTube O;
		void operator()(O *o, P *p)
		{
			p->classTube = o->value;
		}
	};
	template<class P>struct __get__<TypeSize, P>
	{
		typedef TypeSize O;
		void operator()(O *o, P *p)
		{
			p->typeSize = o->value;
		}
	};
	template<int N, class P>struct __get__<XParam<N>, P>
	{
		typedef XParam<N> O;
		void operator()(O *o, P *p)
		{
			p->elements[N - 1] = o->value;
		}
	};
	template<class P>struct __get__<NameFile, P>
	{
		typedef NameFile O;
		void operator()(O *o, P *p)
		{
			p->fileName = o->value;
		}
	};
	template<class O, class P>struct __set__
	{
		typedef typename O::__template_must_be_overridded__ noused;
	};
	template<class P>struct __set__<ClassTube, P>
	{
		typedef ClassTube O;
		void operator()(O *o, P *p)
		{
			o->value = p->classTube;
		}
	};
	template<class P>struct __set__<TypeSize, P>
	{
		typedef TypeSize O;
		void operator()(O *o, P *p)
		{
			o->value = p->typeSize;
		}
	};
	template<int N, class P>struct __set__<XParam<N>, P>
	{
		typedef XParam<N> O;
		void operator()(O *o, P *p)
		{
			o->value = p->elements[N - 1];
		}
	};
	template<class P>struct __set__<NameFile, P>
	{
		typedef NameFile O;
		void operator()(O *o, P *p)
		{
			o->value = (wchar_t *)p->fileName.c_str();
		}
	};
	struct __set_element_data__
	{
		int classTube;
		int typeSize;
		double (&param)[8];
		std::wstring &file;
		__set_element_data__(int ct, int ts, double (&p)[8], std::wstring &f)
			: classTube(ct)
			, typeSize(ts)
			, param(p)
			, file(f)
		{}
	};
	template<class O, class P>struct __set_element__;
	template<class P>struct __set_element__<ClassTube, P>
	{
		typedef ClassTube O;
		void operator()(O *o, P *p)
		{
			o->value = p->classTube;
		}
	};
	template<class P>struct __set_element__<TypeSize, P>
	{
		typedef TypeSize O;
		void operator()(O *o, P *p)
		{
			o->value = p->typeSize;
		}
	};
	template<int N, class P>struct __set_element__<XParam<N>, P>
	{
		typedef XParam<N> O;
		void operator()(O *o, P *p)
		{
			o->value = p->param[N - 1];
		}
	};
	template<class P>struct __set_element__<NameFile, P>
	{
		void operator()(NameFile *o, P *p)
		{
			o->value = (wchar_t *)p->file.c_str();
		}
	};
}
void Corel::Init()
{
	SolidTubeBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<SolidTubeBase::type_list>()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		ADODB::_RecordsetPtr rec;
		Select<ClassTubeTable>(base).Execute(rec);
		while (!rec->EndOfFile) 
		{
			ClassTubeItem *i = new ClassTubeItem;
			int key = rec->Fields->GetItem(L"ID")->GetValue();
			i->Name = _bstr_t(rec->Fields->GetItem(Name().name())->GetValue());
			classTubeItem[key] = i;
			rec->MoveNext(); 
		}
	}
	{
		ParametersBase parameters;
		CBase base(ParametersBase().name()	);
		if(base.IsOpen())
		{
			ADODB::_RecordsetPtr rec;
			Select<ParametersTable>(base).Execute(rec);
			while (!rec->EndOfFile) 
			{
				NewTypeSizeItem(
					rec->Fields->GetItem(L"ID")->GetValue()
					, _bstr_t(rec->Fields->GetItem(NameParam().name())->GetValue())
					);
				rec->MoveNext(); 
			}
		}	
	}
}

void Corel::NewTypeSizeItem(int key, wchar_t *name)
{
	TypeSizeItem *t = new TypeSizeItem;
	t->Name = name;
	typeSizeItem[key] = t;
}

void Corel::LoadFromBase(int typeSize)
{
	SolidTubeBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<SolidTubeBase::type_list>()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		Clear();
		ADODB::_RecordsetPtr rec;
		typedef Select<SolidTubeTable> sel;
		sel(base).Execute(rec);

		SolidTubeTable table;
		while (!rec->EndOfFile) 
		{
			TL::foreach<SolidTubeTable::items_list, sel::set_to_>()(&table.items, rec.GetInterfacePtr());
			if(typeSize == table.items.get<TypeSize>().value)
			{
				Corel::SolidTubeItem *items = new Corel::SolidTubeItem;
				TL::foreach<SolidTubeTable::items_list, __corel__::__get__>()(&table.items, items);
				double root = 0;
				for(int i = 0; i < dimention_of(items->elements); ++i)
				{
					double a = items->elements[i];
					root += a * a;
				}
				items->root = sqrt(root);
				solidTubeItem.push_back(items);
			}
			rec->MoveNext(); 
		}				
	}
}

void Corel::InsertElement(SolidTubeItem &item)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		SolidTubeTable table;
		TL::foreach<SolidTubeTable::items_list, __corel__::__set__>()(&table.items, &item);
		int id = Select<SolidTubeTable>(base).eq_all<SolidTubeTable::items_list>(&table.items).Execute();
		if(0 == id)
		{
			if(0 != item.classTube)
			{
				wchar_t path[1024];
				GetModuleFileName(0, path, 1024);
				PathRemoveFileSpec(path);

				wsprintf(&path[wcslen(path)], L"\\ConfigThresholds\\%s\\%s\\"
					, (wchar_t *)corel.typeSizeItem[item.typeSize]->Name.c_str()
					, corel.ClassTubeName());
				time_t t = time(0);
				struct tm *now = localtime( & t );
				wchar_t *s = &path[wcslen(path)];
				wsprintf(s, L"%s_%02d%02d%02d_%02d%02d%02d.dat"
					, corel.ClassTubeName()
					, now->tm_year - 100
					, now->tm_mon + 1
					, now->tm_mday
					, now->tm_hour
					, now->tm_min
					, now->tm_sec
					);
				FILE *f= _wfopen(path, L"wb+");
				if(NULL != f)
				{
					fwrite(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f)
						&& fwrite(solidData.referenceBuffer, sizeof(double) * solidData.currentOffset, 1, f)
						&& fwrite(&solidData.dataBuffer, sizeof(double) * solidData.currentOffset, 1, f)
						;
					fclose(f);
				}
				table.items.get<NameFile>().value = s;
				Insert_Into<SolidTubeTable>(table, base).Execute();
				SolidTubeItem *items = new SolidTubeItem;
				*items = item;
				items->fileName = s;
				double root = 0;
				for(int i = 0; i < dimention_of(items->elements); ++i)
				{
					double a = items->elements[i];
					root += a * a;
				}
				items->root = sqrt(root);
				solidTubeItem.push_back(items);
			}
		}
	}
}

void Corel::InsertListData(std::list<Data> &lst)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		for(std::list<Data>::iterator i = lst.begin(); i != lst.end(); ++i)
		{
			if(wcsncmp(L"TEST", i->Name.c_str(), 4) == 0) continue;
			Name n;
			wchar_t buf[128];
			wsprintf(buf, L"%s", i->Name.c_str());
			n.value = buf;
			int idClassTube = Select<ClassTubeTable>(base).eq<Name>(n.value).Execute();

			SolidTubeTable table;
			TL::foreach<SolidTubeTable::items_list, __corel__::__set_element__>()(
				&table.items
				, &__corel__::__set_element_data__(
				idClassTube
				, Singleton<CurrentParametersTable>::Instance().items.get<CurrentID>().value
				, i->tresholds
				, i->File
				)
				);
			int id = Select<SolidTubeTable>(base).eq_all<SolidTubeTable::items_list>(&table.items).Execute();
			if(0 == id)
			{
				Insert_Into<SolidTubeTable>(table, base).Execute();
				SolidTubeItem *items = new SolidTubeItem;
				items->classTube = idClassTube;
				items->typeSize = Singleton<CurrentParametersTable>::Instance().items.get<CurrentID>().value;
				memcpy(items->elements, i->tresholds, sizeof(items->elements));
				items->fileName = i->File;
				double root = 0;
				for(int i = 0; i < dimention_of(items->elements); ++i)
				{
					double a = items->elements[i];
					root += a * a;
				}
				items->root = sqrt(root);
				solidTubeItem.push_back(items);
			}
		}
	}
}

struct rem_
{
	Corel::SolidTubeItem *t;
	rem_(Corel::SolidTubeItem *t) : t(t){}
	bool operator()(Corel::SolidTubeItem *item)
	{
		if(
			t->classTube == item->classTube
			&& t->typeSize == item->typeSize
			&& t->fileName == item->fileName
			)
		{
			delete item;
			return true;
		}
		return false;
	}
};
void Corel::RemoveElement(SolidTubeItem &item)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		SolidTubeTable table;
		TL::foreach<SolidTubeTable::items_list, __corel__::__set__>()(&table.items, &item);

		if(Delete<SolidTubeTable>(base).eq_all<SolidTubeTable::items_list>(&table.items).Execute() && 0 == Select<SolidTubeTable>(base).eq_all<SolidTubeTable::items_list>(&table.items).Execute())
		{
			wchar_t path[1024];
			GetModuleFileName(0, path, 1024);
			PathRemoveFileSpec(path);

			wsprintf(&path[wcslen(path)], L"\\ConfigThresholds\\%s\\%s\\%s"
				, (wchar_t *)corel.typeSizeItem[item.typeSize]->Name.c_str()
				, (wchar_t *)classTubeItem[item.classTube]->Name.c_str()
				, (wchar_t *)item.fileName.c_str()
				);
			_wremove(path);
			solidTubeItem.erase(
				std::remove_if(solidTubeItem.begin(), solidTubeItem.end(), rem_(&item)), solidTubeItem.end());
		}
	}
}

void Corel::RemoveAllElement()
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		int currentTypeSize = Singleton<CurrentParametersTable>::Instance().items.get<CurrentID>().value;
		Delete<SolidTubeTable>(base).eq<TypeSize>(currentTypeSize).Execute();
		for(unsigned i = 0; i < solidTubeItem.size(); ++i)
		{
			delete solidTubeItem[i];
		}
		solidTubeItem.clear();

	}
}


struct DataInc
{
	double data;
	int counter;
};
struct Compute_Compare
{
	Corel::SolidTubeItem &vec;
	double correlation;
	Compute_Compare(Corel::SolidTubeItem &vec)
		: vec(vec)
		, correlation(0) 
	{
	}
	void operator()(Corel::SolidTubeItem *item)
	{
		double t = 0;
		for(int i = 0; i < dimention_of(item->elements); ++i)
		{
			t += vec.elements[i] * item->elements[i];
		}
		item->correlation = t / (vec.root * item->root); 

		if(item->correlation > correlation)
		{
			correlation = item->correlation;
			vec.classTube = item->classTube;
			vec.correlation = item->correlation;
		}
	}
};

void Corel::Compute()
{
	double root = 0;
	for(int i = 0; i < dimention_of(inputItem.elements); ++i)
	{
		double a = inputItem.elements[i];
		root += a * a;
	}
	inputItem.root = sqrt(root);
	std::for_each(solidTubeItem.begin(), solidTubeItem.end(), Compute_Compare(inputItem));
}
//------------------------------------------------------------------------------------
int Corel::InsertGroupeName(wchar_t *s, int communicationTCP)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		Name n;
		n.value = s;
		int id = Select<ClassTubeTable>(base).eq<Name>(n.value).Execute();
		if(0 == id)
		{
			ClassTubeTable table;
			table.items.get<Name>().value = s;
			table.items.get<CommunicationID>().value = communicationTCP;
			Insert_Into<ClassTubeTable>(table, base).Execute();
		}
		else
		{
			Update<ClassTubeTable>(base).set<CommunicationID>(communicationTCP).Where().ID(id).Execute();
		}
		return Select<ClassTubeTable>(base).eq<Name>(n.value).Execute();
	}
	return 0;
}
bool Corel::RemoveGroupeName(wchar_t *s)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		Name n;
		n.value = s;
		int nameID = Select<ClassTubeTable>(base).eq<Name>(n.value).Execute();//rec);
		if(0 == Select<SolidTubeTable>(base).eq<ClassTube>(nameID).Execute())
		{
			Delete<ClassTubeTable>(base).eq<Name>(n.value).Execute();
			return true;
		}
	}
	return false;
}
int Corel::ReNameGroupeName(wchar_t *first, wchar_t *second)
{
	CBase base(SolidTubeBase().name());
	if(base.IsOpen())
	{
		Name f;
		f.value = first;
		Name s;
		s.value = second;
		Update<ClassTubeTable>(base).set<Name>(s.value).Where().eq<Name>(f.value).Execute();
		return Select<ClassTubeTable>(base).eq<Name>(s.value).Execute();
	}
	return 0;
}
//-------------------------------------------------------------------------------------------
const wchar_t *Corel::ClassTubeName()
{
	if(0 != corel.inputItem.classTube)
	{
		return classTubeItem[corel.inputItem.classTube]->Name.c_str();
	}
	return L"";
}