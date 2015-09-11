#pragma once

struct SaveDateFile
{
	static void Do(HWND );
};

struct LoadDateFile
{
	static wchar_t path[512];
	static void Do(HWND );
};

void StoreDataFile();