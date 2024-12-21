
#include "OGL.h"

Scene scene;
Camera camera;

GLuint ldUniform;
GLuint kdUniform;
GLuint lightPoistionUniform;

glm::vec4 lightDiffuse = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
glm::vec4 materialDiffuse = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f );
glm::vec4 lightPosition = glm::vec4( 0.0f, 0.0f, 2.0f, 1.0f );

const char* vs = R"(
#version 460 core

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform vec4 u_ld;
uniform vec4 u_kd;
uniform vec4 u_lightPosition;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec3 diffusedLightColor_out;

void main()
{   
    vec4 modelPosition = u_modelMatrix * vec4(a_position, 1.0);
    vec4 viewPosition = u_viewMatrix * modelPosition;

    mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));
    vec3 transformedNormals = normalize(normalMatrix * a_normal);

    vec3 lightDirection = normalize(vec3(u_lightPosition - viewPosition));
    diffusedLightColor_out = vec3(u_ld) * vec3(u_kd) * max(dot(transformedNormals, lightDirection), 0.0);

    gl_Position = u_projectionMatrix * viewPosition;
}

)";

const char* fs = R"(
#version 460 core

in vec3 diffusedLightColor_out;

out vec4 FragColor;

void main()
{   
    FragColor = vec4(diffusedLightColor_out,1.0f);
}

)";


// Function Declaration
int initialize(void)
{
    // Code

    // Geometry =  generate vertex: position, normal, indices
    Geometry cubeGeometry = OGL::cubeGeometry(4,10);
    
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

        OGL::setUniform4fv(shaderProgramObject, "u_ld", lightDiffuse);
        OGL::setUniform4fv(shaderProgramObject, "u_kd", materialDiffuse);
        OGL::setUniform4fv(shaderProgramObject, "u_lightPosition", lightPosition);

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