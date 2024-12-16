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

        HDC hdc;
        HGLRC hrc;
        
        int width;
        int height;
        
        BOOL bFullscreen = FALSE;
		BOOL isTerminated = FALSE;
        BOOL isActive = FALSE;

        // Function Declaration
        void runGameLoop(void); 
        void setResizeCallback(void (*callback)(int, int)); // To register a resize callback

    private:

        // Private variable declaration
        WNDCLASSEX wndclass;
        HWND hwnd;
        TCHAR szAppName[256];
        RECT rect; 
        
        void (*resizeCallback)(int, int) = nullptr; // Resize callback function pointer

        // Function declarations
        int init(void);
        int setPixelFormat(void);
        void toggleFullscreen(void);

        // CALLBACK Function
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 
};