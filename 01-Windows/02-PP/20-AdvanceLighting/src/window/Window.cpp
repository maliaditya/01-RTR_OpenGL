#include "Window.h"
#include "resource.h"
#include <iostream>

Window::Window(int width, int height, const char* title): width(width), height(height), log("Window.log")
{
    // Code
    
    // Copy title to szAppname
    strncpy(szAppName, title, sizeof(szAppName)-1);
    szAppName[sizeof(szAppName)-1] = '\0'; 

    // Create Window
    if(init())
    {
        log.write("ERROR : Window Creation Failed\n");
    }

    if(setPixelFormat())
    {
        log.write("ERROR: Set pixel format failed \n");
    }

    initializeTimer();  
}

Window::~Window()
{
    // Code

    // Cleanup resources
    if (hwnd) {
        DestroyWindow(hwnd);  // Destroy the window
        hwnd = nullptr;       // Nullify the handle
    }

    if (hdc) {
        ReleaseDC(hwnd, hdc);  // Release the device context
        hdc = nullptr;
    }

    if (hrc) {
        wglDeleteContext(hrc); // Delete OpenGL context if it exists
        hrc = nullptr;
    }

    if (wglGetCurrentContext()==hrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

    KillTimer(hwnd, 1);
}

float Window::calculateFPS() {
    QueryPerformanceCounter(&currentTime_);

    // Elapsed time in seconds
    double elapsedTime = static_cast<double>(currentTime_.QuadPart - startTime_.QuadPart) / frequency_.QuadPart;

    if (elapsedTime >= 1.0) {
        fps = frameCount / elapsedTime;

        // Reset for the next second
        frameCount = 0;
        startTime_ = currentTime_;

        // Update the window title with FPS
        char newTitle[256];
        snprintf(newTitle, sizeof(newTitle), "%s - FPS: %.2f", szAppName, fps);
        if (!SetWindowText(hwnd, newTitle)) 
        {
            log.write("ERROR: Failed to set window text\n");
        }

        return fps;
    }

    return 0.0f; // Return 0 if FPS calculation isn't updated yet
}

int Window::init(void)
{
    // Code
    
    // Initiazlize WNDCLASSEX struct
    wndclass.cbSize = sizeof(WNDCLASSEX);  // Measure the window
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Decide its resizing behavior.
    wndclass.cbClsExtra = 0; // No shared storage.
    wndclass.cbWndExtra = 0;  // No unique storage.
    wndclass.lpfnWndProc = WndProc; //Assign a manager.
    wndclass.hInstance = hInstance; //  Choose the instance it belongs to.
    wndclass.hbrBackground =(HBRUSH)GetStockObject(BLACK_BRUSH); // Pick a background color
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // Add a big icon
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // Choose a cursor style.
    wndclass.lpszClassName = szAppName; // Give the class a name.
    wndclass.lpszMenuName = NULL; //Decide if a menu is needed.
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // Add a small icon.

    // Register the window class
    if (!RegisterClassEx(&wndclass)) {
        log.write("ERROR : Window class registration failed!\n");
        return -1;
    }

    // Get the screen work area
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    // Create the window
    hwnd = CreateWindow(
        szAppName,  // Class name (previously registered)
        szAppName ,  // Window title
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

    SetTimer(hwnd, 1, 100, NULL);

    return 0;
}

// Function to initialize high-resolution timer
void Window::initializeTimer() {
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&startTime_);
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
    frameCount++;
}

int Window::setPixelFormat(void)
{
    // code

    // Variable declarations
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;


    // Initialization of pixelformat discriptor
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32; // for Depth

    hdc = GetDC(hwnd);

    iPixelFormatIndex = ChoosePixelFormat(hdc, &pfd);
    if(iPixelFormatIndex == 0)
    {
        log.write("ERROR : Choose pixel format Failed.");
        return -1;
    }

    if(SetPixelFormat(hdc,iPixelFormatIndex, &pfd)==FALSE)
    {
        log.write("ERROR : Set pixel format failed.");
        return -1;
    }
    
    // Create rendering context
    hrc = wglCreateContext(hdc);
    if(hrc == NULL)
    {
       log.write("Error : Create rendering context failed.");
        return -1;
    }

    // Make rendering context as the current context f
    if (wglMakeCurrent(hdc, hrc) == FALSE)
    {
        log.write("Error : Make rendering context as the current context failed ");
        return -1;
    }

    return 0;
}

void Window::setResizeCallback(void (*callback)(int, int))
{
    resizeCallback = callback;  // Set the callback function
}


void Window::setKeyDownCallback(void (*callback)(char))
{
    keydownCallback = callback;  // Set the callback function
}

void Window::setMouseMoveCallback(void (*callback)(float,float))
{
    mousemoveCallback = callback;  // Set the callback function
}

void Window::setMouseLButtonDownCallback(void (*callback)(double,double))
{
    mouseLButtonDownCallback = callback;  // Set the callback function
}

void Window::setMouseLButtonUPCallback(void (*callback)(double,double))
{
    mouseLButtonUPCallback = callback;  // Set the callback function
}

void Window::setMouseWheelCallback(void (*callback)(int, int, int) )
{
    mousewheelCallback = callback;  // Set the callback function
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

void Window::triggerResize(HWND hwnd)
{
    RECT rect;
    if (GetClientRect(hwnd, &rect))
    {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(width, height));
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    // Code
    static bool cursorVisible = true;
    static std::chrono::time_point<std::chrono::steady_clock> lastMouseMoveTime;
    const auto cursorTimeout = std::chrono::seconds(2);

    ImGui_ImplWin32_WndProcHandler(hwnd, iMsg, wParam, lParam);
    
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (!window) {
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

    switch(iMsg)
    {   

        case WM_SETFOCUS:
            window->isActive = TRUE;
            break;

        case WM_KILLFOCUS:
            window->isActive = FALSE; 
            break;
        case WM_TIMER:
            // Check if the cursor should be hidden
            if (cursorVisible && std::chrono::steady_clock::now() - lastMouseMoveTime > cursorTimeout)
            {
                cursorVisible = false;
                ShowCursor(FALSE);
            }
            break;
        
        case WM_MOUSEWHEEL:
        {
            window->delta = GET_WHEEL_DELTA_WPARAM(wParam); // Wheel scroll delta

            window->clientX = LOWORD(lParam);
            window->clientY = HIWORD(lParam);

            if (window->mousewheelCallback) // Call scroll-specific callback
            {
                window->mousewheelCallback(window->delta,window->clientX,window->clientY);
            }
            break;
        }

        case WM_MOUSEMOVE:

            window->clientX = LOWORD(lParam);
            window->clientY = HIWORD(lParam);
             if (window->mousemoveCallback)
            {
                window->mousemoveCallback(window->clientX,window->clientY);  // Call the callback in main.cpp
            }

                // Record the time of mouse movement
            lastMouseMoveTime = std::chrono::steady_clock::now();

            // Show the cursor if it's not already visible
            if (!cursorVisible)
            {
                cursorVisible = true;
                ShowCursor(TRUE);
            }
            break;
        case WM_LBUTTONUP:
            window->xpos = LOWORD(lParam);
            window->ypos = HIWORD(lParam);
             if (window->mouseLButtonUPCallback)
            {
                window->mouseLButtonUPCallback(window->xpos,window->ypos);  // Call the callback in main.cpp
            }
            break;


        case WM_LBUTTONDOWN:
            window->xpos = LOWORD(lParam);
            window->ypos = HIWORD(lParam);
             if (window->mouseLButtonDownCallback)
            {
                window->mouseLButtonDownCallback(window->xpos,window->ypos);  // Call the callback in main.cpp
            }
            break;


        case WM_CHAR:
            window->keydown = wParam;
            
            if (window->keydownCallback)
            {
                window->keydownCallback(window->keydown);  // Call the callback in main.cpp
            }

            switch (wParam)
            {
            case 'F':
            case 'f':
                window->toggleFullscreen();
                break;
            case 'G':
            case 'g':
                window->triggerResize(window->hwnd);
                break;

            default:
                break;
            }
            break;
        
        case WM_SIZE:
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);
            
            if (window->resizeCallback)
            {
                window->resizeCallback(window->width, window->height);  // Call the callback in main.cpp
            }

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
            PostQuitMessage(0);  // Post a quit message, ends the message loop
            return 0;
        
        default:
            break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}