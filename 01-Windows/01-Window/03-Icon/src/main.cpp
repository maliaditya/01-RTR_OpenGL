#include <windows.h>
#include "window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstnace, LPSTR lpstrCmdLine, int iCmdShow)
{
    Window window(800,600,"Window - Icon...!");
    return((int)window.msg.wParam);
}