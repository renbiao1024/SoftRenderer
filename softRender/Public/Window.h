#pragma once
#include <windows.h>

class Window
{
public:
	HWND window;//���ھ��
	int windowWidth;
	int windowHeight;
	HDC hdc;//�豸��������
	HDC screenHDC;
	Window(int w, int h, const char*name);
	~Window();
};