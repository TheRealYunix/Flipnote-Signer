#pragma once
#include "Font.hpp"

Font::Font(const uint16_t IDR_FONT, const WCHAR *fontname)
{
	// Deaktiviere vorhandene Schriftart mit demselben Namen, falls vorhanden
	RemoveFontResourceExW(fontname, FR_PRIVATE, nullptr);

	HANDLE hMyFont = INVALID_HANDLE_VALUE;
	HINSTANCE hInstance = GetModuleHandle(0);
	HRSRC  hFntRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_FONT), "BINARY");
	if (!hFntRes) return;
	HGLOBAL hFntMem = LoadResource(hInstance, hFntRes);
	if (hFntMem)
	{
		void* FntData = LockResource(hFntMem);
		DWORD nFonts = 0, len = SizeofResource(hInstance, hFntRes);
		hMyFont = AddFontMemResourceEx(FntData, len, nullptr, &nFonts);

		LOGFONTW MyLogFont = { 0, 0, 0, 0, 100,
			FALSE, FALSE, FALSE, 0,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY,
			VARIABLE_PITCH | FF_SWISS, L"" };
		wcscpy_s(MyLogFont.lfFaceName, LF_FACESIZE, fontname);
		this->hFont = CreateFontIndirectW(&MyLogFont);
	}
}

Font::~Font()
{
	if (hFont != NULL)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}
}

HFONT Font::GetHFont() const
{
	return hFont;
}
