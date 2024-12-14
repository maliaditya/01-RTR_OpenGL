// Header Files
#include <windows.h>
#include "window/Window.h"

#define WINWIDTH 800
#define WINHEIGHT 600

// Entry Point Function
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int iCmdShow)
{
	// code
    Window window(WINWIDTH, WINHEIGHT, "OGL window");
	return((int)window.msg.wParam);
}
