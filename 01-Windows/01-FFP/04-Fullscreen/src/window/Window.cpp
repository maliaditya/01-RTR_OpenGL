#include "Window.h"
#include "resource.h"
#include <iostream>

Window::Window(int width, int height, const char* title): width(width), height(height), log("Window.log")
{
    // Code
    
    // Copy title to szAppname
    strncpy(szAppName, title, sizeof(szAppName)-1);
    szAppName[sizeof(szAppName)-1] = '\0'; 
    
    log.write("Window Creation \n");

    // Create Window
    if(init())
    {
        log.write("Window Creation Failed\n");
    }

}

Window::~Window()
{
    // Code

    if (hwnd) {
            log.write("Destroy Handle");
            DestroyWindow(hwnd); // Destroy the window if it's created
            hwnd = nullptr; // Nullify the handle to avoid dangling pointer
    }

}

int Window::init(void)
{
    // Code

    // Initiazlize WNDCLASSEX struct
    wndclass.cbSize = sizeof(WNDCLASSEX);  // Measure the window
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // Decide its resizing behavior.
    wndclass.cbClsExtra = 0; // No shared storage.
    wndclass.cbWndExtra = 0;  // No unique storage.
    wndclass.lpfnWndProc = WndProc; //Assign a manager.
    wndclass.hInstance = hInstance; //  Choose the instance it belongs to.
    wndclass.hbrBackground =(HBRUSH)GetStockObject(WHITE_BRUSH); // Pick a background color
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // Add a big icon
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // Choose a cursor style.
    wndclass.lpszClassName = szAppName; // Give the class a name.
    wndclass.lpszMenuName = NULL; //Decide if a menu is needed.
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // Add a small icon.

    // Register the window class
    if (!RegisterClassEx(&wndclass)) {
        log.write("Window class registration failed!\n");
        return -1;
    }

    // Get the screen work area
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    // Create the window
    hwnd = CreateWindow(
        szAppName,  // Class name (previously registered)
        szAppName,  // Window title
        WS_OVERLAPPEDWINDOW, // Window style
        (rect.right-width)/2,   (rect.bottom-height)/2, // Position
        width, height,  // size
        NULL,  // Parent window (none)
        NULL,  // Menu (none)
        hInstance,  // hInstance (your app's instance)
        NULL  // Additional data (none)
    );

    SetWindowLongPtr(hwnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(this));

    ShowWindow(hwnd,SW_SHOWNORMAL); //Show the window to the world

    UpdateWindow(hwnd); // Update everything to make it ready (refresh the window).

    return 0;
}

void Window::runGameLoop(void) 
{     
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isTerminated = true;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
}


  void Window::toggleFullscreen()
    {
		// Variable declarations
		static DWORD dwStyle;
		static WINDOWPLACEMENT wp;
		MONITORINFO mi;


		// Code
		wp.length = sizeof(WINDOWPLACEMENT);

		if (bFullscreen == FALSE)
		{
			dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			if (dwStyle & WS_OVERLAPPEDWINDOW)
			{
				mi.cbSize = sizeof(MONITORINFO);
				if (GetWindowPlacement(hwnd, &wp) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &mi))
				{
					SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
				}
				ShowCursor(FALSE);
				bFullscreen = TRUE;
			}
		}
		else
		{
			SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hwnd, &wp);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

			ShowCursor(TRUE);
			bFullscreen = FALSE;
		}

    }


LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    // Code

     Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (!window) {
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

    switch(iMsg)
    {   
        case WM_CHAR:
            switch (wParam)
            {
            case 'F':
            case 'f':
                window->toggleFullscreen();
                break;

            default:
                break;
            }
            break;
        
        case WM_SIZE:
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);
            break;

         case WM_KEYDOWN:
            switch (wParam)
            {
            case 27:
                DestroyWindow(hwnd);
                break;

            default:
                break;
            }
            break;

        case WM_DESTROY:
            window->log.write("WM_DESTROYs");
            PostQuitMessage(0);  // Post a quit message, ends the message loop
            return 0;
        
        default:
            break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

