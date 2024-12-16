
#include "plane.h"

const char* planeVertexShaderSource = R"(
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

const char* planeFragmentShaderSource = R"(
#version 460 core
in vec4 a_color_out;
out vec4 FragColor;
void main()
{
    FragColor = a_color_out;
}
)";

std::vector<GLfloat> planePosition = {
   -0.5f,  0.5f, 0.0f,  // Top-left
   -0.5f, -0.5f, 0.0f,  // Bottom-left
    0.5f, -0.5f, 0.0f,  // Bottom-right
    0.5f,  0.5f, 0.0f   // Top-right
};
std::vector<GLfloat> planeColors = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f, 
    1.0f, 1.0f, 0.0f, 
};
std::vector<GLuint>  planeIndices = {
    0, 1, 2,  // First triangle
    2, 3, 0   // Second triangle
};

GLuint planeVAO, planePositionVBO, planeColorVBO, planeEBO;
GLuint planeVertexShader, planeFragmentShader;
GLuint planeShaderProgramObject;

void planeInit(void)
{
    //shaders creation & compilation
    planeVertexShader = createShader(GL_VERTEX_SHADER,planeVertexShaderSource);
    planeFragmentShader = createShader(GL_FRAGMENT_SHADER, planeFragmentShaderSource);

    //prelinking attribute 
    std::unordered_map<std::string, GLuint> attributes = {
        {"a_position",ATTRIBUTE_POSITION},
        {"a_color",ATTRIBUTE_COLOR}
    };

    //shaders linking
    planeShaderProgramObject = linkShadersAndAttributes(planeVertexShader, planeFragmentShader, attributes);
    
    //delete planeVertexShader
    glDeleteShader(planeVertexShader);
    //delete planeFragmentShader
    glDeleteShader(planeFragmentShader);

    //generate planeVAO
    genVertexArrays(&planeVAO);
    //generate planePositionVBO
    genBuffers(&planePositionVBO,planePosition);
    vertexAttribPointer(ATTRIBUTE_POSITION, 3);
    unbindBuffer();

    //generate planeColorVBO
    genBuffers(&planeColorVBO,planeColors);
    vertexAttribPointer(ATTRIBUTE_COLOR, 3);
    unbindBuffer();
    
    //generate planeEBO
    genBuffers(&planeEBO,planeIndices);

    //unbind
    unbindVertexArray();   
}
void planeDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
            
        //useprogram planeShaderProgramObject
        glUseProgram(planeShaderProgramObject);

        //modelmatrix = identity
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix = glm::mat4(1.0f);
        //translateMatrix = modelmatrix * translate.z is -2
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f,0.0f,-2.0f));
        
        setUniformMatrix4fv(planeShaderProgramObject, "u_modelMatrix", modelMatrix);
        setUniformMatrix4fv(planeShaderProgramObject,"u_viewMatrix", viewMatrix);
        setUniformMatrix4fv(planeShaderProgramObject,"u_projectionMatrix", projectionMatrix);

        //bind vao
        glBindVertexArray(planeVAO);
        //bind ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,planeEBO);
        //drawelements
        glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);
        
        //unbind
        unbindBuffer();
        unbindVertexArray();
        //useprogram 0
        glUseProgram(0);
}

void planeCleanup(void)
{
        if(planeVAO)
        {
            glDeleteVertexArrays(1,&planeVAO);
            planeVAO = 0;
        }
        if(planePositionVBO)
        {
            glDeleteBuffers(1,&planePositionVBO);
            planePositionVBO = 0;
        }
        if(planeColorVBO)
        {
            glDeleteBuffers(1,&planeColorVBO);
            planeColorVBO = 0;
        }
        if(planeEBO)
        {
            glDeleteBuffers(1,&planeEBO);
            planeEBO = 0;
        }
        if(planeShaderProgramObject)
        {
            deleteShaderProgramObject(planeShaderProgramObject);
            planeShaderProgramObject = 0;
        }
}


/*

    planeInit():
        shaders creation
        shaders compilation
        prelinking attribute 
        shaders linking

        delete planeVertexShader
        delete planeFragmentShader
        
        generate planeVAO
        generate planePositionVBO
        generate planeColorVBO
        generate planeEBO
        unbind

    planeDisplay(viewMatrix, projectionMatrix):

    
    planeCleanup():
      
*/ 
