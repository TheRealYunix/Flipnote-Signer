#pragma once
#include <windows.h>

HWND CreateLabel(HWND hWnd, LPCWSTR text, int x, int y, int width, int height)
{
	return CreateWindowW(L"STATIC", text, WS_VISIBLE | WS_CHILD | SS_LEFT | SS_EDITCONTROL,
		x, y, width, height, hWnd, NULL, NULL, NULL);
}

HWND CreateCheckbox(HWND hWnd, LPCWSTR text, int x, int y, int width, int height, int id)
{
	return CreateWindowW(L"button", text, BS_CHECKBOX | WS_VISIBLE | WS_CHILD | BST_CHECKED, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
}

HWND CreateEditBox(HWND hWnd, int x, int y, int width, int height, int limit = 0)
{
	HWND hEdit = CreateWindowW(L"edit", L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, x, y, width, height, hWnd, 0, NULL, NULL);
	if (limit > 0)
		SendMessageW(hEdit, EM_SETLIMITTEXT, limit, 0);
	return hEdit;
}

HWND CreateNumEditBox(HWND hWnd, int x, int y, int width, int height, int limit = 0)
{
	HWND hEdit = CreateWindowW(L"edit", L"", WS_TABSTOP | ES_NUMBER | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, x, y, width, height, hWnd, 0, NULL, NULL);
	if (limit > 0)
		SendMessageW(hEdit, EM_SETLIMITTEXT, limit, 0);
	return hEdit;
}

HWND CreateButton(HWND hWnd, LPCWSTR text, int x, int y, int width, int height, int id)
{
	return CreateWindowW(L"button", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
}

std::string openFileDialog(HWND hwndOwner)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndOwner;
	ofn.lpstrFilter = "PPM Files (*.ppm)\0*.ppm\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "ppm";

	if (GetOpenFileName(&ofn)) {
		return ofn.lpstrFile;
	}

	return "";
}