#pragma once
#include "BaseDefine.h"
#include "typelist.hpp"

DEFINE_PARAM(MarkPaint, bool, true)

struct AdditionalParametersTable
{
	typedef TL::MkTlst<	
		MarkPaint
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalParametersTable";}
};

struct AdditionalParametersBase
{
	typedef TL::MkTlst<
		AdditionalParametersTable
	>::Result type_list; 

	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[256];
	const wchar_t *name();
};

