#pragma once
#include <windows.h>

		
class Window
{
public:
	
	Window (int width, 	int height, const char* title);
	~Window();

	HWND hwnd;
	HDC hdc;
	MSG msg;
	int width;
	int height;
	void gameLoop();
	void show();
	
    HINSTANCE hInstance = GetModuleHandle(NULL);
    LPSTR lpszCmdLine = GetCommandLineA();
    int iCmdShow = SW_SHOWDEFAULT;

private:
	
	// variable declaration
	WNDCLASSEX wndclass;
	TCHAR szAppName[256]; // Change to fixed-size array for title storage

	
	int init();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};
