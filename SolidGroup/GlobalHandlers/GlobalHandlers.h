#pragma once
#include "typelist.hpp"

template<class T>struct GlobalHandlers;

#define MAKE_GLOBAL_HANDLER(name)\
struct name{};\
template<>struct GlobalHandlers<name>{void operator()();};


MAKE_GLOBAL_HANDLER(UpdateReferencePointsGridHandler)


struct UpdateReferencePointsGrid
{
	typedef TL::TypeToTypeLst<TL::MkTlst<
		UpdateReferencePointsGridHandler
	>::Result, GlobalHandlers>::Result items_list;
	void operator()();
};
