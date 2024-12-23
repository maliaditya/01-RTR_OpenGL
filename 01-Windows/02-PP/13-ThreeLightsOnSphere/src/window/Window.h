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
        
        // Timing variables
        LARGE_INTEGER frequency_;  // Ticks per second
        LARGE_INTEGER startTime_;  // Start time
        LARGE_INTEGER currentTime_; // Current time


        HDC hdc;
        HGLRC hrc;
        
        int width;
        int height;
        char keydown;

        int frameCount = 0;      // Frame counter
        float fps = 0.0f; 

        double xpos;
        double ypos;

        float clientX;
        float clientY;
        
        BOOL bFullscreen = FALSE;
		BOOL isTerminated = FALSE;
        BOOL isActive = FALSE;

        // Function Declaration
        void runGameLoop(void); 
        void setResizeCallback(void (*callback)(int, int)); // To register a resize callback
        void setMouseMoveCallback(void(*callback)(float,float));
        void setKeyDownCallback(void(*callback)(char));
        void setMouseLButtonDownCallback(void(*callback)(double,double));
        void setMouseLButtonUPCallback(void(*callback)(double,double));
        float calculateFPS(void);

        
    private:

        // Private variable declaration
        WNDCLASSEX wndclass;
        HWND hwnd;
        TCHAR szAppName[256];
        RECT rect; 
        
        void (*resizeCallback)(int, int) = nullptr; // Resize callback function pointer
        void (*mousemoveCallback)(float, float) = nullptr;
        void (*keydownCallback)(char) = nullptr;
        void (*mouseLButtonDownCallback)(double,double) = nullptr;
        void (*mouseLButtonUPCallback)(double,double) = nullptr;

        // Function declarations
        int init(void);
        int setPixelFormat(void);
        void toggleFullscreen(void);
        void initializeTimer(void);  

        // CALLBACK Function
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 
};