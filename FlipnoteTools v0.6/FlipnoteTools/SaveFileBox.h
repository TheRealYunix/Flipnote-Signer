#pragma once
#include <windows.h>
#include "Utils.h"
#include "defines.h"
#include "PPM.hpp"
#include "RSA.hpp"
#include "GUIHelpers.hpp"

extern PPM *ppm;



LRESULT CALLBACK SaveFileWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case RandomPPMButton:
		{
			char authorid[17];
			std::string MAC_SEC;
			
			GetWindowText(hAuthorID, authorid, 17);
			MAC_SEC = authorid;

			if (MAC_SEC.length() >= 6)
				MAC_SEC = MAC_SEC.substr(MAC_SEC.length() - 6);


			static bool seeded = false;
			if (!seeded) {
				srand(static_cast<unsigned int>(time(nullptr)));
				seeded = true;
			}

			// Generate the random PPM Filename
			std::wstring RND_SEC(13, L' ');
			std::wstring NUM_SEC(3, L' ');
			for (int i = 0; i < 13; i++) {
				RND_SEC[i] = std::to_wstring(rand() % 10)[0];
			}
			for (int i = 0; i < 3; i++) {
				NUM_SEC[i] = std::to_wstring(rand() % 5)[0];
			}

			std::string filename = MAC_SEC + convert::wstring_to_string(L"_" + RND_SEC + L"_" + NUM_SEC);
			std::cout << "Checksum: " << ppm->calc_check_digit(filename) << std::endl;
			MAC_SEC[0] = ppm->calc_check_digit(filename);

			SetWindowText(FN_MAC_SEC, MAC_SEC.c_str());
			SetWindowTextW(FN_RND_SEC, RND_SEC.c_str());
			SetWindowTextW(FN_NUM_SEC, NUM_SEC.c_str());

			break;
		}

		case SavePPMButton:
		{
			char szMac[7], szRnd[14], szNum[4];

			
			GetWindowText(FN_MAC_SEC, szMac, 7);
			GetWindowText(FN_RND_SEC, szRnd, 14);
			GetWindowText(FN_NUM_SEC, szNum, 4);







			std::string mac(szMac);	std::string rnd(szRnd);	std::string num(szNum);
			
			if (mac.size() < 6 || !convert::IsHexadecimal(mac.substr(1)))
			{
				MessageBox(nullptr, "The MAC section must contain hexadecimal digits only, except the first Letter!", "Error", MB_OK | MB_ICONERROR);
				break;
			}

			std::string filename = std::string(mac) + "_" + std::string(rnd) + "_" + std::string(num) + ".ppm";
			std::string path;

			std::cout << path << std::endl;
			
			//Calculating the checksum:
			std::cout << "MAC SEC: " << mac <<std::endl;
			/*
			char checksum = ppm->calc_check_digit(convert::wstring_to_string(filename));

			// Überprüfung der Prüfziffer
			if (checksum == -1) {
				// Ungültige Eingabe, Zeige eine Fehlermeldung an
				MessageBox(nullptr, "Invalid Filename! Try randomizing the filename", "Error", MB_OK | MB_ICONERROR);
				break;
			}
			*/
			
			path = convert::wstring_to_string(SavePath) + '\\' + filename;

			std::wcout << L"Name string is (WSTRING): " << szRnd << std::endl;

			//Saving File name to the binary hex of the file
			//std::string a = ppm->get_mac_filename().at(1) + convert::wstring_to_string(mac);




			char authorid[17];
			GetWindowText(hAuthorID, authorid, 17);


			std::string mac_file_section = mac;
			mac_file_section[0] = authorid[10];
			std::cout << "MAC_SECTION FILENAME: " << mac_file_section << std::endl;
			ppm->set_mac_filename(mac_file_section);
			if (!ppm->set_rnd_filename(rnd))
			{
				MessageBox(nullptr, "Please Enter a valid filename!", "Error", MB_ICONERROR);
				break;
			}
			if (!ppm->set_num_filename(num))
			{
				MessageBox(nullptr, "Please Enter a valid filename!", "Error", MB_ICONERROR);
				break;
			}

			if (Utils::fileExists(path))
			{
				int response = MessageBox(nullptr, "The file already exists. Do you want to proceed?", "File Exists", MB_OKCANCEL | MB_ICONINFORMATION);
				if (response == IDOK) {
					std::cout << "Save Path: " << path.c_str() << std::endl;
					ppm->save(path.c_str());
					RSA::Sign(path.c_str());
				}
			}
			else
			{
				std::cout << "Save Path: " << path.c_str() << std::endl;
				ppm->save(path.c_str());
				RSA::Sign(path.c_str());
			}
			DestroyWindow(hSaveDialog);
			break;
		}
		}
		break;
	case WM_DESTROY:
		EnableWindow(hMainWindow, TRUE);
		hSaveDialog = nullptr;
		CloseWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void PPM_SaveFileBox()
{
	EnableWindow(hMainWindow, FALSE);
	WNDCLASS winw;
	winw = {};
	winw.style = CS_HREDRAW | CS_VREDRAW;
	winw.lpfnWndProc = SaveFileWinProc; 
	winw.hInstance = GetModuleHandle(NULL);
	winw.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	winw.lpszClassName = "CPPWINDOWZTD";
	RegisterClass(&winw);

	hSaveDialog = CreateWindow(winw.lpszClassName, "Save Flipnote(PPM)", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 355, 200, hMainWindow, 0, winw.hInstance, 0);

	CreateLabel(hSaveDialog, L"Enter a valid Flipnote(PPM) name", 50, 25, 250, 25);
	
	//GUI Stuff:
	FN_MAC_SEC = CreateEditBox(hSaveDialog, 50, 50, 55, 20, 6);
	CreateLabel(hSaveDialog, L"_", 106, 54, 10, 25);
	FN_RND_SEC = CreateEditBox(hSaveDialog, 115, 50, 110, 20, 13);
	CreateLabel(hSaveDialog, L"_", 226, 54, 10, 25);
	FN_NUM_SEC = CreateEditBox(hSaveDialog, 235, 50, 30, 20, 3);
	CreateLabel(hSaveDialog, L".ppm", 266, 54, 35, 25);
	CreateButton(hSaveDialog, L"Save", 217, 80, 50, 25, SavePPMButton);
	CreateButton(hSaveDialog, L"Randomize name", 50, 80, 125, 25, RandomPPMButton);

	char num_str[4];
	sprintf_s(num_str, "%03d", num_sec);  //format the numbers, filling it with zeros

	//Bug fix when the filedialog was reopened on the same file instance
	mac_sec = ppm->get_mac_filename();

	std::cout << "before checksum: " << mac_sec + "_" + rnd_sec + "_" + num_str << std::endl;
	mac_sec.at(0) = ppm->calc_check_digit(mac_sec + "_" + rnd_sec + "_" + num_str);
	std::cout << "Checksum: " << ppm->calc_check_digit(mac_sec + "_" + rnd_sec + "_" + num_str) << std::endl;
	//std::cout << "The full filename is: " << filename << std::endl;
	std::cout << "The Mac file section is: " << mac_sec << std::endl;

	SetWindowText(FN_MAC_SEC, mac_sec.c_str());
	SetWindowText(FN_RND_SEC, rnd_sec.c_str());
	SetWindowText(FN_NUM_SEC, num_str);
}