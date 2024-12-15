#pragma once
#include <windows.h>
#include <utils/Logger.h>

class Window
{
    // Code

    public: 
        // Constructor 
        Window(int width, int height, const char* title);
        ~Window();

        Logger log;

        // Public variable declarations
        MSG msg;
        HINSTANCE hInstance= GetModuleHandle(NULL);
        HINSTANCE hPrevInstance;
        LPSTR lpstrCmdLine = GetCommandLineA();    
        int iCmdShow = SW_SHOWDEFAULT;
        
        int width;
        int height;

        
        BOOL bFullscreen = FALSE;
		BOOL isTerminated = FALSE;

        // Function Declaration
        void runGameLoop(void); 

    private:
        // Private variable declaration
        WNDCLASSEX wndclass;
        HWND hwnd;
        TCHAR szAppName[256];
        RECT rect;


        // Function declarations
        int init(void);
        void toggleFullscreen(void);

        // CALLBACK Function
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 
};