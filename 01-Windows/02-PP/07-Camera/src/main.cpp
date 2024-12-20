#include "OGL.h"
Logger mainLog("Main.log");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstrCmdLine, int iCmdShow)
{
    // Code

     SetEnvironmentVariable("CUDA_VISIBLE_DEVICES", "0");  // Force CUDA to use GPU 0 (typically discrete GPU on laptops)

    // Create Window
    Window window(WINWIDTH, WINHEIGHT, "OGL - Texture");
    window.setResizeCallback(resize); 
    window.setKeyDownCallback(keydown); 
    window.setMouseMoveCallback(mousemove); 
    window.setMouseLButtonDownCallback(l_buttonDown); 
    window.setMouseLButtonUPCallback(l_buttonUp); 
    
    // GLEW Initialization
    if(glewInit() != GLEW_OK)
        mainLog.error("GLEW initialization failed.","");
    
    if(!initialize()==FALSE)
    {
        mainLog.error(" OGL Initialize Failed","");
        return(1);
    }
    
    // Game Loop
    while(window.isTerminated == FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(window.isActive == TRUE)
        {
            display();
            update();
        }

        SwapBuffers(window.hdc);
        window.runGameLoop();
    }

    uninitialize();
    return(0);
}
