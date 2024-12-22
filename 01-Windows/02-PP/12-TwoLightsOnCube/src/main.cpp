#include "OGL.h"

#define WINWIDTH 800
#define WINHEIGHT 600

struct Cursor{
    double x; 
    double y;
    double lastx;
    double lasty;
};

struct Sizes{
    int width = WINWIDTH;
    int height = WINHEIGHT;
};

Cursor cursor;
Sizes sizes;
GLfloat angleCamera;
Logger mainLog("Main.log");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstrCmdLine, int iCmdShow)
{
    // Code

    // Callback functions declaration
    void resize(int width, int height);
    void mousemove(float xpos, float ypos);
    void l_buttonDown(double xpos, double ypos);
    void l_buttonUp(double xpos, double ypos);
    void keydown(char keydown);

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
    
    // OGL initialization
    if(initialize() == EXIT_FAILURE) 
    {
        mainLog.error(" OGL Initialize Failed","");
        return(1);
    }
    
    // window resize 
    resize(WINWIDTH, WINHEIGHT);

    // Game Loop
    while(window.isTerminated == FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(window.isActive == TRUE) // Render scene only when window is active
        {
            display(); // OGL display
            update();  // OGL update
        }

        SwapBuffers(window.hdc);
        window.runGameLoop();  // Poll Events 
    }

    uninitialize();
    return(0);
}

void resize(int width, int height)
{
    sizes.width = width;
    sizes.height = height;
    glViewport(0, 0, width, height);
    camera.projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void mousemove(float xpos, float ypos)
{
    // code
    cursor.x = (xpos / sizes.width - 0.5);
    cursor.y = -(ypos / sizes.height - 0.5);

    // Default Orbit camera    
    GLfloat radius = 20.0f; // how much away from the center
    GLfloat centerOfcircleZ = camera.target.z; // camera target is object position
    GLfloat centerOfcircleX = camera.target.x; // camera target is object position
    camera.position.x = radius * glm::sin(angleCamera) + centerOfcircleX;
    camera.position.z = radius * glm::cos(angleCamera) + centerOfcircleZ;

    GLuint travelDistanceOfCameraAroundTargetPosition = 15.0f; // thisis not radius 
    camera.position.y = cursor.y * travelDistanceOfCameraAroundTargetPosition;

    if(camera.isRotating && xpos > cursor.lastx)
    {
        angleCamera +=  xpos * 0.001 *0.2;
    }
    if(camera.isRotating && xpos < cursor.lastx)
    {
        angleCamera -=  xpos * 0.001 * 0.2;
    }
    cursor.lastx = xpos;
    cursor.lasty = ypos;
    camera.updateTransform();
}


void l_buttonDown(double xpos, double ypos)
{
    camera.isRotating = true;
}

void l_buttonUp(double xpos, double ypos)
{
    camera.isRotating = false;
}

void keydown(char keydown)
{
    // code
    if(keydown == 'w' || keydown == 'W')
    {   
        camera.position.z -= 0.1;
    }

    if(keydown == 's' || keydown == 'S')
    {
        camera.position.z += 0.1;
    }

    if(keydown == 'e' || keydown == 'E')
    {
        angleCamera += 0.01;
    }
    if(keydown == 'q' || keydown == 'Q')
    {
        angleCamera -= 0.01;
    }
    if(keydown == 'v')
    {
        plfs = 0; // pervertex
    }    
    if(keydown == 'V')
    {
        plfs = 1; //Per fragment
    }    
}

