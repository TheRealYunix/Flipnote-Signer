//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <windows.h>
#include <cassert>
#include <iostream>
#include <string>
#include <codecvt>

#include "resource1.h"
#include "File.hpp"
#include "Flipnote.hpp"
#include "types.h"
#include <filesystem>


char TitleOfPPM[256];
char PPMFilepath[1024];
bool Loaded = false;

bool FlipnoteIsLocked = false;
HWND IsLocked;


char OriginalCreator[10];
char Creator[10];
HWND hOriginalCreator;
HWND hCreator;


char AuthorID[17];
char OriginalAuthorID[17];
HWND hAuthorID;
HWND hOriginalAuthorID;

HWND hHexFilename; char HexFilename[7];
HWND hppmFilename; char ppmFilename[13];
HWND hPartialFilename; char PartialFilename[4];

LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		

		if (LOWORD(wParam) == LockCheckbox)
		{
			FlipnoteIsLocked = !FlipnoteIsLocked;
		}
		if (LOWORD(wParam) == SaveFileBox)
		{


			std::string combinedstr = PPMFilepath;
			std::reverse(combinedstr.begin(), combinedstr.end());
			size_t pos = combinedstr.find('\\');
			if (pos != std::string::npos)
				combinedstr.erase(0, pos); //str.size() is not needed!
			std::reverse(combinedstr.begin(), combinedstr.end());

			combinedstr += PPM::Name::Filename(PPMFilepath) + ".ppm";
			std::cout << combinedstr << std::endl;
			std::string info = "Successfully saved to " + combinedstr;
			if (!CopyFile(PPMFilepath, combinedstr.c_str(), NULL))
				MessageBox(nullptr, "An error occurred while saving the file", "Error", MB_ICONERROR);
			else
			{
				MessageBox(nullptr, info.c_str(), "Save", MB_ICONINFORMATION);
			}
		}

		if (LOWORD(wParam) == OpenFileBox)
		{
				//std::string title;
				PPMFilepath[0] = 0;
				OPENFILENAME LoadDL = {};
				LoadDL.lStructSize = sizeof(OPENFILENAME);
				LoadDL.lpstrFilter = "Flipnote-Files (.ppm)\0*.ppm;\0All Files\0*.*";
				LoadDL.lpstrCustomFilter = 0;
				LoadDL.nMaxCustFilter = 0;
				LoadDL.nFilterIndex = 0;
				LoadDL.lpstrFile = PPMFilepath;
				LoadDL.nMaxFile = 1024;
				LoadDL.lpstrTitle = "Select a Flipnote-File/PPM to load";
				LoadDL.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				//title = TitleOfPPM;
				
				if (GetOpenFileName(&LoadDL) != 0)
				{
					Loaded = true;


					if (!PPM::ValidPPM(PPMFilepath))
					{
						MessageBox(nullptr, "PPM-File is corrupted!", "Warning", MB_ICONERROR);
					}
						else
						{
							//title.erase(title.size() - 0x4);
							//if (PPM::Filename(PPMFilepath) != title)
								//MessageBox(nullptr, "Filename does not match the binary filename! Flipnote will not sign!", "Warning", MB_ICONERROR);
							//std::cout << test;
							//Check the Lock
							
							SendMessage(IsLocked, BM_SETCHECK, PPM::isLocked(PPMFilepath), 0);

							SetWindowText(hCreator, PPM::CreatorName(PPMFilepath).c_str());
							SetWindowText(hOriginalCreator, PPM::OriginalCreatorName(PPMFilepath).c_str());

							SetWindowText(hAuthorID, PPM::AuthorID(PPMFilepath).c_str());
							SetWindowText(hOriginalAuthorID, PPM::OriginalAuthorID(PPMFilepath).c_str());

							//Loading file name
							SetWindowText(hHexFilename, PPM::Name::Hexname(PPMFilepath).c_str());
							SetWindowText(hppmFilename, PPM::Name::centralname(PPMFilepath).c_str());
							SetWindowText(hPartialFilename, PPM::Name::endname(PPMFilepath).c_str());
						}
				}
			
		}
		if (LOWORD(wParam) == ButtonSign)
		{
			if (!Loaded)
			{
				MessageBox(nullptr, "Please load a PPM file to continue", "Warning", MB_ICONWARNING);
				break;
			}
			//Writing if its locked or not
			File::WriteBinary(PPMFilepath, std::to_string(IsDlgButtonChecked(hWnd, LockCheckbox)), 0x10, std::ios::beg);

			GetWindowText(hCreator, Creator, 0x10);
			PPM::SetCreatorName(PPMFilepath, Creator);
			GetWindowText(hOriginalCreator, OriginalCreator, 0x10);
			PPM::SetOriginalCreatorName(PPMFilepath, OriginalCreator);

			GetWindowText(hOriginalAuthorID, OriginalAuthorID, 17);
			PPM::SetOriginalAuthorID(PPMFilepath, OriginalAuthorID);
			GetWindowText(hAuthorID, AuthorID, 17);
			PPM::SetAuthorID(PPMFilepath, AuthorID);

			GetWindowText(hHexFilename, HexFilename, sizeof(HexFilename));
			GetWindowText(hppmFilename, ppmFilename, sizeof(ppmFilename));
			GetWindowText(hPartialFilename, PartialFilename, sizeof(PartialFilename));
			PPM::Name::Write_Hexname(PPMFilepath, HexFilename);
			PPM::Name::Write_centralname(PPMFilepath, ppmFilename);
			PPM::Name::Write_endname(PPMFilepath, PartialFilename);

			//GetWindowText(hOriginalCreator, OriginalCreator, 0x10);

			bool result = PPM::Sign(PPMFilepath);
			switch (result)
			{
			case true:
				MessageBox(nullptr, "Succesfully signed the PPM-File.", "Result", MB_ICONINFORMATION);
				break;
			case false:
				MessageBox(nullptr, "An error occurred while signing.", "Result", MB_ICONERROR);
				break;
			}
		}

		if (LOWORD(wParam) == ButtonCheck)
		{
			
			if (!Loaded)
			{
				MessageBox(nullptr, "Please load a PPM file to continue", "Warning", MB_ICONWARNING);
				break;
			}
			bool result = PPM::CheckSign(PPMFilepath);
			switch (result)
			{
			case true: MessageBox(nullptr, "This Flipnote is already signed!", "Result", MB_ICONINFORMATION);
				break;
			case false:MessageBox(nullptr, "This Flipnote is not signed.", "Result", MB_ICONWARNING);
				break;
			}
			
		}

		if (LOWORD(wParam) == MenuAbout)
			MessageBox(nullptr, "Flipnote Tool v0.5 by Yunix", "About", MB_ICONINFORMATION);

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main()
{
	HINSTANCE hInstance = GetModuleHandle(0);
	//By default the Console is Hidden.



	HWND hWnd;

	WNDCLASS winw;
	MSG msg;

	HMENU hMenu = CreateMenu();
	HMENU hFile = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
	AppendMenu(hFile, MF_STRING, OpenFileBox, "Load");
	AppendMenu(hFile, MF_STRING, SaveFileBox, "Save");

	AppendMenu(hMenu, MF_POPUP, MenuAbout, "About");



	winw = {};
	winw.style = CS_HREDRAW | CS_HREDRAW;
	winw.lpfnWndProc = MessageHandler;
	winw.hInstance = hInstance;
	winw.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	winw.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winw.lpszClassName = "CPPWINDOWZT";
	assert(RegisterClass(&winw));

	//Window
	hWnd = CreateWindow("CPPWINDOWZT", "Flipnote Tool", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, 0, 0, hInstance, 0);
	
	/*Text fields*/
	HWND OptionsText = CreateWindow("STATIC", "Options\n----------------", WS_VISIBLE | WS_CHILD | NULL, 20, 10, 260, 150, hWnd, NULL, hInstance, NULL);
	HWND SignText = CreateWindow("STATIC", "Sign/PPM Options\n---------------------------------------", WS_VISIBLE | WS_CHILD | NULL, 300, 10, 260, 150, hWnd, NULL, hInstance, NULL);

    IsLocked = CreateWindow("button", "Locked ", BS_CHECKBOX | WS_VISIBLE | WS_CHILD | BST_CHECKED, 20, 40, 165, 25, hWnd, (HMENU)LockCheckbox, hInstance, NULL);
	
	HWND CreatorText = CreateWindow("STATIC", "Creator:", WS_VISIBLE | WS_CHILD | NULL, 20, 65, 100, 25, hWnd, NULL, hInstance, NULL);
	hCreator = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 65, 150, 20, hWnd, 0, 0, 0);
	SendMessage(hCreator, EM_SETLIMITTEXT, 10, 0);

	HWND OriginalCreatorText = CreateWindow("STATIC", "Original creator:", WS_VISIBLE | WS_CHILD | NULL, 20, 90, 200, 25, hWnd, NULL, hInstance, NULL);
	hOriginalCreator = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 90, 150, 20, hWnd, 0, 0, 0);
	SendMessage(hOriginalCreator, EM_SETLIMITTEXT, 10, 0);

	HWND ExtraSection = CreateWindow("STATIC", "-----------------------------------------------Advanced------------------------------------------------", WS_VISIBLE | WS_CHILD | NULL, 20, 120, 500, 25, hWnd, NULL, hInstance, NULL);
	HWND AuthorIDText = CreateWindow("STATIC", "AuthorID", WS_VISIBLE | WS_CHILD | NULL, 20, 135, 200, 25, hWnd, NULL, hInstance, NULL);
	hAuthorID = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 20, 150, 150, 20, hWnd, 0, 0, 0);
	SendMessage(hAuthorID, EM_SETLIMITTEXT, 16, 0);

	HWND OriginalAuthorIDText = CreateWindow("STATIC", "Original AuthorID", WS_VISIBLE | WS_CHILD | NULL, 20, 173, 200, 25, hWnd, NULL, hInstance, NULL);
	hOriginalAuthorID = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 20, 190, 150, 20, hWnd, 0, 0, 0);
	SendMessage(hOriginalAuthorID, EM_SETLIMITTEXT, 16, 0);

	
	HWND ppmFilenameText = CreateWindow("STATIC", "Filename", WS_VISIBLE | WS_CHILD | NULL, 230, 173, 200, 25, hWnd, NULL, hInstance, NULL);
	hHexFilename = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 230, 190, 60, 20, hWnd, 0, 0, 0);
	hppmFilename = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 300, 190, 120, 20, hWnd, 0, 0, 0);
	hPartialFilename = CreateWindow("edit", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 430, 190, 30, 20, hWnd, 0, 0, 0);
	SendMessage(hHexFilename, EM_SETLIMITTEXT, 6, 0);
	SendMessage(hppmFilename, EM_SETLIMITTEXT, 13, 0);
	SendMessage(hPartialFilename, EM_SETLIMITTEXT, 3, 0);
	
	/*Button's*/
	HWND hSignButton = CreateWindow("button", "Sign", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 40, 75, 25, hWnd, (HMENU)ButtonSign, hInstance, 0);
	HWND hCheckButton = CreateWindow("button", "Check", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 380, 40, 75, 25, hWnd, (HMENU)ButtonCheck, hInstance, 0);
	SetMenu(hWnd, hMenu);

	//Main loop
	while (true) {

		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return result;
		}
	}
}