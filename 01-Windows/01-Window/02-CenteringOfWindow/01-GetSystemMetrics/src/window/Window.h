#include <windows.h>

class Window
{
    public:
        Window(int width, int height, const char* title);
        ~Window();
        MSG msg;
        HWND hwnd;
        int width;
        int height;


    private:
        WNDCLASSEX  wndclass;
        TCHAR szAppName[256];
        HINSTANCE hInstance = GetModuleHandle(NULL);
        int iCmdShow = SW_SHOWDEFAULT;
        
        RECT rc; // sentering the window
        
        
        int init(void);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};