#pragma once
#include "AppBase.h"
#include <list>
#include <string>
#include <map>
#include <vector>
#include "AutomaticTresholdsCommon.h"

template<class List>struct SelectWithWapper;
template<class Head, class Tail>struct SelectWithWapper<Tlst<Head, Tail> >
{
	typedef typename SelectWithWapper<Tail>::Result Result;
};
template<int N, template<int>class W, class Tail>struct SelectWithWapper<Tlst<W<N>, Tail> >
{
	typedef typename Tlst<W<N>, typename SelectWithWapper<Tail>::Result> Result;
};
template<>struct SelectWithWapper<NullType>
{
	typedef NullType Result;
};

class Corel
{   
public:
	struct SolidTubeItem
	{
		static const int size_array = TL::Length<SelectWithWapper<ThresholdsTable::items_list>::Result>::value;
		int classTube;
		int typeSize;
		std::wstring fileName;
		double elements[size_array];
		double root;
		double correlation;
	};
	struct TypeSizeItem
	{
		std::wstring Name;
	};
	struct ClassTubeItem
	{
	//	int communicationID;
		unsigned color;
		std::wstring Name;
	};
public:
	 std::vector<SolidTubeItem *> solidTubeItem; 
	 typedef std::map<int, TypeSizeItem *> TTypeSizeItem;
	 TTypeSizeItem typeSizeItem;
	 typedef std::map<int, ClassTubeItem *> TClassTubeItem;
	 TClassTubeItem classTubeItem;
public:
	SolidTubeItem inputItem;
	double correlation;
	int classTube;
	unsigned selectedRow;
	Corel();
	~Corel();
	void Init();
	void LoadFromBase(int typeSize);
	void InsertElement(SolidTubeItem &);
	void InsertListData(std::list<Data> &);

	void RemoveElement(SolidTubeItem &);
	void RemoveAllElement();
	void Clear();
	void Compute();
	int InsertGroupeName(wchar_t *, int);
	bool RemoveGroupeName(wchar_t *);
	int ReNameGroupeName(wchar_t *, wchar_t *);
	const wchar_t *ClassTubeName();
	void NewTypeSizeItem(int, wchar_t *);
};

extern Corel corel;
