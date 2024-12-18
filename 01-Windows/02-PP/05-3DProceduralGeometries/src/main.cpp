#include <OGL/objects.h>
#include <OGL/geomat.h>

#define WINWIDTH 800
#define WINHEIGHT 600

glm::mat4 perspectiveProjectionMatrix;
//std::vector<Mesh> scene;
Scene scene;
Camera camera;

Logger mainLog("Main.log");
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
    Window window(WINWIDTH, WINHEIGHT, "OGL - 3d Objects");
    window.setResizeCallback(resize); 

    // GLEW Initialization
    if(glewInit() != GLEW_OK)
        window.log.write("Error : GLEW initialization failed.");
    
    if(!initialize()==FALSE)
    {
        mainLog.error("Main Initialize Failed","");
        return(1);
    }

    
    
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
    perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    
}

int initialize()
{   
    // Code
    //Geometry PlaneGeometry = planeGeometry();
    Geometry coneGeometry = OGL::coneGeometry(0.5,1,13);
    Material basicMaterial = OGL::basicMaterial();
    Mesh cone = createMesh(coneGeometry, basicMaterial, "Plane");
    cone.translate(glm::vec3(-3.0f, 1.0f, -6.0f));
    cone.rotate(1.0f);
    cone.scale(glm::vec3(1,1,1));
    scene.add(cone);

    Geometry cubeGeometry = OGL::cubeGeometry(0.8,5);
    Material cubeMaterial = OGL::basicMaterial();
    Mesh cube = createMesh(cubeGeometry, basicMaterial, "Cube");
    cube.translate(glm::vec3(0.0f,1.2f,-6.0f));
    cube.rotate(1.0f);
    cube.scale(glm::vec3(1,1,1));
    scene.add(cube);

    Geometry sphereGeometry = OGL::sphereGeometry(0.5,36,20);
    Mesh sphere = createMesh(sphereGeometry, cubeMaterial, "sphere");
    sphere.translate(glm::vec3(3.0f,1.2f,-6.0f));
    sphere.rotate(1.0f);
    sphere.scale(glm::vec3(1,1,1));
    scene.add(sphere);

    Geometry torusGeometry = OGL::torusGeometry(0.5f,0.2f,36,9);
    Mesh torus = createMesh(torusGeometry, cubeMaterial, "torus");
    torus.translate(glm::vec3(3.0f,-1.2f,-6.0f));
    torus.rotate(1.0f);
    torus.scale(glm::vec3(1,1,1));
    scene.add(torus);

    Geometry planeGeometry = OGL::planeGeometry(1,1,10,10);
    Mesh plane = createMesh(planeGeometry, cubeMaterial, "plane");
    plane.translate(glm::vec3(0.0f,-1.2f,-6.0f));
    plane.rotate(1.0f);
    plane.scale(glm::vec3(1,1,1));
    scene.add(plane);

    Geometry pyramidGeometry = OGL::coneGeometry(0.5,0.6,4);
    Mesh pyramid = createMesh(pyramidGeometry, basicMaterial, "Plane");
    pyramid.translate(glm::vec3(-3.0f, -1.5f, -6.0f));
    pyramid.rotate(1.0f);
    pyramid.scale(glm::vec3(1,1,1));
    scene.add(pyramid);

    camera.position = glm::vec3(0.0f,0.0f,2.0f);
    camera.updateTransform();

    // Depth & Clear Color
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    
    

    // Optional
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    perspectiveProjectionMatrix = glm::mat4(1.0);
    resize(WINWIDTH, WINHEIGHT);
    return 0;
}

Timer timer;
void display()
{
    // code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 viewMatrix = camera.getViewMatrix();
    for(int i=0; i<scene.meshes.size() ; i++)
    {   
        OGL::displayBasic( viewMatrix,perspectiveProjectionMatrix, scene.meshes[i]);
    }
}

void update()
{
    GLfloat deltaTime = timer.getElapsedTime();
    GLfloat rotationSpeed = 2.0f;  // Adjust the speed as needed
    
    // Update rotation for each mesh in the scene
    for (auto& mesh : scene.meshes)
    {
        // Increment rotation based on elapsed time
        mesh.rotation.x += deltaTime * rotationSpeed; // X-axis rotation
        mesh.rotation.y += deltaTime * rotationSpeed; // Y-axis rotation
        mesh.rotation.z += deltaTime * rotationSpeed; // Z-axis rotation
        mesh.updateModelMatrix();
    }
}

void cleanupScene(std::vector<Mesh>& meshes) {
    for (auto& mesh : meshes) {
        mesh.cleanup(); // Call cleanup on each mesh
    }
    meshes.clear(); // Clear the scene vector after cleanup
}

void uninitialize()
{
    cleanupScene(scene.meshes);
    //triangleCleanup();
}


    // glEnable(GL_MULTISAMPLE);
    // glEnable(GL_LINE_SMOOTH);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glFrontFace(GL_CCW);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
