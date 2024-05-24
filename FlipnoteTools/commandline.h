#pragma once
#include <Windows.h>
#include <iostream>
#include "RSA.hpp"


void PrintUsage()
{
	std::wcout << L"\n\nFlipnoteTools - A tool for signing Flipnote files\n";
	std::wcout << L"Usage: FlipnoteTools.exe -s <filename>\n\n";
	std::wcout << L"Options:\n";
	std::wcout << L"  -s <filename>   Sign the specified file\n";
}

void command_line()
{
	FILE* pFile;
	int argc = __argc;
	LPWSTR* argv = __wargv;

	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		AllocConsole();
	}

	freopen_s(&pFile, "CONOUT$", "w", stdout);

	if (argc < 3 || std::wstring(argv[1]) != L"-s")
	{
		PrintUsage();
	}
	else
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, argv[2], -1, NULL, 0, NULL, NULL);
		std::string filename_utf8(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, argv[2], -1, &filename_utf8[0], size_needed, NULL, NULL);

		if (RSA::Sign(filename_utf8.c_str()))
		{
			std::wcout << L"File successfully signed!" << std::endl;
		}
		else
		{
			std::wcout << L"Error signing the file!" << std::endl;
		}
	}

	LocalFree(argv);
	exit(0);
}