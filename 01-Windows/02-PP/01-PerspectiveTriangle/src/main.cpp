#include <windows.h>
#include <window/Window.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <OGL/triangle.h>

#define WINWIDTH 800
#define WINHEIGHT 600

glm::mat4 perspectiveProjectionMatrix;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstrCmdLine, int iCmdShow)
{
    // Code

    // Function Declaration
    int initialize(void);
    void resize(int width, int height);
    void display(void);
    void update(void);
    void uninitialize(void);
    
    // Create Window
    Window window(WINWIDTH, WINHEIGHT, "OGL - Triangle");
    window.setResizeCallback(resize); 

    // GLEW Initialization
    if(glewInit() != GLEW_OK)
        window.log.write("Error : GLEW initialization failed.");
    
    initialize();

    // Game Loop
    while(window.isTerminated == FALSE)
    {
        //code
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

void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    perspectiveProjectionMatrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

int initialize()
{   
    triangleInit();

    glClearColor(0.2f,0.3f,0.3f,1.0f);

    perspectiveProjectionMatrix = glm::mat4(1.0);

    resize(WINWIDTH, WINHEIGHT);
    return 0;
}

void display()
{
    // code

    glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 viewMatrix = glm::mat4(1.0);
    triangleDisplay(viewMatrix,perspectiveProjectionMatrix);

}

void update()
{

}

void uninitialize()
{
    triangleCleanup();
}