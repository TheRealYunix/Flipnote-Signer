#pragma once
#include <Windows.h>

#include "defines.h"
#include "Strings.h"

#include "FlipnoteThumbnail.h"
#include "SaveFileBox.h"

extern PPM *ppm;


LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case ButtonCheck:
		{			
			if (!ppm->get_isOpen())
			{
				MessageBox(hMainWindow, "You have to load a file first!", "Error", MB_ICONERROR);
				break;
			}

			if (!Utils::fileExists(ppm->get_filepath()))
			{
				MessageBox(hMainWindow, "An error occurred. The file was moved or deleted. This program will now close.", "Error", MB_ICONERROR);
				exit(1);
			}
				if (RSA::CheckSign(ppm->get_filepath().c_str()))
					MessageBox(hMainWindow, "This Flipnote is already signed.", "Result", MB_ICONINFORMATION);
				else 
					MessageBox(hMainWindow, "This Flipnote is not signed.", "Result", MB_ICONWARNING);
		break;
		}

		case SaveFileBox:
		{
			if (!ppm->get_isOpen())
			{
				MessageBox(hMainWindow, "You have to load a file first!", "Error", MB_ICONERROR);
				break;
			}

			if(!Utils::fileExists(ppm->get_filepath()))
			{
				MessageBox(hMainWindow, "An error occurred. The file was moved or deleted. This program will now close.", "Error", MB_ICONERROR);
				exit(1);
			}

			bool isLocked = SendMessage(IsLocked, BM_GETCHECK, 0, 0) == BST_CHECKED;
			ppm->set_is_locked(isLocked);


			//Saving Creator names:
			wchar_t creator_name[12];
			GetWindowTextW(hCreator, creator_name, 12);
			ppm->set_creator_name(creator_name);

			wchar_t original_creator_name[12];
			GetWindowTextW(hOriginalCreator, original_creator_name, 12);
			ppm->set_original_creator_name(original_creator_name);

			char authorID[17];
			GetWindowText(hAuthorID, authorID, 17);
			if (!Utils::ValidateAuthorIDs(hMainWindow, authorID, "AuthorID"))
				break;

			ppm->set_author_id(convert::string_to_author_ids(authorID));

			char OriginalauthorID[17];
			GetWindowText(hOriginalAuthorID, OriginalauthorID, 17);
			if (!Utils::ValidateAuthorIDs(hMainWindow, OriginalauthorID, "Original AuthorID"))
				break;
			ppm->set_original_author_id(convert::string_to_author_ids(OriginalauthorID));


			wchar_t szDay[3], szMonth[3], szYear[5], szHour[3], szMin[3];

			GetWindowTextW(hDayTimestamp, szDay, 3);
			if (!Utils::ValidateInput(hMainWindow, szDay, 1, 31, "The entered day is invalid. Please enter a day between 1 and 31."))
				break;
			day = std::stoi(szDay);

			GetWindowTextW(hMonthTimestamp, szMonth, 3);
			if (!Utils::ValidateInput(hMainWindow, szMonth, 1, 12, "The entered month is invalid. Please enter a month between 1 and 12."))
				break;
			month = std::stoi(szMonth);

			GetWindowTextW(hYearTimestamp, szYear, 5);
			if (!Utils::ValidateInput(hMainWindow, szYear, 1900, 2099, "The entered year is invalid. Please enter a year between 1900 and 2100."))
				break;
			year = std::stoi(szYear);

			GetWindowTextW(hHourTimestamp, szHour, 3);
			if (!Utils::ValidateInput(hMainWindow, szHour, 0, 23, "The entered hour is invalid. Please enter an hour between 0 and 23."))
				break;
			hour = std::stoi(szHour);

			GetWindowTextW(hMinTimestamp, szMin, 3);
			if (!Utils::ValidateInput(hMainWindow, szMin, 0, 59, "The entered minute is invalid. Please enter a minute between 0 and 59."))
				break;
			min = std::stoi(szMin);

			uint32_t timestamp = convert::dateTimeToTimestamp(year, month, day, hour, min, sec);
			ppm->set_timestamp(timestamp);

			BROWSEINFO bi = { 0 };
			bi.lpszTitle = "Select a Folder";
			bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON | BIF_EDITBOX;
			if (!hSaveDialog)
			{
				LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
				if (pidl != 0) {
					WCHAR path[MAX_PATH];


					SHGetPathFromIDListW(pidl, path);
					SavePath = path;

					//Opening the PPM Save file dialog
					PPM_SaveFileBox();


					IMalloc *imalloc = 0;
					if (SUCCEEDED(SHGetMalloc(&imalloc))) {
						imalloc->Free(pidl);
						imalloc->Release();
					}
				}
			}

			break;
		}

		case Menu_ShowThumbnail:
		{
			if (!ppm->get_isOpen())
			{
				MessageBox(hMainWindow, "You have to load a file first!", "Error", MB_ICONERROR);
				break;
			}

			if (!hThumbnailWindow)
			{
				Open_ThumbnailWindow();
			}

		}
			break;
		case OpenFileBox:
		{
			std::string filePath = openFileDialog(hMainWindow);
			if (!filePath.empty()) 
			{
				delete ppm;  
				ppm = new PPM(); 

				ppm->open(filePath.c_str());
				if (!ppm->get_is_valid()) {
					delete ppm;  
					ppm = nullptr; 
					break;
				}


				
				mac_sec = ppm->get_mac_filename();
				rnd_sec = ppm->get_rnd_filename();
				num_sec = ppm->get_num_filename();

				std::cout << "-Filename-system-" << std::endl;
				std::cout << "MAC: " << mac_sec << std::endl;
				std::cout << "RND: " << rnd_sec << std::endl;
				std::cout << "NUM: " << num_sec << std::endl;

				//Close the thumbnail window if its open
				if (hThumbnailWindow)
				{
					DestroyWindow(hThumbnailWindow);
					hThumbnailWindow = nullptr;
				}
				//Same for the save dialog
				if (hSaveDialog)
				{
					DestroyWindow(hSaveDialog);
					hSaveDialog = nullptr;
				}
				
				convert::timestampToDateTime(ppm->get_timestamp(), day, month, year, hour, min, sec);


				SetWindowTextW(hDayTimestamp, std::to_wstring(day).c_str());
				SetWindowTextW(hMonthTimestamp, std::to_wstring(month).c_str());
				SetWindowTextW(hYearTimestamp, std::to_wstring(year).c_str());
				SetWindowTextW(hHourTimestamp, std::to_wstring(hour).c_str());
				SetWindowTextW(hMinTimestamp, std::to_wstring(min).c_str());


				SendMessage(IsLocked, BM_SETCHECK, ppm->get_is_locked(), 0);


				SetWindowTextW(hCreator, ppm->get_creator_name().c_str());
				SetWindowTextW(hOriginalCreator, ppm->get_original_creator_name().c_str());


				SetWindowTextW(hAuthorID, convert::author_ids_to_wstring(ppm->get_author_id()).c_str());
				SetWindowTextW(hOriginalAuthorID, convert::author_ids_to_wstring(ppm->get_original_author_id()).c_str());

				std::cout << "Filename:" << convert::format_filename(filePath.c_str()) << std::endl;
		}

			break;
		}


		case MenuAbout:
			MessageBox(hWnd, VERSION, "About", MB_ICONINFORMATION);
			break;

		case Menu_FlipnoteHelp:
			MessageBox(hWnd, "-Signing-\n\nTo Sign a Flipnote go to (File>Save and Sign)\nIt will automaticly save your Flipnote and Sign it.", "How to use", MB_ICONINFORMATION);
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}