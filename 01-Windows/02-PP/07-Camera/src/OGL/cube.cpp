#include "cube.h"

// Data
std::vector<GLfloat> cubePositions = 
{
    // Front face
   -0.5f, -0.5f,  0.5f,  // Bottom-left
    0.5f, -0.5f,  0.5f,  // Bottom-right
    0.5f,  0.5f,  0.5f,  // Top-right
   -0.5f,  0.5f,  0.5f,  // Top-left

    // Back face
   -0.5f, -0.5f, -0.5f,  // Bottom-left
    0.5f, -0.5f, -0.5f,  // Bottom-right
    0.5f,  0.5f, -0.5f,  // Top-right
   -0.5f,  0.5f, -0.5f,  // Top-left

    // Left face
   -0.5f, -0.5f, -0.5f,  // Bottom-left
   -0.5f, -0.5f,  0.5f,  // Bottom-right
   -0.5f,  0.5f,  0.5f,  // Top-right
   -0.5f,  0.5f, -0.5f,  // Top-left

    // Right face
    0.5f, -0.5f, -0.5f,  // Bottom-left
    0.5f, -0.5f,  0.5f,  // Bottom-right
    0.5f,  0.5f,  0.5f,  // Top-right
    0.5f,  0.5f, -0.5f,  // Top-left

    // Top face
   -0.5f,  0.5f,  0.5f,  // Bottom-left
    0.5f,  0.5f,  0.5f,  // Bottom-right
    0.5f,  0.5f, -0.5f,  // Top-right
   -0.5f,  0.5f, -0.5f,  // Top-left

    // Bottom face
   -0.5f, -0.5f,  0.5f,  // Bottom-left
    0.5f, -0.5f,  0.5f,  // Bottom-right
    0.5f, -0.5f, -0.5f,  // Top-right
   -0.5f, -0.5f, -0.5f,  // Top-left
}; 

std::vector<GLfloat> cubeColors= 
{
     // Front face (Red)
    1.0f, 0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f, 0.0f,  // Bottom-right
    1.0f, 0.0f, 0.0f,  // Top-right
    1.0f, 0.0f, 0.0f,  // Top-left

    // Back face (Green)
    0.0f, 1.0f, 0.0f,  // Bottom-left
    0.0f, 1.0f, 0.0f,  // Bottom-right
    0.0f, 1.0f, 0.0f,  // Top-right
    0.0f, 1.0f, 0.0f,  // Top-left

    // Left face (Blue)
    0.0f, 0.0f, 1.0f,  // Bottom-left
    0.0f, 0.0f, 1.0f,  // Bottom-right
    0.0f, 0.0f, 1.0f,  // Top-right
    0.0f, 0.0f, 1.0f,  // Top-left

    // Right face (Yellow)
    1.0f, 1.0f, 0.0f,  // Bottom-left
    1.0f, 1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f, 0.0f,  // Top-right
    1.0f, 1.0f, 0.0f,  // Top-left

    // Top face (Cyan)
    0.0f, 1.0f, 1.0f,  // Bottom-left
    0.0f, 1.0f, 1.0f,  // Bottom-right
    0.0f, 1.0f, 1.0f,  // Top-right
    0.0f, 1.0f, 1.0f,  // Top-left

    // Bottom face (Magenta)
    1.0f, 0.0f, 1.0f,  // Bottom-left
    1.0f, 0.0f, 1.0f,  // Bottom-right
    1.0f, 0.0f, 1.0f,  // Top-right
    1.0f, 0.0f, 1.0f,  // Top-left

}; 
std::vector<GLfloat> cubeNormals = 
{
    // Front face
    0.0f,  0.0f,  1.0f,  // Normal pointing forward
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

    // Back face
    0.0f,  0.0f, -1.0f,  // Normal pointing backward
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    // Left face
   -1.0f,  0.0f,  0.0f,  // Normal pointing left
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,

    // Right face
    1.0f,  0.0f,  0.0f,  // Normal pointing right
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    // Top face
    0.0f,  1.0f,  0.0f,  // Normal pointing up
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,

    // Bottom face
    0.0f, -1.0f,  0.0f,  // Normal pointing down
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
};

std::vector<GLfloat> cubeTexCoords = 
{
    // Front face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left

    // Back face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left

    // Left face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left


    // Right face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left

    // Top face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left

    // Bottom face
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,  // Top-left
};

std::vector<GLuint>  cubeIndices = {
    // Front face
    0, 1, 2,  // First triangle
    0, 2, 3,  // Second triangle

    // Back face
    4, 5, 6,
    4, 6, 7,

    // Left face
    8, 9, 10,
    8, 10, 11,

    // Right face
    12, 13, 14,
    12, 14, 15,

    // Top face
    16, 17, 18,
    16, 18, 19,

    // Bottom face
    20, 21, 22,
    20, 22, 23,
};


// Shaders
const char* cubeVertexShaderSource = R"(
#version 460 core
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
in vec4 a_position;
in vec4 a_color;
in vec4 a_normal;
in vec2 a_uv;
out vec4 a_color_out;
void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
    a_color_out = a_color;
    
}
)"; 
const char* cubeFragmentShaderSource = R"(
#version 460 core
in vec4 a_color_out;
in vec4 a_normal_out;
out vec4 FragColor;
void main()
{
    FragColor = a_color_out;
}
)"; 

GLuint cubeVertexShader; 
GLuint cubeFragmentShader; 
GLuint cubeShaderProgramObject;

// Buffers
GLuint cubeVAO; 
GLuint cubePositionVBO; 
GLuint cubeColorsVBO; 
GLuint cubeNormalVBO;
GLuint cubeTextcorrdVBO;
GLuint cubeEBO;

Logger cube("Cube.log");
// Function Declaration
int cubeInit(void)
{
    cube.printVectorToLog("Positions",cubePositions);

    // Create and Compile Shaders
    cubeVertexShader = createShader(GL_VERTEX_SHADER, cubeVertexShaderSource, "Cube");
    cubeFragmentShader = createShader(GL_FRAGMENT_SHADER, cubeFragmentShaderSource, "Cube");

    // Prelinking attributes
    std::unordered_map<std::string, GLuint> attributes = {
        {"a_position", ATTRIBUTE_POSITION},
        {"a_color", ATTRIBUTE_COLOR},
        {"a_normal", ATTRIBUTE_NORMAL},
        {"a_texcoord", ATTRIBUTE_TEXCOORD},
    };
    // Linking Shaders
    cubeShaderProgramObject = linkShadersAndAttributes(cubeVertexShader, cubeFragmentShader, attributes,"Cube");

    // Delete Shaders
    glDeleteShader(cubeVertexShader);
    glDeleteShader(cubeFragmentShader);

    // Generate cubeVAOc
    genVertexArrays(&cubeVAO);

    // Generate VBO
    genBuffers(&cubePositionVBO, cubePositions);
    vertexAttribPointer(ATTRIBUTE_POSITION,3);
    unbindBuffer();
    genBuffers(&cubeColorsVBO, cubeColors);
    vertexAttribPointer(ATTRIBUTE_COLOR, 3);
    unbindBuffer();
    genBuffers(&cubeNormalVBO,cubeNormals);
    vertexAttribPointer(ATTRIBUTE_NORMAL, 3);
    unbindBuffer();
    genBuffers(&cubeTextcorrdVBO,cubeTexCoords);
    vertexAttribPointer(ATTRIBUTE_TEXCOORD,2);
    unbindBuffer();

    // Generate cubeEBO;
    genBuffers(&cubeEBO,cubeIndices);

    // unbindVertexArray
    unbindVertexArray();

    return 0;
}

void cubeDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    // useprogram cubeShaderProgramObject
    glUseProgram(cubeShaderProgramObject);

    // Initialize modelMatrix, translateMatrix, rotateMatrix
    glm::mat4 modelMatrix = glm::mat4(1.0);
    glm::mat4 translateMatrix = glm::mat4(1.0);
    glm::mat4 rotateMatrix = glm::mat4(1.0);

    // Apply translation and rotation to model
    modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0f,0.0f,-4.0f));
    modelMatrix = glm::rotate(modelMatrix,angle,glm::vec3(1.0f,0.0f,0.0f));
    modelMatrix = glm::rotate(modelMatrix,angle,glm::vec3(0.0f,1.0f,0.0f));
    modelMatrix = glm::rotate(modelMatrix,angle,glm::vec3(0.0f,0.0f,1.0f));

    // set uniform matrix
    setUniformMatrix4fv(cubeShaderProgramObject,"u_projectionMatrix" , projectionMatrix);
    setUniformMatrix4fv(cubeShaderProgramObject,"u_viewMatrix" ,viewMatrix);
    setUniformMatrix4fv(cubeShaderProgramObject,"u_modelMatrix" ,modelMatrix);

    // Bind Vertex Array cubeVAO
    glBindVertexArray(cubeVAO);
    // Bind cubeEBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // Draw elements
    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
    
    // unbind cubeEBO and cubeVAO
    unbindBuffer();
    unbindVertexArray();
    glUseProgram(0);

} 
void cubeUpdate(GLfloat elapsedTime)
{
    // update angle
    angle += elapsedTime;
}
void cubeCleanup(void)
{
    if(cubeVAO)
    {
        glDeleteVertexArrays(1,&cubeVAO);
        cubeVAO = 0;
    } 
    if(cubePositionVBO)
    {
        glDeleteBuffers(1,&cubePositionVBO);
        cubePositionVBO= 0;
    }
    if(cubeColorsVBO)
    {
        glDeleteBuffers(1,&cubeColorsVBO);
        cubeColorsVBO = 0;
    }
    if(cubeEBO)
    {
        glDeleteBuffers(1,&cubeEBO);
        cubeEBO = 0;
    }
    if(cubeShaderProgramObject)
    {

    }
}