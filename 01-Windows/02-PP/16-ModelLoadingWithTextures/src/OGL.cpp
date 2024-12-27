#include "OGL.h"
#include <OGL/texture.h>
#include <OGL/model.h>
#include <OGL/shaders/phongADSLight.h>
#include <utils/combineStrings.h>

Scene scene;
Camera camera;
Logger ogl("OGL.log");

const char* vertexShaderSource = R"(
#version 460 core
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec2 a_uv_out;

void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
    a_uv_out = a_uv;
}
)";

const char* fragmentShaderSource = R"(
#version 460 core
uniform sampler2D texture1;

in vec2 a_uv_out;
out vec4 FragColor;
void main()
{
    FragColor = texture(texture1, a_uv_out);
}
)";


int initialize()
{   
    // Code

    std::vector<Geometry> models = OGL::loadModel(std::string(RESOURCE_DIR)+"/models/helmet.glb");
    GLuint container = OGL::loadTexture(std::string(RESOURCE_DIR)+"/textures/container.jpg");
    Geometry cubeGeometry =  models[0];
    Material shaderMaterial = OGL::shaderMaterial(vertexShaderSource, fragmentShaderSource);
    Mesh helmet = OGL::createMesh(cubeGeometry, shaderMaterial);
    helmet.name = "helmet";
    helmet.rotation.x = M_PI*0.5;
    helmet.texture.colorMap = cubeGeometry.texture.colorMap;
    helmet.translate(glm::vec3(0.0f,0.0f,0.0f));
    helmet.scale(glm::vec3(1,1,1));
    scene.add(helmet);

    Geometry cubeGeom = OGL::cubeGeometry(75,1);
    Material basicMaterial = OGL::basicMaterial();
    Mesh cube = OGL::createMesh(cubeGeom, basicMaterial);
    cube.name = "cube";
    cube.texture.colorMap = container;
    cube.translate(glm::vec3(0.0f,0.0f,0.0f));
    cube.scale(glm::vec3(1,1,1));
    scene.add(cube);

    camera.orbitRadius = 5;
    camera.target = helmet.position;

    // Depth & Clear Color
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    
	// Enabling the texture
	glEnable(GL_TEXTURE_2D);

    // Enable Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Optional
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    return 0;
}

void display()
{
    // Code
    for(auto& mesh : scene.meshes)
    {   
        glUseProgram(mesh.material.shaderProgramObject);
        if(mesh.name == "helmet")
        {       
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.colorMap);
            OGL::setUniform1i(mesh.material.shaderProgramObject, "u_colorMap", 0);

            OGL::displayShader(camera, mesh);
                
        }
        else
        {
            OGL::displayBasic(camera, mesh);
        }
    }
}

void update()
{
}

void uninitialize()
{
    scene.cleanupScene(scene.meshes);
}