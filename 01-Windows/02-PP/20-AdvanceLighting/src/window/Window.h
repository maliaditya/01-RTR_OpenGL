#pragma once
#include <windows.h>
// ImGUI Headers
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <utils/Logger.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWhwnnd, UINT msg, WPARAM wParam, LPARAM lParam);


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
        HWND hwnd;
        RECT rect; 

        
        int width;
        int height;
        int delta;
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
        void setMouseWheelCallback(void(*callback)(int, int, int) );
        float calculateFPS(void);
        void triggerResize(HWND hwnd);

        
    private:

        // Private variable declaration
        WNDCLASSEX wndclass;
        TCHAR szAppName[256];
        
        void (*resizeCallback)(int, int) = nullptr; // Resize callback function pointer
        void (*mousemoveCallback)(float, float) = nullptr;
        void (*keydownCallback)(char) = nullptr;
        void (*mouseLButtonDownCallback)(double,double) = nullptr;
        void (*mouseLButtonUPCallback)(double,double) = nullptr;
        void (*mousewheelCallback)(int, int, int) = nullptr;

        // Function declarations
        int init(void);
        int setPixelFormat(void);
        void toggleFullscreen(void);
        void initializeTimer(void);  

        // CALLBACK Function
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 
};