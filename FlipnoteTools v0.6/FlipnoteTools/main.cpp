//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <windows.h>
#include <cassert>
#include <string>
#include <iomanip>
#include <shlobj.h>

#include "GUIHelpers.hpp"
#include "Utils.h"
#include "PPM.hpp"
#include "RSA.hpp"
#include "Font.hpp"
#include "string"

#include "MessageHandler.hpp"
#include "commandline.h"
#include "defines.h"

//#define VERSION "Flipnote Tool v1.0 by Yunix"
//#define VERSION "Flipnote Tool Beta Test Version. [0.9]"
//#define PROG_NAME "Flipnote Tool"
//#define PROG_NAME "Flipnote Tool Beta Release"

PPM *ppm = new PPM;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	//Check if the program is opened with arguments
	if (__argc > 1)
		command_line();

	WNDCLASS winw;
	MSG msg;

	HMENU hMenu = CreateMenu();
	HMENU hFile = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
	AppendMenu(hFile, MF_STRING, OpenFileBox, "Load");
	AppendMenu(hFile, MF_STRING, SaveFileBox, "Save and Sign");

	AppendMenu(hMenu, MF_POPUP, MenuAbout, "About");
	//AppendMenu(hMenu, MF_POPUP, Menu_FlipnoteHelp, "Help");
	AppendMenu(hMenu, MF_POPUP, Menu_ShowThumbnail, "Show Thumbnail");

	winw = {};
	winw.style = CS_HREDRAW | CS_VREDRAW;
	winw.lpfnWndProc = MessageHandler;
	//HINSTANCE hInstance = GetModuleHandle(NULL); //for debugging reasons
	winw.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winw.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	winw.lpszClassName = "CPPWINDOWZT";
	assert(RegisterClass(&winw));


	hMainWindow = CreateWindow(winw.lpszClassName, PROGRAM_NAME, WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, 0, 0, winw.hInstance, 0);

	//create labels
	CreateLabel(hMainWindow, L"Options\n----------------", 20, 10, 260, 150);
	CreateLabel(hMainWindow, L"PPM Options\n---------------------------------------", 300, 10, 260, 150);
	CreateLabel(hMainWindow, L"Creator:", 20, 65, 100, 25);
	CreateLabel(hMainWindow, L"Original creator:", 20, 90, 200, 25);
	CreateLabel(hMainWindow, L"-----------------------------------------------Advanced------------------------------------------------", 20, 120, 500, 25);
	CreateLabel(hMainWindow, L"AuthorID", 20, 135, 200, 15);
	CreateLabel(hMainWindow, L"Original AuthorID", 20, 173, 200, 20);
	CreateLabel(hMainWindow, L"DD  /  MM  /  YY", 230, 173, 200, 20);
	CreateLabel(hMainWindow, L"Time", 370, 173, 200, 20);
	CreateLabel(hMainWindow, L":", 400, 190, 200, 20);

	//create checkboxes
	IsLocked = CreateCheckbox(hMainWindow, L"Locked ", 20, 40, 165, 25, LockCheckbox);

	//edit boxes
	hCreator = CreateEditBox(hMainWindow, 125, 65, 150, 20, 10);
	hOriginalCreator = CreateEditBox(hMainWindow, 125, 90, 150, 20, 10);
	hAuthorID = CreateEditBox(hMainWindow, 20, 150, 150, 20, 16);
	hOriginalAuthorID = CreateEditBox(hMainWindow, 20, 190, 150, 20, 16);

	//Font for the Creator names because some characters are not supported
	Font myFont(IDR_MSEDITED1, L"Perfect DOS VGA 437 Edited");
	HFONT hCustomFont = myFont.GetHFont();
	SendMessageW(hCreator, WM_SETFONT, (WPARAM)hCustomFont, TRUE);
	SendMessageW(hOriginalCreator, WM_SETFONT, (WPARAM)hCustomFont, TRUE);

	CreateLabel(hMainWindow, L"/", 260, 190, 10, 20);
	CreateLabel(hMainWindow, L"/", 300, 190, 10, 20);
	hDayTimestamp = CreateEditBox(hMainWindow, 230, 190, 25, 20, 2);
	hMonthTimestamp = CreateEditBox(hMainWindow, 270, 190, 25, 20, 2);
	hYearTimestamp = CreateEditBox(hMainWindow, 310, 190, 40, 20, 4);
	hHourTimestamp = CreateEditBox(hMainWindow, 370, 190, 25, 20, 2);
	hMinTimestamp = CreateEditBox(hMainWindow, 410, 190, 25, 20, 2);

	CreateButton(hMainWindow, L"Check signature", 300, 40, 150, 25, ButtonCheck);

	SetMenu(hMainWindow, hMenu);
	UpdateWindow(hMainWindow);


	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}