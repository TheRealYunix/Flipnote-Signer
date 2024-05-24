#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "PPM.hpp"
#include "resource.h"

#define THUMBNAIL_SCALE 5

extern PPM *ppm;

const COLORREF palette[] =
{
	RGB(255, 255, 255),
	RGB(82, 82, 82),   
	RGB(255, 255, 255),
	RGB(156, 156, 156),
	RGB(255, 72, 68),  
	RGB(200, 81, 79),  
	RGB(255, 173, 172),
	RGB(0, 255, 0),    
	RGB(72, 64, 255),  
	RGB(81, 79, 184),  
	RGB(173, 171, 255),
	RGB(0, 255, 0),    
	RGB(182, 87, 183), 
	RGB(0, 255, 0),    
	RGB(0, 255, 0),    
	RGB(0, 255, 0)     
};

void ReadThumbnailData(std::vector<std::vector<int>>& image)
{
	std::ifstream file(ppm->get_filepath(), std::ios::binary);
	if (!file) {
		exit(1);
	}

	file.seekg(0xA0);

	std::vector<uint8_t> data(1536);
	file.read(reinterpret_cast<char*>(data.data()), data.size());

	int data_offset = 0;
	for (int tile_y = 0; tile_y < 48; tile_y += 8) {
		for (int tile_x = 0; tile_x < 64; tile_x += 8) {
			for (int line = 0; line < 8; line++) {
				for (int pixel = 0; pixel < 8; pixel += 2) {
					int x = tile_x + pixel;
					int y = tile_y + line;
					image[y][x] = data[data_offset] & 0x0F;
					image[y][x + 1] = (data[data_offset] >> 4) & 0x0F;
					data_offset++;
				}
			}
		}
	}
}
bool g_WindowOpen = false;
LRESULT CALLBACK ThumbnailWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrushes[16];

	switch (message)
	{
	case WM_CREATE:
		if (!Utils::fileExists(ppm->get_filepath()))
		{
			int result = MessageBox(hMainWindow, "An error occurred. Do you want to close the program?", "Error", MB_ICONERROR);
			exit(1);
		}

		for (int i = 0; i < 16; ++i) {
			hBrushes[i] = CreateSolidBrush(palette[i]);
		}
		return 0;
	case WM_PAINT:
	{
		std::vector<std::vector<int>> image(48, std::vector<int>(64));
		ReadThumbnailData(image);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		for (int y = 0; y < 48; ++y) {
			for (int x = 0; x < 64; ++x) {
				int colorIndex = image[y][x];
				HBRUSH hBrush = hBrushes[colorIndex];

				RECT rect = { x * 5, y * 5, (x + 1) * 5, (y + 1) * 5 };
				FillRect(hdc, &rect, hBrush);
			}
		}

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		for (int i = 0; i < 16; ++i) {
			DeleteObject(hBrushes[i]);
		}
		hThumbnailWindow = nullptr;
		CloseWindow(hWnd); 
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Open_ThumbnailWindow()
{
	WNDCLASS winw2;
	winw2 = {};
	winw2.style = CS_HREDRAW | CS_VREDRAW;
	winw2.lpfnWndProc = ThumbnailWinProc;
	winw2.hInstance = GetModuleHandle(0);
	winw2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	winw2.hIcon = LoadIcon(winw2.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winw2.lpszClassName = "CPPWINDOWZTDT";
	RegisterClass(&winw2);

	hThumbnailWindow = CreateWindow(winw2.lpszClassName, "Thumbnail", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 336, 279, hMainWindow, 0, winw2.hInstance, 0);
}