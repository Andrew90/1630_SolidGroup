#include "StdAfx.h"
#include "AdditionalBase.h"
#include <Shlwapi.h>

#pragma warning(disable : 4996)
//------------------------------------------------------------------------
const wchar_t *AdditionalParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\AdditionalParametersBase");
	return path;
}
//-------------------------------------------------------------------------	
