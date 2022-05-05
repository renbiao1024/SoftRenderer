#include "Window.h"
#include <iostream>
using namespace std;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)//参数:窗口句柄，消息，消息参数，消息参数
{
	Window* window = static_cast<Window*>(GetPropA(hWnd,"Owner"));
	if(!window)
		return DefWindowProcA(hWnd,msg,wParam,lParam);

	switch(msg){}

	return DefWindowProcA(hWnd,msg,wParam,lParam);
}

Window::Window(int w, int h, const char* name) :windowWidth(w), windowHeight(h)
{
	WNDCLASS wndClass = {CS_BYTEALIGNCLIENT,(WNDPROC)MsgProc,0,0,0,NULL,NULL,NULL,NULL,TEXT("softRender")};
	wndClass.hInstance = GetModuleHandle(NULL);

	if(!RegisterClass(&wndClass)) return;

	window = CreateWindow(TEXT("softRender"), TEXT("softRender"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wndClass.hInstance, NULL);
	if (window == NULL) return;

	hdc = GetDC((window));
	screenHDC = CreateCompatibleDC(hdc);

	BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER),windowWidth, windowHeight, 1, 32, BI_RGB, windowWidth * windowHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	//创建设备无关的位图
	HBITMAP bitmapHandler = CreateDIBSection(screenHDC, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	if (bitmapHandler == NULL)
		return;

	HBITMAP screenObject = (HBITMAP)SelectObject(screenHDC, bitmapHandler);

	SetWindowPos(window, NULL, 50, 50, windowWidth, windowHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	ShowWindow(window, SW_NORMAL);
	UpdateWindow(window);
}

Window::~Window(){}