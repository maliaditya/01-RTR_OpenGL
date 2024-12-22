
#include "OGL.h"
#include <OGL/shaders/phongADSLight.h>
#include <utils/combineStrings.h>

Scene scene;
Camera camera;

struct Light_ {
    glm::vec4 lightAmbient;
    glm::vec4 lightDiffuse;
    glm::vec4 lightSpecular;
    glm::vec4 lightPosition;
};


Light_ lights[2];

glm::vec4 materialAmbient = glm::vec4(0.0f,0.0f,0.0f,1.0f);
glm::vec4 materialDiffuse =  glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 materialSpecular = glm::vec4(1.0f,1.0f,1.0f,1.0f);
GLfloat materialShininess = 50.0f;

GLuint lightingEnabledUniform;
GLuint plfs = 1;



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

    for(int i=0; i<2; i++)
        phong_ads_light_pervertex_out = phong_ads_light_pervertex_out + phongLight(u_light[i] ,a_normal,viewPosition);
}
)";

const char* fsp = R"(
in vec4 viewPosition_out;
in vec3 a_normal_out;
uniform int plfs;

out vec4 FragColor;
in vec3 phong_ads_light_pervertex_out;

void main()
{   
    FragColor = vec4(phong_ads_light_pervertex_out, 1.0);
}

)";



const char* fs = combine(phongADSLight,fsp);
const char* vs = combine(phongADSLight,vsp);



// Function Declaration
int initialize(void)
{
    // Code

    // Geometry =  generate vertex: position, normal, indices
    Geometry cubeGeometry = OGL::cubeGeometry(5,1);
    
    // Material =  shader create build compile 
    Material shaderMaterial;
    GLuint vertexShader = OGL::createShader(GL_VERTEX_SHADER, vs, "ShaderMaterial");
    GLuint fragmentShader = OGL::createShader(GL_FRAGMENT_SHADER, fs, "ShaderMaterial");
    shaderMaterial.attributes = {
                                    {"a_postion", ATTRIBUTE_POSITION},
                                    {"a_normal", ATTRIBUTE_NORMAL},
                                    {"a_uv", ATTRIBUTE_TEXCOORD}
                                };    
    shaderMaterial.shaderProgramObject = OGL::linkShadersAndAttributes(vertexShader,fragmentShader,shaderMaterial.attributes, "ShaderMaterial");

    // Buffer creation form position, normal,indices
    Mesh cube = OGL::createMesh(cubeGeometry, shaderMaterial); 
    scene.add(cube);

    // camera
    camera.position.z = 2;
    camera.target = cube.position;

    // Initialize Lights
    lights[0].lightAmbient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightSpecular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPosition = glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	lights[1].lightAmbient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1].lightDiffuse  = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightSpecular = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightPosition = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);



    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Smooth
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // clearColor
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    return(0);
}

void display(void)
{
    // Code
    for(auto& mesh : scene.meshes)
    {   
        GLuint shaderProgramObject =  mesh.material.shaderProgramObject;
        glUseProgram(shaderProgramObject);
        
        // send uniforms
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_modelMatrix", mesh.modelMatrix);
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_viewMatrix", camera.getViewMatrix());
        OGL::setUniformMatrix4fv(shaderProgramObject, "u_projectionMatrix", camera.projectionMatrix);

        for(int i=0; i<2; i++)
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

void update(void)
{
    // Code
    
}

void uninitialize(void)
{
    // Code
    scene.cleanupScene(scene.meshes);
    
}