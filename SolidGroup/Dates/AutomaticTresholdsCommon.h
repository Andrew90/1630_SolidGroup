#pragma once
#include <string>

struct Data
{
	unsigned color;
	std::wstring Name;
	std::wstring File;
	double tresholds[8];
};

struct CallMetod
{
	struct Obj {};
	typedef void (Obj::*PtrMet)();
	Obj *o;
	PtrMet ptrMet;
	CallMetod() : o(NULL), ptrMet(NULL) {}
	virtual void Do() = 0;
};

template<class T>struct CallMetodTemplate : CallMetod
{
	void Set(T *t, void (T::*ptr)())
	{
		o = (Obj *)t;
		ptrMet = (PtrMet)ptr;
	}
	void Do()
	{
		if(o&&ptrMet)(((T *)o)->*((void (T::*)())ptrMet))();
	}
};