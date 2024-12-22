
#include "OGL.h"

Scene scene;
Camera camera;

glm::vec4 lightAmbient = glm::vec4(0.0f,0.0f,0.0f,1.0f);
glm::vec4 lightDiffuse = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 lightSpecular = glm::vec4(1.0f,1.0f,1.0f,1.0f);
glm::vec4 lightPosition = glm::vec4( 100.0f, 100.0f,100.0f,1.0f);

glm::vec4 materialAmbient = glm::vec4(0.0f,0.0f,0.0f,1.0f);
glm::vec4 materialDiffuse =  glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 materialSpecular = glm::vec4(1.0f,1.0f,1.0f,1.0f);
GLfloat materialShininess = 50.0f;

GLuint lightingEnabledUniform;

const char* vs = R"(
#version 460 core

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec4 viewPosition_out;
out vec3 a_normal_out;

void main()
{   
    vec4 modelPosition = u_modelMatrix * vec4(a_position, 1.0);
    vec4 viewPosition = u_viewMatrix * modelPosition;
    
    gl_Position = u_projectionMatrix * viewPosition;

    viewPosition_out = viewPosition; 
    a_normal_out = a_normal;  
}
)";

const char* fs = R"(
#version 460 core

in vec4 viewPosition_out;
in vec3 a_normal_out;

vec3 phong_ads_light;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec4 u_lightPosition;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_materialShininess;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;

out vec4 FragColor;

void main()
{   
    // Ambient Light
    vec3 ambient = u_la * u_ka;

    // Diffused Light  
    mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
    vec3 transformedNormals = normalize(normalMatrix * a_normal_out);
    vec3 lightDirection = normalize(vec3(u_lightPosition) - viewPosition_out.xyz);
    vec3 diffuse = u_ld * u_kd * max(dot(transformedNormals, lightDirection), 0.0);
    
    // Specular Light 
    vec3 reflectionVector = reflect(-lightDirection,transformedNormals);
    vec3 viewerVector = normalize(-viewPosition_out.xyz);
    vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector,viewerVector),0.0), u_materialShininess);
    
    // Phong ADS Light
    phong_ads_light = ambient + diffuse + specular;

    FragColor = vec4(phong_ads_light,1.0f);


}

)";


// Function Declaration
int initialize(void)
{
    // Code

    // Geometry =  generate vertex: position, normal, indices
    Geometry cubeGeometry = OGL::sphereGeometry(5,100,50);
    
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

        OGL::setUniform3fv(shaderProgramObject, "u_la" ,lightAmbient );
        OGL::setUniform3fv(shaderProgramObject, "u_ld" ,lightDiffuse );
        OGL::setUniform3fv(shaderProgramObject, "u_ls" ,lightSpecular );
        OGL::setUniform4fv(shaderProgramObject, "u_lightPosition" , lightPosition);
        OGL::setUniform3fv(shaderProgramObject, "u_ka" , materialAmbient);
        OGL::setUniform3fv(shaderProgramObject, "u_kd" ,materialDiffuse);
        OGL::setUniform3fv(shaderProgramObject, "u_ks" , materialSpecular);
        OGL::setUniform1f(shaderProgramObject, "u_materialShininess" , materialShininess);

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