
#include "OGL.h"
#include <OGL/texture.h>

Scene scene;
Camera camera;
// Function Declaration
int initialize(void) {
    
    GLuint sunMap   = OGL::loadTexture(std::string(RESOURCE_DIR) + "/textures/planets/2k_sun.jpg");
    GLuint earthMap = OGL::loadTexture(std::string(RESOURCE_DIR) + "/textures/planets/2k_earth_daymap.jpg");
    GLuint moonMap  = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/planets/2k_moon.jpg");

    // Mesh
    Material material = OGL::basicMaterial();

    Geometry earthGeometry = OGL::sphereGeometry(1.5,100,50);
    Mesh earth = OGL::createMesh(earthGeometry, material);
    earth.name = "earth";
    earth.texture.colorMap = earthMap;
    earth.color = glm::vec3(0.0f,0.0f,1.0f);
    scene.add(earth);

    Geometry sunGeometry = OGL::sphereGeometry(4,36,18);
    Mesh sun = OGL::createMesh(sunGeometry, material);
    sun.name = "sun";
    sun.texture.colorMap = sunMap;
    sun.color = glm::vec3(1.0f,1.0f,0.0f);
    scene.add(sun);

    Geometry moonGeometry = OGL::sphereGeometry(0.5,36,18);
    Mesh moon = OGL::createMesh(moonGeometry, material);
    moon.name = "moon";
    moon.texture.colorMap = moonMap;
    moon.color = glm::vec3(1.0f,1.0f,1.0f);
    scene.add(moon);
 
    // orbit Camera
    camera.orbitRadius = 50.0f;
    camera.target = sun.position;

    // Depth
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Texture
    glEnable(GL_TEXTURE_2D);

    // Points

    // clearColor
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Code
    return(0);
}

glm::mat4 tempMatrix = glm::mat4(1.0);
void display(void)
{
    // Code
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // display Mesh From scene
    for(auto& mesh : scene.meshes)
    {
        if(mesh.name == "earth")
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        OGL::displayBasic(camera,mesh);
    }
    
}
Timer t;
GLfloat angleEarth;
GLfloat angleMoon;
void update(void)
{
    
    // Code
    for(auto& mesh : scene.meshes)
    {
        if(mesh.name == "earth")
        {
            GLfloat radius = 12; // how much away from the center
            GLfloat centerOfcircleZ = camera.target.z; // camera target is object position
            GLfloat centerOfcircleX = camera.target.x; // camera target is object position
            mesh.position.x = radius * glm::sin(angleEarth) + centerOfcircleX;
            mesh.position.z = radius * glm::cos(angleEarth) + centerOfcircleZ;
            mesh.updateModelMatrix();

            mesh.rotateY(t.getElapsedTime());
        }
        if(mesh.name == "moon")
        {
            GLfloat radius = 3; // how much away from the center
            GLfloat centerOfcircleZ = scene.meshes[0].position.z; // camera target is object position
            GLfloat centerOfcircleX = scene.meshes[0].position.x; // camera target is object position
            mesh.position.x = radius * glm::sin(angleMoon) + centerOfcircleX;
            mesh.position.z = radius * glm::cos(angleMoon) + centerOfcircleZ;
            mesh.updateModelMatrix();

            mesh.rotateY(t.getElapsedTime()*0.01);
        }
         if(mesh.name == "sun")
         { 
            mesh.rotate(t.getElapsedTime());
         }

    }

    angleEarth += 0.001;
    angleMoon += 0.01;

}

void uninitialize(void)
{
    // Code
    scene.cleanupScene(scene.meshes);
}