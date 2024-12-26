#include "pyramid.h"

using namespace OGL;

#include <vector>
#include <glm/glm.hpp>
#include <cmath>


// data: 
std::vector<glm::vec3> pyramidPositions; 
std::vector<glm::vec3> pyramidColors; 
std::vector<glm::vec2> pyramidTexcoords; 
std::vector<glm::vec3> pyramidNormals; 
std::vector<GLuint>  pyramidIndices; 

// // shader: 
const char* pyramidVertexShaderSource = R"(
#version 460 core
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;
out vec3 a_position_out;
out vec3 a_normal_out;
out vec2 a_uv_out;
void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position,1.0f);
    gl_PointSize = 5.0f;
    a_position_out = a_position;
    a_normal_out = a_normal;
    a_uv_out = a_uv;

}

)";
const char* pyramidFragmentShaderSource = R"(
#version 460 core
in vec3 a_position_out;
in vec3 a_normal_out;
in vec2 a_uv_out;
out vec4 FragColor;
void main()
{   
    //float lightIntensity = max(dot(a_normal_out, normalize(vec3(0.0f, 0.0f, -1.0f))), 0.0f); // Simple lighting
    //FragColor = vec4(lightIntensity,lightIntensity,lightIntensity,1.0f);
    FragColor = vec4(a_uv_out,1.0f,1.0f);
}

)";

GLuint pyramidVertexShader; 
GLuint pyramidFragmentShader; 
std::unordered_map<std::string, GLuint> attributes; 
GLuint pyramidShaderProgramObject;

// buffer:
GLuint pyramidVAO; 
GLuint pyramidPositionsVBO; 
GLuint pyramidColorsVBO; 
GLuint pyramidTexcoordsVBO; 
GLuint pyramidNormalsVBO; 
GLuint pyramidIndicesEBO;

// // update: 
GLfloat pyramidAngle;


Logger pyramid("Pyramid.log");


void generateUVSphere(float radius, int latitudeDivisions, int longitudeDivisions,
                      std::vector<glm::vec3>& positions, 
                      std::vector<glm::vec3>& normals, 
                      std::vector<glm::vec2>& uvs, 
                      std::vector<GLuint>& indices) {
    
    // Clear any existing data
    positions.clear();
    normals.clear();
    uvs.clear();
    indices.clear();
    
    // Step size for latitude and longitude
    const float latStep = glm::pi<float>() / latitudeDivisions;
    const float lonStep = 2.0f * glm::pi<float>() / longitudeDivisions;

    // Generate vertices for the sphere
    for (int lat = 0; lat <= latitudeDivisions; ++lat) {
        for (int lon = 0; lon <= longitudeDivisions; ++lon) {
            // Latitude angle
            float theta = lat * latStep;  // Latitude from 0 to pi (North to South)
            float phi = lon * lonStep;    // Longitude from 0 to 2*pi (Around the sphere)
            
            // Compute spherical coordinates
            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);
            
            // Store position
            positions.push_back(glm::vec3(x, y, z));
            
            // Compute normal (just normalize the position)
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
            
            // Store UV coordinates
            float u = (float)lon / longitudeDivisions;
            float v = (float)lat / latitudeDivisions;
            uvs.push_back(glm::vec2(u, v));
        }
    }

    // Generate indices for the sphere (two triangles per face)
    for (int lat = 0; lat < latitudeDivisions; ++lat) {
        for (int lon = 0; lon < longitudeDivisions; ++lon) {
            int current = lat * (longitudeDivisions + 1) + lon;
            int next = current + longitudeDivisions + 1;

            // Two triangles per square face
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
}

// functions: 
int pyramidInit(void)
{
    //generateSphereGeometry(1.0f, 36, 18, pyramidPositions, pyramidNormals, pyramidTexcoords, pyramidIndices);
    //generatePlaneGeometry(2.0f, 2.0f, 10, 10, pyramidPositions, pyramidNormals, pyramidTexcoords, pyramidIndices);
    //generateUVSphere(1.0f, 20, 40, pyramidPositions, pyramidNormals, pyramidTexcoords, pyramidIndices);
    //Shader build and compile
    pyramidVertexShader =   createShader(GL_VERTEX_SHADER, pyramidVertexShaderSource,"Pyramid");
    pyramidFragmentShader = createShader(GL_FRAGMENT_SHADER, pyramidFragmentShaderSource,"Pyramid");

    // prelinking
    attributes = {
        {"a_position",ATTRIBUTE_POSITION},
        {"a_normal",ATTRIBUTE_NORMAL},
        {"a_uv",ATTRIBUTE_TEXCOORD},
    };
    //linking program
    pyramidShaderProgramObject = linkShadersAndAttributes(pyramidVertexShader, pyramidFragmentShader,attributes, "Pyramid");

    //  VAO
    genVertexArrays(&pyramidVAO);
    //  VBO
    genBuffers(& pyramidPositionsVBO , pyramidPositions  );
    vertexAttribPointer(ATTRIBUTE_POSITION,3, sizeof(glm::vec3));
    unbindBuffer();
    genBuffers(& pyramidNormalsVBO , pyramidNormals  );
    vertexAttribPointer(ATTRIBUTE_NORMAL,3,sizeof(glm::vec3));
    unbindBuffer();
    genBuffers(& pyramidTexcoordsVBO , pyramidTexcoords  );
    vertexAttribPointer(ATTRIBUTE_TEXCOORD,2,sizeof(glm::vec2));
    unbindBuffer();

    genBuffers(&pyramidIndicesEBO,pyramidIndices);
    
    //  unbind
    //  unbindVAO
    unbindVertexArray();
    glDisable(GL_CULL_FACE);
   // glPointSize(2.0f); // Set point size to 5 pixels
    // Enable point rendering mode
    // Enable OpenGL features
    return(0);
}

void pyramidDisplay(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(pyramidShaderProgramObject);
    // modelMatrix, translateMatrix and rotateMatrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
        
    modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0f,0.0f,-4.0f));
    modelMatrix = glm::rotate(modelMatrix,pyramidAngle,glm::vec3(1.0f,0.0f,0.0f));
    modelMatrix = glm::rotate(modelMatrix,pyramidAngle,glm::vec3(0.0f,1.0f,0.0f));
    modelMatrix = glm::rotate(modelMatrix,pyramidAngle,glm::vec3(0.0f,0.0f,1.0f));

    // setuniformMaritx
    setUniformMatrix4fv(pyramidShaderProgramObject,"u_projectionMatrix" ,projectionMatrix);
    setUniformMatrix4fv(pyramidShaderProgramObject,"u_viewMatrix" ,viewMatrix);
    setUniformMatrix4fv(pyramidShaderProgramObject,"u_modelMatrix" ,modelMatrix);


    // bind vao
    glBindVertexArray(pyramidVAO);
    // binf ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIndicesEBO);
    // draw elements
    glDrawElements(GL_POINTS, pyramidIndices.size(), GL_UNSIGNED_INT, 0);
    // unbind ebo,vao,program
    unbindBuffer();
    unbindVertexArray();
    glUseProgram(0);


}
void pyramidUpdate(GLfloat elapsedTime)
{
    pyramidAngle += elapsedTime;
}


void pyramidCleanup(void)
{
    if(pyramidVAO)
    {
        glDeleteVertexArrays(1,&pyramidVAO);
        pyramidVAO = 0;
    }
    if(pyramidPositionsVBO)
    {
        glDeleteBuffers(1,&pyramidPositionsVBO);
        pyramidPositionsVBO = 0;
    }
    if(pyramidColorsVBO)
    {
        glDeleteBuffers(1,&pyramidColorsVBO);
        pyramidColorsVBO = 0;
    }
    if(pyramidTexcoordsVBO)
    {
        glDeleteBuffers(1,&pyramidTexcoordsVBO);
        pyramidTexcoordsVBO = 0;
    }
    if(pyramidNormalsVBO)
    {
        glDeleteBuffers(1,&pyramidNormalsVBO);
        pyramidNormalsVBO = 0;
    }
    if(pyramidIndicesEBO)
    {
        glDeleteBuffers(1,&pyramidIndicesEBO);
        pyramidIndicesEBO = 0;
    }
    if(pyramidShaderProgramObject)
    {
        deleteShaderProgramObject(pyramidShaderProgramObject);
        pyramidShaderProgramObject = 0;
    }
}


