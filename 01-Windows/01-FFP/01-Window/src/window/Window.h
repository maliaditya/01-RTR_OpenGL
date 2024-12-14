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
	
	// Get the current instance handle
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Command-line arguments and show command
    LPSTR lpszCmdLine = GetCommandLineA();
    int iCmdShow = SW_SHOWDEFAULT;

private:
	// variable declaration
	WNDCLASSEX wndclass;
	TCHAR szAppName[256]; // Change to fixed-size array for title storage
	RECT rc;
	BOOL bFullscreen = FALSE;
	BOOL bActiveWindow = TRUE;
	BOOL bDone = FALSE;
	
	int init(HINSTANCE hInstance,int iCmdShow);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};
