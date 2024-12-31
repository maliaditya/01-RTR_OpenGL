#include "OGL.h"

#define WINWIDTH 800
#define WINHEIGHT 600

struct Cursor{
    double x; 
    double y;
    double lastx;
    double lasty;
};



Sizes sizes;
Cursor cursor;

BOOL GUI = FALSE;
GLfloat angleCamera;

Logger mainLog("Main.log");

std::unique_ptr<ImguiManager> imguiManager;

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
    
    imguiManager = std::make_unique<ImguiManager>(window.hwnd);


    // window resize 
    resize(WINWIDTH, WINHEIGHT);

    // Game Loop
    while(window.isTerminated == FALSE)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(GUI)
        {

            imguiManager->beginFrame();

            ImGui::SetNextWindowBgAlpha(0.0f);
            ImGui::Begin("Scene");
            ImVec2 windowSize = ImGui::GetWindowSize();

            resize(windowSize.x, windowSize.y);
            if(window.isActive == TRUE) // Render scene only when window is active
            {
                display(); // OGL display
                update();  // OGL update
            }
            
            ImGui::End();
            imguiManager->endFrame();
        }
        else
        {
             if(window.isActive == TRUE) // Render scene only when window is active
            {  
                display(); // OGL display
                update();  // OGL update
            }
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
    glViewport(sizes.dockedLeft, sizes.dockedBottom, width, height);
    camera.projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}
void mousewheel(int delta, int x, int y)
{


   // Get current OpenGL viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); 

     // Assuming the bottom window starts after the OpenGL scene and takes up part of the window
    int sceneHeight = viewport[3];  // Height of the OpenGL viewport
    int bottomWindowHeight = sizes.dockedBottom;   // Example height of the bottom window/UI area, adjust this value
    int sceneY = viewport[1];       // The y position of the scene in the window

    // Check if the cursor is inside the OpenGL viewport (both x and y axis)
    if (x >= viewport[0] && x <= viewport[0] + viewport[2] &&
        y >= viewport[1] && y <= viewport[1] + viewport[3]  &&
        y >= sceneY && y <= (sceneY + sceneHeight - bottomWindowHeight)
        ) 
    {
        GLfloat zoomSpeed = 0.01f;  // Speed of zoom
        GLfloat minRadius = 1.0f;   // Minimum distance to target
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
}

void mousemove(float xpos, float ypos)
{
    // Get current OpenGL viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); 

     // Assuming the bottom window starts after the OpenGL scene and takes up part of the window
    int sceneHeight = viewport[3];  // Height of the OpenGL viewport
    int bottomWindowHeight = sizes.dockedBottom;   // Example height of the bottom window/UI area, adjust this value
    int sceneY = viewport[1];       // The y position of the scene in the window

    // Check if the cursor is inside the OpenGL viewport (both x and y axis)
    if (xpos >= viewport[0] && xpos <= viewport[0] + viewport[2] &&
        ypos >= viewport[1] && ypos <= viewport[1] + viewport[3]  &&
        ypos >= sceneY && ypos <= (sceneY + sceneHeight - bottomWindowHeight)
        ) 
    {
        // Normalize cursor position (assuming window center is 0.0, 0.0)
        cursor.x = (xpos / sizes.width - 0.5f) * 2.0f;  // Normalize to [-1, 1]
        cursor.y = -(ypos / sizes.height - 0.5f) * 2.0f; // Normalize to [-1, 1]

        // Orbit camera adjustments only if the cursor is inside the viewport
        if (camera.isRotating)
        {
            GLfloat horizontalSensitivity = 0.02f; // Horizontal rotation sensitivity
            GLfloat verticalSensitivity = 0.02f;   // Vertical rotation sensitivity

            // Adjust camera angles based on mouse movement
            angleCamera += (xpos - cursor.lastx) * horizontalSensitivity;
            camera.verticalAngle += (ypos - cursor.lasty) * verticalSensitivity;

            // Clamp vertical angle to prevent flipping
            camera.verticalAngle = glm::clamp(camera.verticalAngle, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
        }

        // Update camera position based on spherical coordinates
        GLfloat centerX = camera.target.x;
        GLfloat centerY = camera.target.y;
        GLfloat centerZ = camera.target.z;

        camera.position.x = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::sin(angleCamera) + centerX;
        camera.position.y = camera.orbitRadius * glm::sin(camera.verticalAngle) + centerY;
        camera.position.z = camera.orbitRadius * glm::cos(camera.verticalAngle) * glm::cos(angleCamera) + centerZ;

        // Update cursor for next frame
        cursor.lastx = xpos;
        cursor.lasty = ypos;

        // Update camera transformation
        camera.updateTransform();
    }
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

    if(keydown == 'g' || keydown == 'G')
    {
        if(GUI)
        {
           GUI = FALSE; 
           sizes.dockedBottom = 0;
           sizes.dockedLeft = 0;
        }else
        {
            GUI = TRUE;
        }
    }
   
}

