#include "stdafx.h"
#include "SaveLoadDates.h"
#include <stdio.h>
#include "DlgFiles.h"
#include "SolidData.h"
#include <ShlWapi.h>
#include <time.h>
#include "Corel.h"
#include "StoredData.h"

#ifdef XDEBUG
#define xprint debug.print(__FUNCTION__);
#define dprint debug.print
#define d_mess(x)
#define x_mess debug.print
#else
#define xprint
#define dprint
#endif

struct close_file
{
	FILE *f;
	close_file(FILE *f) : f(f){}
	~close_file(){if(NULL != f)fclose(f);}
};
#pragma warning(disable : 4996)
void SaveDateFile::Do(HWND h)
{
	SaveData o(h);
	bool b = false;
	if(solidData.currentOffset > 0 && o())
	{
		FILE *f= _wfopen(o.sFile, L"wb+");
		close_file c_f(f);
		if(NULL != f)
		{
			b = fwrite(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f)
		     && fwrite(solidData.referenceBuffer, sizeof(double) * solidData.currentOffset, 1, f)
			 && fwrite(&solidData.dataBuffer, sizeof(double) * solidData.currentOffset, 1, f)
			 ;
		}
	}
	if(!b)
	{
		MessageBox(0, L"Файл не сохранён", L"Ошибка !!!", MB_ICONERROR);
	}
}
wchar_t LoadDateFile::path[512];
void LoadDateFile::Do(HWND h)
{
	OpenData o(h);
	bool b = false;
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"rb");
		close_file c_f(f);
		if(NULL != f)
		{
			int len = wcslen(o.sFile);
			wchar_t *s = o.sFile;
			for(int i = len; i > 0; --i)
			{
				if(s[i] == '\\' || s[i] == '/')
				{
					wcscpy(path, &s[i+1]);
					break;
				}
			}
			if(fread(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f))
			{
				if(solidData.currentOffset < SolidData::MAX_ZONES_COUNT)
				{
					b = fread(solidData.referenceBuffer, sizeof(double) * solidData.currentOffset, 1, f)
						&& fread(&solidData.dataBuffer, sizeof(double) * solidData.currentOffset, 1, f)
						;
				}
			}
		}
		if(!b)
		{
			MessageBox(0, L"Файл не загружен", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}


void StoreDataFile()
{
	wchar_t path[1024];
	 GetModuleFileName(0, path, 1024);
	 PathRemoveFileSpec(path);
	 wcscat(path, L"\\Stored\\");
	 CreateDirectory(path, NULL);
	 time_t t = time(0);
	 struct tm *now = localtime( & t );
#pragma message("изменил формирование названия файла")
	 //
	 wchar_t *nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	 int numberTube = StoredData::Instance().Current();
	 wsprintf(&path[wcslen(path)], L"%06d_%s_%s_%02d%02d%02d_%02d%02d%02d.dat"
	     , numberTube
		 , nameParam
		 , corel.ClassTubeName()
		 , now->tm_year - 100
		 , now->tm_mon + 1
		 , now->tm_mday
		 , now->tm_hour
		 , now->tm_min
		 , now->tm_sec
		 );
	FILE *f= _wfopen(path, L"wb+");
	close_file c_f(f);
	if(NULL != f)
	{
		fwrite(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f)
			&& fwrite(solidData.referenceBuffer, sizeof(double) * solidData.currentOffset, 1, f)
			&& fwrite(&solidData.dataBuffer, sizeof(double) * solidData.currentOffset, 1, f)
			;
	}
}