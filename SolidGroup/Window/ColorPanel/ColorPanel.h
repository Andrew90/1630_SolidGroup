#pragma once
#include "message.h"

class ColorPanel
{
	wchar_t buf[256];
	unsigned backColor;
//	Gdiplus::Font font;
	Gdiplus::Bitmap *backScreen;
public:
   HWND hWnd;
	ColorPanel();
	void operator()(TSize &);
	void operator()(TPaint &);

	void SetText(wchar_t *, unsigned);
	void Clear();
};