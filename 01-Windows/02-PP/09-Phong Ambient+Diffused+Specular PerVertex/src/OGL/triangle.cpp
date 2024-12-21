// triangle.cpp
#include "triangle.h"
#include "global.h"
#include "shader.h"

using namespace OGL;

const char* triangleVertexShaderSource = R"(
    #version 460 core
    in vec4 a_position;
    in vec4 a_color;
    uniform mat4 u_modelMatrix;
    uniform mat4 u_viewMatrix;
    uniform mat4 u_projectionMatrix;
    out vec4 a_color_out;
    void main()
    {
        gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
        a_color_out = a_color;
    }
)";

const char* triangleFragmentShaderSource = R"(
    #version 460 core
    in vec4 a_color_out;
    out vec4 FragColor;
    void main()
    {
        FragColor = a_color_out;
    }
)";

std::vector<GLfloat> trianglePositions = {
    0.0f,  0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

std::vector<GLfloat> triangleColors = { 
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

std::vector<GLuint> triangeIndices = {0, 1, 2};

GLuint triangleVAO, triangleVBO, triangleColorVBO, triangleEBO;
GLuint triangleShaderProgramObject;
GLuint triangelVertexShader;
GLuint triangleFragmentShader;

void triangleInit() {
    GLuint u_modelMatrix;
    GLuint u_viewMatrix;
    GLuint u_projectionMatrix;

    triangelVertexShader = createShader(GL_VERTEX_SHADER, triangleVertexShaderSource);
    triangleFragmentShader = createShader(GL_FRAGMENT_SHADER, triangleFragmentShaderSource);
    
    std::unordered_map<std::string, GLuint> attributes = {
        {"a_position", ATTRIBUTE_POSITION},
        {"a_color", ATTRIBUTE_COLOR}
    };
    
    triangleShaderProgramObject = linkShadersAndAttributes(triangelVertexShader, triangleFragmentShader, attributes);
    
    glDeleteShader(triangelVertexShader);
    glDeleteShader(triangleFragmentShader);
    
    genVertexArrays(&triangleVAO);
    
    genBuffers(&triangleVBO, trianglePositions);
    vertexAttribPointer(ATTRIBUTE_POSITION, 3);
    unbindBuffer();

    genBuffers(&triangleColorVBO, triangleColors);
    vertexAttribPointer(ATTRIBUTE_COLOR, 3);
    unbindBuffer();

    genBuffers(&triangleEBO, triangeIndices);
    
    unbindVertexArray();
}

void triangleDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    glUseProgram(triangleShaderProgramObject);
    
    glm::mat4 translationMatrix = glm::mat4(1.0);
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = translate(modelMatrix, glm::vec3(-0.5f, 0.0f, -2.0f));
    
    setUniformMatrix4fv(triangleShaderProgramObject, "u_modelMatrix", modelMatrix);
    setUniformMatrix4fv(triangleShaderProgramObject, "u_viewMatrix", viewMatrix);
    setUniformMatrix4fv(triangleShaderProgramObject, "u_projectionMatrix", projectionMatrix);
    
    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glDrawElements(GL_TRIANGLE_FAN,sizeof(triangeIndices)*sizeof(triangeIndices[0]),GL_UNSIGNED_INT,0);


    glUseProgram(0);
}

void triangleCleanup() {
    if (triangleVAO) {
        glDeleteVertexArrays(1, &triangleVAO);
        triangleVAO = 0;
    }
    if (triangleVBO) {
        glDeleteBuffers(1, &triangleVBO);
        triangleVBO = 0;
    }
    if (triangleColorVBO) {
        glDeleteBuffers(1, &triangleColorVBO);
        triangleColorVBO = 0;
    }
    if (triangleEBO) {
        glDeleteBuffers(1, &triangleEBO);
        triangleEBO = 0;
    }
    
    if (triangleShaderProgramObject) {
        deleteShaderProgramObject(triangleShaderProgramObject);
        triangleShaderProgramObject = 0;
    }
}
