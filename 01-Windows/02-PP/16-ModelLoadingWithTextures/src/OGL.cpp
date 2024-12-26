#include "OGL.h"
#include <OGL/texture.h>
#include <OGL/model.h>
#include <OGL/shaders/phongADSLight.h>
#include <utils/combineStrings.h>

Scene scene;
Camera camera;
Logger ogl("OGL.log");
Timer t;
struct Light_ {
    glm::vec4 lightAmbient;
    glm::vec4 lightDiffuse;
    glm::vec4 lightSpecular;
    glm::vec4 lightPosition;
};


Light_ lights[3];

glm::vec4 materialAmbient = glm::vec4(0.0f,0.0f,0.0f,1.0f);
glm::vec4 materialDiffuse =  glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 materialSpecular = glm::vec4(1.0f,1.0f,1.0f,1.0f);
GLfloat materialShininess = 50.0f;

GLuint lightingEnabledUniform;
GLuint plfs = 1;
GLfloat lightAngle;

const char* vsp = R"(



in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec4 viewPosition_out;
out vec3 a_normal_out;
out vec3 phong_ads_light_pervertex_out;



void main()
{   
    vec4 modelPosition = u_modelMatrix * vec4(a_position, 1.0);
    vec4 viewPosition = u_viewMatrix * modelPosition;
    
    gl_Position = u_projectionMatrix * viewPosition;
    viewPosition_out =viewPosition;
    a_normal_out = a_normal;


}
)";

const char* fsp = R"(

in vec4 viewPosition_out;
in vec3 a_normal_out;
uniform int plfs;

out vec4 FragColor;
in vec3 phong_ads_light_pervertex_out;
vec3 phong_ads_light;


void main()
{   
    for(int i=0; i<MAX_LIGHTS; i++)
        phong_ads_light = phong_ads_light + phongLight(u_light[i] ,a_normal_out,viewPosition_out);    
    
    FragColor = vec4(phong_ads_light, 1.0);
}

)";



const char* fs = combine(phongADSLight,fsp);
const char* vs = combine(phongADSLight,vsp);



int initialize()
{   
    // Code

    std::vector<Geometry> models = OGL::loadModel(std::string(RESOURCE_DIR)+"/models/model.glb");
    ogl.debug("Model Name: ", models[0].name);
    ogl.printGLMVectorToLog("positions: ", models[0].positions);


    Material shaderMaterial;
    GLuint vertexShader = OGL::createShader(GL_VERTEX_SHADER, vs, "ShaderMaterial");
    GLuint fragmentShader = OGL::createShader(GL_FRAGMENT_SHADER, fs, "ShaderMaterial");
    shaderMaterial.attributes = {
                                    {"a_postion", ATTRIBUTE_POSITION},
                                    {"a_normal", ATTRIBUTE_NORMAL},
                                    {"a_uv", ATTRIBUTE_TEXCOORD}
                                };    
    shaderMaterial.shaderProgramObject = OGL::linkShadersAndAttributes(vertexShader,fragmentShader,shaderMaterial.attributes, "ShaderMaterial");

    
    Geometry cubeGeometry =  models[1];
    Mesh walls = OGL::createMesh(cubeGeometry, shaderMaterial);
    walls.name = "cube";
    walls.translate(glm::vec3(0.0f,0.0f,-10.0f));
    walls.scale(glm::vec3(1,1,1));
    scene.add(walls);

    camera.position.z = 2;
    camera.target = walls.position;

    // Initialize Lights
    lights[0].lightAmbient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightSpecular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPosition = glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	lights[1].lightAmbient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1].lightDiffuse  = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightSpecular = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightPosition = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);

    lights[2].lightAmbient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2].lightDiffuse  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightSpecular = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightPosition = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);


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

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        GLuint shaderProgramObject =  mesh.material.shaderProgramObject;
        glUseProgram(shaderProgramObject);
        
        // send uniforms
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_modelMatrix", mesh.modelMatrix);
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_viewMatrix", camera.getViewMatrix());
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_projectionMatrix", camera.projectionMatrix);
        

        lights[0].lightPosition = glm::vec4(0.0f, 80 * sin(lightAngle), 80 * cos(lightAngle), 1.0f);
	    lights[1].lightPosition = glm::vec4(80 * cos(lightAngle), 0.0f, 80 * sin(lightAngle), 1.0f);
	    lights[2].lightPosition = glm::vec4(80 * sin(lightAngle), 80 * cos(lightAngle), 0.0f, 1.0f);

        for(int i=0; i<3; i++)
        {
            std::string baseName = "u_light[" + std::to_string(i) + "]";
            OGL::setUniform3fv(shaderProgramObject, (baseName + ".u_la").c_str() ,lights[i].lightAmbient );
            OGL::setUniform3fv(shaderProgramObject, (baseName + ".u_ld").c_str() ,lights[i].lightDiffuse );
            OGL::setUniform3fv(shaderProgramObject, (baseName + ".u_ls").c_str() ,lights[i].lightSpecular );
            OGL::setUniform4fv(shaderProgramObject, (baseName + ".u_lightPosition").c_str(), lights[i].lightPosition);
        }
        
        OGL::setUniform3fv(shaderProgramObject, "u_ka" , materialAmbient);
        OGL::setUniform3fv(shaderProgramObject, "u_kd" ,materialDiffuse);
        OGL::setUniform3fv(shaderProgramObject, "u_ks" , materialSpecular);
        OGL::setUniform1f(shaderProgramObject, "u_materialShininess" , materialShininess);
        OGL::setUniform1i(shaderProgramObject, "plfs" , plfs);
        

        // draw
        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh.indicesEBO);
        glDrawElements(GL_TRIANGLES,mesh.geometry.indices.size(), GL_UNSIGNED_INT, 0);

        // unbind
        OGL::unbindBuffer();
        OGL::unbindVertexArray();
    }
}

void update()
{
    GLfloat speed = 2.0f;
    lightAngle += t.getElapsedTime() *  speed;
}

void uninitialize()
{
    scene.cleanupScene(scene.meshes);
}
