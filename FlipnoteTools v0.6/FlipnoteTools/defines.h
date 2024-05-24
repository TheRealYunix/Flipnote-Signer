#pragma once
#include <Windows.h>
#include <iostream>


#define LockCheckbox       1001
#define OpenFileBox        1002
#define SaveFileBox        1003
#define ButtonSign         1004
#define ButtonCheck		   1005
#define MenuAbout		   1006
#define Menu_FlipnoteHelp  1007
#define Menu_ShowThumbnail 1008


//Save File Dialog
//HWND FILENAME;
HWND FN_MAC_SEC; //File name MAC-Section
HWND FN_RND_SEC; //File name Random-Section
HWND FN_NUM_SEC; //File name Nnmber of Edits section

std::string mac_sec;
std::string rnd_sec;
uint16_t num_sec;

std::wstring SavePath;
#define SavePPMButton    1009
#define RandomPPMButton  1010

HWND hMainWindow;
HWND hThumbnailWindow;

HWND hSaveDialog;
HWND IsLocked;
HWND hCreator;
HWND hOriginalCreator;
HWND hAuthorID;
HWND hOriginalAuthorID;

HWND hDayTimestamp;
HWND hMonthTimestamp;
HWND hYearTimestamp;
HWND hHourTimestamp;
HWND hMinTimestamp;
HWND hSecTimestamp;
int day, month, year, hour, min, sec;