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
    void mousewheel(int delta, int x, int y);

    // Create Window
    Window window(WINWIDTH, WINHEIGHT, "OGL");
    window.setResizeCallback(resize); 
    window.setKeyDownCallback(keydown); 
    window.setMouseMoveCallback(mousemove); 
    window.setMouseLButtonDownCallback(l_buttonDown); 
    window.setMouseLButtonUPCallback(l_buttonUp); 
    window.setMouseWheelCallback(mousewheel); 
    
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
        window.calculateFPS();
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

void mousewheel(int delta, int x, int y)
{
    mainLog.debug("x : ",x);
    mainLog.debug("y : ",y);
    mainLog.debug("delta : ",delta);

    
    GLfloat zoomSpeed = 0.1f; // Speed of zoom
    GLfloat minRadius = 2.0f; // Minimum distance to target
    GLfloat maxRadius = 1000.0f; // Maximum distance to target

    // Adjust the orbit radius based on mouse wheel input
    camera.orbitRadius -= delta * zoomSpeed;

    // Clamp the radius to prevent it from going too far or too close
    camera.orbitRadius = glm::clamp(camera.orbitRadius, minRadius, maxRadius);

    // Update the camera position after zooming
    GLfloat centerX = camera.target.x;
    GLfloat centerY = camera.target.y;
    GLfloat centerZ = camera.target.z;

    camera.position.x = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::sin(angleCamera) + centerX;
    camera.position.y = camera.orbitRadius * glm::sin(camera.verticalAngle) + centerY;
    camera.position.z = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::cos(angleCamera) + centerZ;

    camera.updateTransform();
}
void mousemove(float xpos, float ypos)
{
    // Normalize cursor position
    cursor.x = (xpos / sizes.width - 0.5);
    cursor.y = -(ypos / sizes.height - 0.5);

    // Orbit camera adjustments
    if (camera.isRotating)
    {
        GLfloat horizontalSensitivity = 0.02f; // Horizontal rotation sensitivity
        GLfloat verticalSensitivity = 0.02f;   // Vertical rotation sensitivity

        // Adjust longitude and latitude based on mouse movement
        angleCamera += (xpos - cursor.lastx) * horizontalSensitivity;
        camera.verticalAngle += (ypos - cursor.lasty) * verticalSensitivity;

        // Clamp vertical angle to prevent flipping
        camera.verticalAngle = glm::clamp(camera.verticalAngle, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
    }

    // Update the camera position based on spherical coordinates
    GLfloat centerX = camera.target.x;
    GLfloat centerY = camera.target.y;
    GLfloat centerZ = camera.target.z;

    camera.position.x = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::sin(angleCamera) + centerX;
    camera.position.y = camera.orbitRadius * glm::sin(camera.verticalAngle) + centerY;
    camera.position.z = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::cos(angleCamera) + centerZ;

    // Update the cursor position for the next frame
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
   
}

