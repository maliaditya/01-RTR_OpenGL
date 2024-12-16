#include <windows.h>
#include "window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstrCmdLine, int iCmdShow)
{
    // Code
    
    // Create Window
    Window window(800,600, "Window - Fullscreen.");

    window.hInstance = hInstance;
    window.hPrevInstance = hPrevInstance;
    window.lpstrCmdLine = lpstrCmdLine;
    window.iCmdShow = iCmdShow;


   while(!window.isTerminated)
   {
    
    window.runGameLoop();
   }

    return(0);
}