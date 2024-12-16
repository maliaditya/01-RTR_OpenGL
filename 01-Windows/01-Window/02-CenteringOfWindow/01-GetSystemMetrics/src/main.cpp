#include <windows.h>
#include "window/Window.h"

#define WINWIDTH 800
#define WINHEIGHT 600

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    Window window(WINWIDTH, WINHEIGHT, "OGL window..!");
    return((int)window.msg.wParam);
}
