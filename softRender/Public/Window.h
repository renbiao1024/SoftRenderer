#pragma once
#include <windows.h>

class Window
{
public:
	HWND window;//窗口句柄
	int windowWidth;
	int windowHeight;
	HDC hdc;//设备描述表句柄
	HDC screenHDC;
	Window(int w, int h, const char*name);
	~Window();
};