#pragma once
#include "Base.hpp"
#include "SolidCounterData.h"

class SolidCounter
{
	CBase base;
public:
	SolidCounterData &data;
public:
  SolidCounter();
  int Get(wchar_t *nameParam, wchar_t *solidGroup);
  void Set(int, wchar_t *nameParam, wchar_t *solidGroup);
  void Clear(wchar_t *nameParam);
  void Load(wchar_t *nameParam);
  void Store(wchar_t *nameParam);
  int SolidGroupCount(wchar_t *solidGroup);
};