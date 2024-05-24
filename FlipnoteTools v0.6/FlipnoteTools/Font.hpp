#pragma once
#include <iostream>
#include <windows.h>
#include "resource.h"


class Font
{

private:
	HFONT hFont;

public:
	//You have to include the Fonts in the "resourcesfiles" after that you can just enter the ID, fontname and Visual Studio automaticly created for you
	Font(const uint16_t IDR_FONT, const WCHAR *fontname);
	~Font();

	HFONT GetHFont() const;
};