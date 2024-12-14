#include <windows.h>
#include "resource.h"

class Window
{
    public:
        Window(int width, int height, const char* title);
        ~Window();
        MSG msg;

        
    private:
        int width;
        int height;
        TCHAR szAppName[256];
        WNDCLASSEX wndclass;

        HINSTANCE hInstance = GetModuleHandle(NULL);
        LPSTR lpstrCmdLine = GetCommandLineA();
        int iCmdShow = SW_SHOW;
        HWND hwnd;


        int init(void);

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};