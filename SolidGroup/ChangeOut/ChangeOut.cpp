#include "stdafx.h"
#include "ChangeOut.h"
#include <Windows.h>
#include <Shlwapi.h>
#include "typelist.hpp"
#pragma warning(disable : 4996)
bool ChangeOut::operator()()
{
	/*
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	
	wcscat(path, L"\\communication.ini");
	wchar_t buf[2048];
	toOut.clear();
	if(GetPrivateProfileString(L"", NULL, L"", buf, 1024, path))
	{
		wchar_t param[128];
		for(wchar_t *s = buf; *s; s += wcslen(s) + 1)
		{
			if(GetPrivateProfileString(L"", s, L"", param, 128, path))
			{
				toOut[_wtoi(s)] = _wtoi(param);
			}
		}
	}
	*/
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\communication.ini");
	toOut.clear();
	FILE *f = _wfopen(path, L"r");
	if(NULL != f)
	{
		wchar_t buf[1024];
		while(NULL != fgetws(buf, 1024, f))
		{
			int key = 0;
			int data = 0;

			wchar_t *s = wcstok(buf, L" ");
			if(NULL != s)
			{
				key = _wtoi(s);
			}
			s = wcstok(NULL, L" ");
			if(NULL != s)
			{
				data = _wtoi(s);
			}
			if(key && data)toOut[key] = data;
		}
		fclose(f);
	}
	return toOut.size() > 0;
}